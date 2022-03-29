
#include "Sound.h"
#include "../PathManager.h"

CSound::CSound()	:
	m_Play(false),
	m_Loop(false),
	m_System(nullptr),
	m_Sound(nullptr),
	m_Group(nullptr),
	m_Channel(nullptr)
{
}

CSound::~CSound()
{
	if (m_Sound)
		m_Sound->release();
}

bool CSound::LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, 
	bool Loop, const std::string& Name, const char* FileName,
	const std::string& PathName)
{
	SetName(Name);

	m_System = System;
	m_Group = Group;
	m_Loop = Loop;

	char	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		strcpy_s(FullPath, Info->pPathMultibyte);

	strcat_s(FullPath, FileName);

	FMOD_MODE	Mode = FMOD_DEFAULT;

	if (Loop)
		Mode = FMOD_LOOP_NORMAL;

	if (m_System->createSound(FullPath, Mode, nullptr, &m_Sound) != FMOD_OK)
		return false;

	return true;
}

void CSound::Play()
{
	m_System->playSound(m_Sound, m_Group, false, &m_Channel);
	m_Play = true;
}

void CSound::Stop()
{
	if (m_Channel)
	{
		bool	Playing = false;

		m_Channel->isPlaying(&Playing);

		if (Playing)
		{
			m_Channel->stop();
			m_Channel = nullptr;
			m_Play = false;
		}
	}
}

void CSound::Pause()
{
	if (!m_Channel)
		return;

	bool	Playing = false;

	m_Channel->isPlaying(&Playing);

	if (Playing)
		m_Channel->setPaused(true);

	m_Play = false;
}

void CSound::Resume()
{
	if (!m_Channel)
		return;

	bool	Playing = false;

	m_Channel->isPlaying(&Playing);

	if (!Playing)
		m_Channel->setPaused(false);

	m_Play = true;
}

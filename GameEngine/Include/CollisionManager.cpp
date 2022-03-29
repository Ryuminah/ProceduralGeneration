#include "CollisionManager.h"

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	{
		size_t	Size = m_vecChannel.size();

		for (size_t i = 0; i < Size; ++i)
		{
			SAFE_DELETE(m_vecChannel[i]);
		}
	}

	{
		auto	iter = m_mapProfile.begin();
		auto	iterEnd = m_mapProfile.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}
	}
}

bool CCollisionManager::Init()
{
	CreateChannel("Static", Collision_Interaction::Block);
	CreateChannel("Player", Collision_Interaction::Block);
	CreateChannel("Monster", Collision_Interaction::Block);

	CreateProfile("Static", Collision_Channel::Static);
	CreateProfile("Player", Collision_Channel::Player);
	CreateProfile("Monster", Collision_Channel::Monster);

	SetProfileChannelState("Player", Collision_Channel::Player, 
		Collision_Interaction::Ignore);

	return true;
}

bool CCollisionManager::CreateChannel(const std::string& Name, 
	Collision_Interaction Interaction)
{
	if (m_vecChannel.size() == (size_t)Collision_Channel::End)
		return false;

	CollisionChannel* Channel = new CollisionChannel;
	Channel->Name = Name;
	Channel->Interaction = Interaction;
	Channel->Channel = (Collision_Channel)m_vecChannel.size();

	m_vecChannel.push_back(Channel);

	auto	iter = m_mapProfile.begin();
	auto	iterEnd = m_mapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->vecChannel.push_back(*Channel);
	}

	return true;
}

bool CCollisionManager::CreateProfile(const std::string& Name, Collision_Channel Channel)
{
	CollisionProfile* Profile = FindProfile(Name);

	if (Profile)
		return false;

	Profile = new CollisionProfile;

	Profile->Name = Name;
	Profile->Channel = Channel;

	m_mapProfile.insert(std::make_pair(Name, Profile));

	size_t	Size = m_vecChannel.size();

	for (size_t i = 0; i < Size; ++i)
	{
		Profile->vecChannel.push_back(*m_vecChannel[i]);
	}

	return true;
}

bool CCollisionManager::SetProfileChannelState(const std::string& Name, 
	Collision_Channel DestChannel, Collision_Interaction Interaction)
{
	CollisionProfile* Profile = FindProfile(Name);

	if (!Profile)
		return false;

	Profile->vecChannel[(int)DestChannel].Interaction = Interaction;

	return true;
}

CollisionProfile* CCollisionManager::FindProfile(const std::string& Name)
{
	auto	iter = m_mapProfile.find(Name);

	if (iter == m_mapProfile.end())
		return nullptr;

	return iter->second;
}

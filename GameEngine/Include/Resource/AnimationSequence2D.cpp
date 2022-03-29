
#include "AnimationSequence2D.h"

CAnimationSequence2D::CAnimationSequence2D()	:
	m_Type(Animation2D_Type::Atlas),
	m_Loop(true),
	m_PlayRate(1.f),
	m_PlayTime(1.f),
	m_FrameTime(1.f),
	m_pScene(nullptr)	
{
}

CAnimationSequence2D::~CAnimationSequence2D()
{
	{
		auto	iter = m_vecNotify.begin();
		auto	iterEnd = m_vecNotify.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE((*iter));
		}
	}
}

void CAnimationSequence2D::SetTexture(CTexture* pTexture)
{
	m_Texture = pTexture;

	switch (pTexture->GetImageType())
	{
	case Image_Type::Atlas:
		m_Type = Animation2D_Type::Atlas;
		break;
	case Image_Type::Frame:
		m_Type = Animation2D_Type::Frame;
		break;
	case Image_Type::Array:
		m_Type = Animation2D_Type::Array;
		break;
	}
}

void CAnimationSequence2D::SetPlayTime(float PlayTime)
{
	m_PlayTime = PlayTime;
}

void CAnimationSequence2D::SetPlayRate(float PlayRate)
{
	m_PlayRate = PlayRate;
}

void CAnimationSequence2D::AddFrame(const Vector2& Start, const Vector2& End)
{
	Animation2DFrame	Frame;
	Frame.Start = Start;
	Frame.End = End;
	m_vecAnimFrame.push_back(Frame);

	m_FrameTime = m_PlayTime / (float)m_vecAnimFrame.size();
}

void CAnimationSequence2D::AddFrame(const Vector2& Start, const Vector2& End, int Count)
{
	m_vecAnimFrame.resize(Count);

	for (int i = 0; i < Count; ++i)
	{
		m_vecAnimFrame[i].Start = Start;
		m_vecAnimFrame[i].End = End;
	}

	m_FrameTime = m_PlayTime / (float)Count;
}

void CAnimationSequence2D::AddNotify(const std::string& Name, int Frame)
{
	Animation2DNotify* pNotify = new Animation2DNotify;

	pNotify->Name = Name;
	pNotify->Frame = Frame;
	pNotify->pOwner = this;

	m_vecNotify.push_back(pNotify);
}

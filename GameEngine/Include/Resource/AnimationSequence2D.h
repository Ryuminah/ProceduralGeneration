#pragma once

#include "../Ref.h"
#include "Texture.h"

struct Animation2DFrame
{
	Vector2	Start;
	Vector2	End;
};

struct Animation2DNotify
{
	std::string			Name;
	class CAnimationSequence2D* pOwner;
	int				Frame;
	bool			Call;

	Animation2DNotify()
	{
		Call = false;
	}
};

class CAnimationSequence2D :
	public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;
	friend class CAnimation2D;

private:
	CAnimationSequence2D();
	~CAnimationSequence2D();

private:
	class CScene* m_pScene;
	CSharedPtr<CTexture>    m_Texture;
	Animation2D_Type        m_Type;
	bool					m_Loop;
	float                   m_PlayRate;
	float                   m_PlayTime;
	float					m_FrameTime;
	std::vector<Animation2DFrame>	m_vecAnimFrame;
	std::vector<Animation2DNotify*>	m_vecNotify;

public:
	Animation2D_Type GetAnimation2DType()	const
	{
		return m_Type;
	}

	const Animation2DFrame& GetAnimation2DFrame(int Index = 0)	const
	{
		return m_vecAnimFrame[Index];
	}

	Animation2DFrame GetAnimation2DFrameUV(int Index)	const
	{
		Animation2DFrame	Frame = {};

		unsigned int Width = m_Texture->GetWidth();
		unsigned int Height = m_Texture->GetHeight();

		Frame.Start = m_vecAnimFrame[Index].Start / Vector2((float)Width, (float)Height);
		Frame.End = m_vecAnimFrame[Index].End / Vector2((float)Width, (float)Height);

		return Frame;
	}

public:
	void SetTexture(CTexture* pTexture);
	void SetPlayTime(float PlayTime);
	void SetPlayRate(float PlayRate);
	void AddFrame(const Vector2& Start, const Vector2& End);
	void AddFrame(const Vector2& Start, const Vector2& End, int Count);
	void AddNotify(const std::string& Name, int Frame);
};


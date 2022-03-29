#pragma once

#include "../Ref.h"
#include "AnimationSequence2D.h"

struct Sequence2DInfo
{
	CSharedPtr<CAnimationSequence2D>	Sequence;
	bool				Loop;
	float				PlayRate;

	Sequence2DInfo() :
		Loop(true),
		PlayRate(1.f)
	{
	}
};

class CAnimation2D	:
	public CRef
{
	friend class CSpriteComponent;
	friend class CMouseWidget;

protected:
	CAnimation2D();
	CAnimation2D(const CAnimation2D& anim);
	virtual ~CAnimation2D();

protected:
	class CScene* m_pScene;
	class CSpriteComponent* m_Owner;
	class CWidget* m_OwnerWidget;
	class CAnimation2DConstantBuffer* m_pCBuffer;
	std::unordered_map<std::string, Sequence2DInfo*>	m_mapSequence;
	Sequence2DInfo*	m_CurrentSequence;
	float		m_FrameTime;
	int			m_Frame;

protected:
	std::function<void(const std::string&)>	m_FrameEndFunction;

public:
	void SetScene(class CScene* pScene)
	{
		m_pScene = pScene;
	}

	void SetOwner(class CSpriteComponent* Owner)
	{
		m_Owner = Owner;
	}

	void SetOwner(class CWidget* Owner)
	{
		m_OwnerWidget = Owner;
	}

	std::string GetCurrentSequenceName()	const
	{
		return m_CurrentSequence->Sequence->GetName();
	}


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual CAnimation2D* Clone();
	void SetShader();
	void ResetShader();

public:
	void AddAnimationSequence2D(const std::string& Name, bool Loop = true);
	void AddAnimationSequence2D(CAnimationSequence2D* Sequence, bool Loop = true);
	void SetSequencePlayRate(const std::string& Name, float PlayRate);
	void AddSequencePlayRate(const std::string& Name, float PlayRate);
	void ChangeAnimation(const std::string& Name);

private:
	Sequence2DInfo* FindSequence(const std::string& Name);

public:
	template <typename T>
	void SetFrameEndFunction(T* pObj, void(T::* pFunc)(const std::string&))
	{
		m_FrameEndFunction = std::bind(pFunc, pObj, std::placeholders::_1);
	}
};


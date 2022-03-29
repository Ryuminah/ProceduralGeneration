#pragma once

#include "../Ref.h"
#include "AnimationSequence.h"

class CAnimation :
	public CRef
{
public:
	CAnimation();
	CAnimation(const CAnimation& Anim);
	virtual ~CAnimation();

protected:
	class CScene* m_Scene;

public:
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

private:
	std::unordered_map<std::string,  CSharedPtr<CAnimationSequence>>  m_mapSequence;
	CSharedPtr<CAnimationSequence>  m_Sequence;
	CSharedPtr<CAnimationSequence>  m_ChangeSequence;
	class CSkeleton* m_Skeleton;
	class CAnimationMeshComponent* m_Owner;
	Vector3         m_BlendScale;
	Vector4         m_BlendRot;
	Vector3         m_BlendPos;
	bool            m_End;
	float           m_GlobalTime;
	float           m_SequenceProgress;
	float           m_ChangeTimeAcc;
	float           m_ChangeTime;

private:
	CSharedPtr<class CComputeShader> m_AnimationUpdateShader;
	class CAnimationUpdateConstantBuffer* m_CBuffer;
	class CStructuredBuffer* m_OutputBuffer;	// 애니메이션 결과를 저장하기 위헌 버퍼
	class CStructuredBuffer* m_BoneBuffer;
	ID3D11Buffer* m_BoneDataBuffer;
	void* m_BoneResourceAddress;
	ID3D11Buffer* m_BoneOutputDataBuffer;
	void* m_BoneOutputResourceAddress;
	std::vector<Matrix>	m_vecBoneMatrix;

public:
	void SetOwner(class CAnimationMeshComponent* Owner)
	{
		m_Owner = Owner;
	}
	void SetSkeleton(class CSkeleton* Skeleton);

public:
	CAnimationSequence* GetSequence()	const
	{
		return m_Sequence;
	}

public:
	void AddAnimationSequence(const std::string& Name);
	void ChangeAnimation(const std::string& Name);

private:
	CAnimationSequence* FindSequence(const std::string& Name);

public:
	void Start();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void SetShader();
	void ResetShader();
	CAnimation* Clone();
};


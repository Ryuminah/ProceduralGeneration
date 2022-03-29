#include "Animation.h"
#include "Skeleton.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "StructuredBuffer.h"
#include "ComputeShader.h"
#include "AnimationUpdateConstantBuffer.h"
#include "ShaderManager.h"
#include "../Component/AnimationMeshComponent.h"
#include "../Device.h"

CAnimation::CAnimation()    :
	m_Skeleton(nullptr),
	m_AnimationUpdateShader(nullptr),
	m_CBuffer(nullptr),
	m_OutputBuffer(nullptr),
	m_BoneBuffer(nullptr),
	m_BoneDataBuffer(nullptr),
	m_BoneResourceAddress(nullptr),
	m_GlobalTime(0.f),
	m_End(false),
	m_Owner(nullptr),
	m_SequenceProgress(0.f),
	m_ChangeTimeAcc(0.f),
	m_ChangeTime(0.2f)
{
}

CAnimation::CAnimation(const CAnimation& Anim)
{
	if (Anim.m_Skeleton)
		m_Skeleton = Anim.m_Skeleton->Clone();

	m_GlobalTime = 0.f;
	m_End = false;
	m_SequenceProgress = 0.f;
	m_ChangeTimeAcc = 0.f;
	m_ChangeTime = Anim.m_ChangeTime;

	if (Anim.m_CBuffer)
		m_CBuffer = Anim.m_CBuffer->Clone();

	if (Anim.m_OutputBuffer)
		m_OutputBuffer = Anim.m_OutputBuffer->Clone();

	if (Anim.m_BoneBuffer)
		m_BoneBuffer = Anim.m_BoneBuffer->Clone();

	m_AnimationUpdateShader = Anim.m_AnimationUpdateShader;

	m_mapSequence.clear();

	auto	iter = Anim.m_mapSequence.begin();
	auto	iterEnd = Anim.m_mapSequence.end();

	for (; iter != iterEnd; ++iter)
	{
		m_mapSequence.insert(std::make_pair(iter->first, iter->second));
	}

	m_Sequence = FindSequence(Anim.m_Sequence->GetName());


	D3D11_BUFFER_DESC	Desc = {};

	Desc.ByteWidth = sizeof(Matrix) * (unsigned int)m_Skeleton->GetBoneCount();
	Desc.BindFlags = 0;
	Desc.Usage = D3D11_USAGE_STAGING;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, nullptr, &m_BoneDataBuffer);

	D3D11_MAPPED_SUBRESOURCE	Map = {};

	CDevice::GetInst()->GetContext()->Map(m_BoneDataBuffer, 0, D3D11_MAP_READ, 0, &Map);

	m_BoneResourceAddress = Map.pData;

	CDevice::GetInst()->GetContext()->Unmap(m_BoneDataBuffer, 0);
}

CAnimation::~CAnimation()
{
	SAFE_RELEASE(m_BoneDataBuffer);
	SAFE_RELEASE(m_BoneOutputDataBuffer);
	SAFE_DELETE(m_CBuffer);
	SAFE_DELETE(m_OutputBuffer);
	SAFE_DELETE(m_BoneBuffer);

	SAFE_RELEASE(m_Skeleton);
}

void CAnimation::SetSkeleton(CSkeleton* Skeleton)
{
	SAFE_RELEASE(m_Skeleton);

	if (Skeleton)
		m_Skeleton = Skeleton->Clone();
}

void CAnimation::AddAnimationSequence(const std::string& Name)
{
	CAnimationSequence* Sequence = FindSequence(Name);

	if (Sequence)
		return;

	Sequence = m_Scene->GetResource()->FindAnimationSequence(Name);

	if (!Sequence)
		return;

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	if (!m_Sequence)
		m_Sequence = Sequence;
}

void CAnimation::ChangeAnimation(const std::string& Name)
{
	if (m_mapSequence.empty())
		return;

	if (m_Sequence->GetName() == Name)
		return;

	m_ChangeSequence = FindSequence(Name);
}

CAnimationSequence* CAnimation::FindSequence(const std::string& Name)
{
	auto    iter = m_mapSequence.find(Name);

	if (iter == m_mapSequence.end())
		return nullptr;

	return iter->second;
}

void CAnimation::Start()
{
	m_AnimationUpdateShader = (CComputeShader*)CShaderManager::GetInst()->FindShader("AnimationUpdateShader");

	m_CBuffer = new CAnimationUpdateConstantBuffer;

	m_CBuffer->Init();

	m_vecBoneMatrix.resize(m_Skeleton->GetBoneCount());

	m_OutputBuffer = new CStructuredBuffer;

	m_OutputBuffer->Init("OutputBone", sizeof(Matrix), (unsigned int)m_Skeleton->GetBoneCount(),
		0, false, CBT_COMPUTE);

	m_BoneBuffer = new CStructuredBuffer;

	m_BoneBuffer->Init("BoneBuffer", sizeof(Matrix), (unsigned int)m_Skeleton->GetBoneCount(),
		1, false, CBT_COMPUTE);

	D3D11_BUFFER_DESC	Desc = {};

	Desc.ByteWidth = sizeof(Matrix) * (unsigned int)m_Skeleton->GetBoneCount();
	Desc.BindFlags = 0;
	Desc.Usage = D3D11_USAGE_STAGING;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, nullptr, &m_BoneDataBuffer);

	D3D11_MAPPED_SUBRESOURCE	Map = {};

	CDevice::GetInst()->GetContext()->Map(m_BoneDataBuffer, 0, D3D11_MAP_READ, 0, &Map);

	m_BoneResourceAddress = Map.pData;

	CDevice::GetInst()->GetContext()->Unmap(m_BoneDataBuffer, 0);



	Desc = {};

	Desc.ByteWidth = sizeof(Matrix) * (unsigned int)m_Skeleton->GetBoneCount();
	Desc.BindFlags = 0;
	Desc.Usage = D3D11_USAGE_STAGING;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, nullptr, &m_BoneOutputDataBuffer);

	CDevice::GetInst()->GetContext()->Map(m_BoneOutputDataBuffer, 0, D3D11_MAP_READ, 0, &Map);

	m_BoneOutputResourceAddress = Map.pData;

	CDevice::GetInst()->GetContext()->Unmap(m_BoneOutputDataBuffer, 0);
}

void CAnimation::Update(float DeltaTime)
{
	if (!m_Skeleton)
		return;

	m_End = false;

	m_GlobalTime += DeltaTime;

	bool	Change = false;
	bool	ChangeEnd = false;

	// 바꿀 모션이 있을 경우
	if (m_ChangeSequence)
	{
		Change = true;

		m_ChangeTimeAcc += DeltaTime;

		if (m_ChangeTimeAcc >= m_ChangeTime)
		{
			m_ChangeTimeAcc = m_ChangeTime;
			ChangeEnd = true;
		}
	}

	// 바꿀 모션이 없을 경우
	else
	{
		m_SequenceProgress = m_GlobalTime / m_Sequence->m_TimeLength;

		while (m_GlobalTime >= m_Sequence->m_TimeLength)
		{
			m_GlobalTime -= m_Sequence->m_TimeLength;

			m_End = true;
		}

		m_CBuffer->SetChangeAnimation(false);
		m_CBuffer->SetChangeRatio(0.f);
		m_CBuffer->SetChangeFrameCount(0);

		float	AnimationTime = m_GlobalTime + m_Sequence->m_StartTime;

		int	StartFrame = 0;
		int	EndFrame = m_Sequence->m_FrameLength;

		int	FrameIndex = (int)(AnimationTime / m_Sequence->m_FrameTime);
		int	NextFrameIndex = FrameIndex + 1;

		if (NextFrameIndex >= EndFrame)
			NextFrameIndex = StartFrame;

		float	Ratio = (AnimationTime - m_Sequence->m_FrameTime * FrameIndex) / m_Sequence->m_FrameTime;

		m_CBuffer->SetFrameCount(EndFrame);
		m_CBuffer->SetCurrentFrame(FrameIndex);
		m_CBuffer->SetNextFrame(NextFrameIndex);
		m_CBuffer->SetRatio(Ratio);
		m_CBuffer->SetBoneCount((int)m_Skeleton->GetBoneCount());

		if (m_End)
		{
			auto	iter = m_Sequence->m_vecNotify.begin();
			auto	iterEnd = m_Sequence->m_vecNotify.end();

			for (; iter != iterEnd; ++iter)
			{
				if ((*iter)->Time == AnimationTime && !(*iter)->Call)
				{
					(*iter)->Call = true;

					if (m_Owner)
						m_Owner->CallNotify((*iter)->Name);

					//else
					//	m_OwnerWidget->CallAnimNotify((*iter)->Name);
				}
			}
		}
	}

	if (Change)
	{
		m_CBuffer->SetChangeRatio(m_ChangeTimeAcc / m_ChangeTime);
		m_CBuffer->SetChangeAnimation(true);
		m_CBuffer->SetChangeFrameCount(m_ChangeSequence->m_FrameLength);

		m_ChangeSequence->SetChangeShader();
	}

	m_CBuffer->SetRowIndex(0);

	// Bone 결과 버퍼 갱신
	m_OutputBuffer->SetShader(0);

	// 상수버퍼를 갱신한다.
	m_CBuffer->UpdateCBuffer();

	// Sequence에 있는 구조화 버퍼를 갱신한다.
	m_Sequence->SetShader();

	// Offset 행렬을 갱신한다.
	m_Skeleton->SetShader();

	m_BoneBuffer->SetShader(1);

	unsigned int	GroupX = (unsigned int)m_Skeleton->GetBoneCount() / 256 + 1;

	m_AnimationUpdateShader->Excute(GroupX, 1, 1);

	m_OutputBuffer->ResetShader(0);

	m_BoneBuffer->ResetShader(1);

	m_Sequence->ResetShader();

	m_Skeleton->ResetShader();

	if (Change)
	{
		m_ChangeSequence->ResetChangeShader();
	}

	if (ChangeEnd)
	{
		m_Sequence = m_ChangeSequence;
		m_ChangeSequence = nullptr;
		m_ChangeTimeAcc = 0.f;
		m_GlobalTime = 0.f;
	}

	CDevice::GetInst()->GetContext()->CopyResource(m_BoneDataBuffer, m_BoneBuffer->GetBuffer());

	memcpy(&m_vecBoneMatrix[0], m_BoneResourceAddress, sizeof(Matrix) * m_vecBoneMatrix.size());
}

void CAnimation::PostUpdate(float DeltaTime)
{
	m_Skeleton->Update(DeltaTime, m_vecBoneMatrix, m_Owner->GetWorldMatrix());
}

void CAnimation::SetShader()
{
	m_OutputBuffer->SetShader(106, CBT_VERTEX);
}

void CAnimation::ResetShader()
{
	m_OutputBuffer->ResetShader(106, CBT_VERTEX);
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

#include "Skeleton.h"
#include "../Device.h"
#include "../PathManager.h"
#include "StructuredBuffer.h"
#include "BoneSocket.h"

CSkeleton::CSkeleton() :
	m_pOffsetMatrixBuffer(nullptr)
{
}

CSkeleton::CSkeleton(const CSkeleton& skeleton)
{
	for (size_t i = 0; i < skeleton.m_vecBones.size(); ++i)
	{
		Bone*	pBone = new Bone;

		*pBone = *skeleton.m_vecBones[i];

		m_vecBones.push_back(pBone);
	}

	m_pOffsetMatrixBuffer = nullptr;

	m_BoneSocket.clear();

	size_t	Size = skeleton.m_BoneSocket.size();

	for (size_t i = 0; i < Size; ++i)
	{
		CBoneSocket* Socket = skeleton.m_BoneSocket[i]->Clone();

		m_BoneSocket.push_back(Socket);
	}
}

CSkeleton::~CSkeleton()
{
	SAFE_DELETE(m_pOffsetMatrixBuffer);

	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		SAFE_DELETE(m_vecBones[i]);
	}

	m_vecBones.clear();

	size_t	Size = m_BoneSocket.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_BoneSocket[i]);
	}

	m_BoneSocket.clear();
}

size_t CSkeleton::GetBoneCount() const
{
	return m_vecBones.size();
}

Bone* CSkeleton::GetBone(int iIndex) const
{
	return m_vecBones[iIndex];
}

Bone* CSkeleton::GetBone(const std::string& strName) const
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->strName == strName)
			return m_vecBones[i];
	}

	return nullptr;
}

int CSkeleton::GetBoneIndex(const std::string& strName) const
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->strName == strName)
			return (int)i;
	}

	return -1;
}

bool CSkeleton::CheckBone(const std::string& Name) const
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->strName == Name)
			return true;
	}

	return false;
}

const Matrix& CSkeleton::GetBoneMatrix(const std::string& strName) const
{
	int	iIndex = GetBoneIndex(strName);

	return m_vecBones[iIndex]->matBone;
}

const Matrix& CSkeleton::GetBoneMatrix(int iIndex) const
{
	return m_vecBones[iIndex]->matBone;
}

CBoneSocket* CSkeleton::GetSocket(const std::string& Name)
{
	size_t	Size = m_BoneSocket.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_BoneSocket[i]->m_Name == Name)
			return m_BoneSocket[i];
	}

	return nullptr;
}

void CSkeleton::AddBone(Bone* pBone)
{
	m_vecBones.push_back(pBone);
}

bool CSkeleton::SaveSkeleton(const char* pFileName,
	const std::string& strPathName)
{
	const PathInfo* pPath = CPathManager::GetInst()->FindPath(strPathName);

	char	strFullPath[MAX_PATH] = {};

	if (pPath)
		strcpy_s(strFullPath, pPath->pPathMultibyte);

	strcat_s(strFullPath, pFileName);

	return SaveSkeletonFullPath(strFullPath);
}

bool CSkeleton::SaveSkeletonFullPath(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (!pFile)
		return false;

	size_t	iBoneCount = m_vecBones.size();

	fwrite(&iBoneCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iBoneCount; ++i)
	{
		size_t	iNameCount = m_vecBones[i]->strName.size();

		fwrite(&iNameCount, sizeof(size_t), 1, pFile);
		fwrite(m_vecBones[i]->strName.c_str(), 1, iNameCount, pFile);

		fwrite(&m_vecBones[i]->iDepth, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i]->iParentIndex, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i]->matOffset, sizeof(Matrix), 1, pFile);
		fwrite(&m_vecBones[i]->matBone, sizeof(Matrix), 1, pFile);
	}

	fclose(pFile);

	return true;
}

bool CSkeleton::LoadSkeleton(CScene* pScene, const std::string& strName,
	const char* pFileName, const std::string& strPathName)
{
	const PathInfo* pPath = CPathManager::GetInst()->FindPath(strPathName);

	char	strFullPath[MAX_PATH] = {};

	if (pPath)
		strcpy_s(strFullPath, pPath->pPathMultibyte);

	strcat_s(strFullPath, pFileName);

	return LoadSkeletonFullPath(pScene, strName, strFullPath);
}

bool CSkeleton::LoadSkeletonFullPath(CScene* pScene, const std::string& strName,
	const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rb");

	if (!pFile)
		return false;

	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		--m_vecBones[i]->iRefCount;

		if (m_vecBones[i]->iRefCount == 0)
		{
			SAFE_DELETE(m_vecBones[i]);
		}
	}

	m_vecBones.clear();

	size_t	iBoneCount = 0;

	fread(&iBoneCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iBoneCount; ++i)
	{
		Bone*	pBone = new Bone;

		m_vecBones.push_back(pBone);

		size_t	iNameCount = 0;

		char	strName[256] = {};
		fread(&iNameCount, sizeof(size_t), 1, pFile);
		fread(strName, 1, iNameCount, pFile);
		pBone->strName = strName;

		fread(&pBone->iDepth, sizeof(int), 1, pFile);
		fread(&pBone->iParentIndex, sizeof(int), 1, pFile);
		fread(&pBone->matOffset, sizeof(Matrix), 1, pFile);
		fread(&pBone->matBone, sizeof(Matrix), 1, pFile);
	}

	fclose(pFile);

	return true;
}

void CSkeleton::SetShader()
{
	//CONTEXT->VSSetShaderResources(3, 1, &m_pBoneSRV);
	if (!m_pOffsetMatrixBuffer)
	{
		m_pOffsetMatrixBuffer = new CStructuredBuffer;

		std::vector<Matrix>	vecOffset;

		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffset.push_back(m_vecBones[i]->matOffset);
		}

		m_pOffsetMatrixBuffer->Init("OffsetMatrixBuffer", sizeof(Matrix),
			(unsigned int)vecOffset.size(), 14, true, CBT_COMPUTE);

		m_pOffsetMatrixBuffer->UpdateBuffer(&vecOffset[0], sizeof(Matrix) * (int)vecOffset.size());
	}

	m_pOffsetMatrixBuffer->SetShader(14);
}

void CSkeleton::ResetShader()
{
	m_pOffsetMatrixBuffer->ResetShader(14);
}

void CSkeleton::AddSocket(const std::string& BoneName, const std::string& SocketName, const Vector3& Offset,
	const Vector3& OffsetRot)
{
	if (!CheckBone(BoneName))
		return;

	CBoneSocket* Socket = new CBoneSocket;

	Socket->m_BoneName = BoneName;
	Socket->m_Name = SocketName;
	Socket->m_Offset = Offset;
	Socket->m_OffsetRot = OffsetRot;
	Socket->m_BoneIndex = GetBoneIndex(BoneName);

	m_BoneSocket.push_back(Socket);
}

void CSkeleton::Update(float fTime, const std::vector<Matrix>& vecBoneMatrix, const Matrix& matWorld)
{
	size_t	Size = m_BoneSocket.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_BoneSocket[i]->Update(vecBoneMatrix[m_BoneSocket[i]->m_BoneIndex] * matWorld);
		//m_BoneSocket[i]->Update(vecBoneMatrix[m_BoneSocket[i]->m_BoneIndex]);
	}
}

CSkeleton* CSkeleton::Clone()
{
	return new CSkeleton(*this);
}

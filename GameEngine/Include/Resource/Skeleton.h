#pragma once

#include "../Ref.h"

struct Bone
{
	std::string		strName;
	int				iDepth;
	int				iParentIndex;
	Matrix		matOffset;
	Matrix		matBone;
	int				iRefCount;

	Bone() :
		iRefCount(1)
	{
	}
};

class CSkeleton :
    public CRef
{
	friend class CResourceManager;
	friend class CAnimation;
	friend class CAnimationSequence;
	friend class CAnimationMesh;

private:
	CSkeleton();
	CSkeleton(const CSkeleton& skeleton);
	~CSkeleton();

private:
	std::vector<Bone*>						m_vecBones;
	class CStructuredBuffer* m_pOffsetMatrixBuffer;
	std::vector<class CBoneSocket*>	m_BoneSocket;

public:
	size_t GetBoneCount()	const;
	Bone* GetBone(int iIndex = 0)	const;
	Bone* GetBone(const std::string& strName)	const;
	int GetBoneIndex(const std::string& strName)	const;
	bool CheckBone(const std::string& Name)	const;
	const Matrix& GetBoneMatrix(const std::string& strName)	const;
	const Matrix& GetBoneMatrix(int iIndex)	const;
	class CBoneSocket* GetSocket(const std::string& Name);

public:
	void AddBone(Bone* pBone);
	bool SaveSkeleton(const char* pFileName, const std::string& strPathName = MESH_PATH);
	bool SaveSkeletonFullPath(const char* pFullPath);
	bool LoadSkeleton(class CScene* pScene, const std::string& strName, const char* pFileName, const std::string& strPathName = MESH_PATH);
	bool LoadSkeletonFullPath(class CScene* pScene, const std::string& strName, const char* pFullPath);
	void SetShader();
	void ResetShader();
	void AddSocket(const std::string& BoneName, const std::string& SocketName,
		const Vector3& Offset = Vector3::Zero, const Vector3& OffsetRot = Vector3::Zero);
	void Update(float fTime, const std::vector<Matrix>& vecBoneMatrix, const Matrix& matWorld);
	CSkeleton* Clone();
};


#include "AnimationMesh.h"
#include "FBXLoader.h"
#include "Skeleton.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CAnimationMesh::CAnimationMesh()
{
	m_MeshType = Mesh_Type::AnimationMesh;
}

CAnimationMesh::~CAnimationMesh()
{
}

bool CAnimationMesh::LoadMeshFullPathMultibyte(const char* FullPath)
{
    char    Ext[_MAX_EXT] = {};

    _splitpath_s(FullPath, 0, 0, 0, 0, 0, 0, Ext, _MAX_EXT);
    _strupr_s(Ext);

    if (strcmp(Ext, ".FBX") == 0)
    {
        CFBXLoader  Loader;

        if (!Loader.LoadFBX(FullPath, false))
            return false;

        return ConvertFBX(&Loader, FullPath);
    }

    return LoadMeshFile(FullPath);
}

void CAnimationMesh::SetSkeleton(CSkeleton* Skeleton)
{
	m_Skeleton = Skeleton;
}

void CAnimationMesh::SetSkeleton(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Skeleton = new CSkeleton;

	char	FileNameMultibyte[MAX_PATH] = {};

	int Length = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FileName, -1, FileNameMultibyte, Length, 0, 0);

	m_Skeleton->LoadSkeleton(m_pScene, Name, FileNameMultibyte, PathName);

	m_Skeleton->Release();
}

void CAnimationMesh::Render()
{
    CMesh::Render();
}

void CAnimationMesh::Render(int MaterialSlotIndex)
{
    CMesh::Render(MaterialSlotIndex);
}

void CAnimationMesh::RenderInstancing(unsigned int Count)
{
    CMesh::RenderInstancing(Count);
}

void CAnimationMesh::RenderInstancing(int MaterialSlotIndex, unsigned int Count)
{
    CMesh::RenderInstancing(MaterialSlotIndex, Count);
}

bool CAnimationMesh::ConvertFBX(CFBXLoader* Loader, const char* FullPath)
{
    if (!CMesh::ConvertFBX(Loader, FullPath))
        return false;

    // �ִϸ��̼� ó��
	const std::vector<PFBXBONE>* pvecBone = Loader->GetBones();

	if (pvecBone->empty())
		return true;

	std::string	strSkeletonName = m_Name + "_Skeleton";

	m_Skeleton = new CSkeleton;

	// �� ����ŭ �ݺ��Ѵ�.
	std::vector<PFBXBONE>::const_iterator	iterB;
	std::vector<PFBXBONE>::const_iterator	iterBEnd = pvecBone->end();

	for (iterB = pvecBone->begin(); iterB != iterBEnd; ++iterB)
	{
		Bone*	pBone = new Bone;

		pBone->strName = (*iterB)->strName;
		pBone->iDepth = (*iterB)->iDepth;
		pBone->iParentIndex = (*iterB)->iParentIndex;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				pBone->matOffset[i][j] = (float)(*iterB)->matOffset.mData[i].mData[j];
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				pBone->matBone[i][j] = (float)(*iterB)->matBone.mData[i].mData[j];
			}
		}

		m_Skeleton->AddBone(pBone);
	}

	//pScene->GetResourceManager()->SetSkeleton(strSkeletonName, pSkeleton);

	// �ִϸ��̼� Ŭ���� �߰��Ѵ�.
	const std::vector<PFBXANIMATIONCLIP>* pvecClip = Loader->GetClips();

	// ���� �̸��� ������ ��θ� �����д�.
	char	strAnimPath[MAX_PATH] = {};
	strcpy_s(strAnimPath, FullPath);

	int	iLength = (int)strlen(strAnimPath);
	for (int i = iLength - 1; i >= 0; --i)
	{
		// aa/bb.exe 9��, 2���ε��� 3 ~ 8������ ����
		if (strAnimPath[i] == '/' || strAnimPath[i] == '\\')
		{
			memset(&strAnimPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
			break;
		}
	}

	// Ŭ���� �о�´�.
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterC;
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterCEnd = pvecClip->end();

	for (iterC = pvecClip->begin(); iterC != iterCEnd; ++iterC)
	{
		m_pScene->GetResource()->LoadAnimationSequence((*iterC)->strName, false, *iterC);

		CAnimationSequence* pSequence = m_pScene->GetResource()->FindAnimationSequence((*iterC)->strName);

		if (!pSequence)
			continue;

		char	strAnimFullPath[MAX_PATH] = {};
		strcpy_s(strAnimFullPath, strAnimPath);
		strcat_s(strAnimFullPath, (*iterC)->strName.c_str());
		strcat_s(strAnimFullPath, ".sqc");

		pSequence->SaveFullPathMultibyte(strAnimFullPath);
	}

	if (m_Skeleton)
	{
		char	SkeletonPath[MAX_PATH] = {};
		strcpy_s(SkeletonPath, FullPath);
		memcpy(&SkeletonPath[iLength - 3], "bne", 3);
		m_Skeleton->SaveSkeletonFullPath(SkeletonPath);

		m_Skeleton->Release();
	}

    return true;
}

bool CAnimationMesh::SaveMesh(FILE* File)
{
    if (!CMesh::SaveMesh(File))
        return false;

    return true;
}

bool CAnimationMesh::LoadMesh(FILE* File)
{
    if (!CMesh::LoadMesh(File))
        return false;

    return true;
}

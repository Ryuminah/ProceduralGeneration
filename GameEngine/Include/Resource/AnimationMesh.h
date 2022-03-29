#pragma once
#include "Mesh.h"
class CAnimationMesh :
    public CMesh
{
    friend class CResourceManager;
    friend class CSceneResource;

protected:
    CAnimationMesh();
    virtual ~CAnimationMesh();

private:
    CSharedPtr<class CSkeleton> m_Skeleton;

public:
    class CSkeleton* GetSkeleton()  const
    {
        return m_Skeleton;
    }

public:
    virtual bool LoadMeshFullPathMultibyte(const char* FullPath);
    void SetSkeleton(class CSkeleton* Skeleton);
    void SetSkeleton(const std::string& Name, const TCHAR* FileName, const std::string& PathName = MESH_PATH);
    virtual void Render();
    virtual void Render(int MaterialSlotIndex);
    virtual void RenderInstancing(unsigned int Count);
    virtual void RenderInstancing(int MaterialSlotIndex, unsigned int Count);

private:
    bool ConvertFBX(class CFBXLoader* Loader, const char* FullPath);

protected:
    virtual bool SaveMesh(FILE* File);
    virtual bool LoadMesh(FILE* File);
};


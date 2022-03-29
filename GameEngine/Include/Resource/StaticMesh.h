#pragma once
#include "Mesh.h"
class CStaticMesh :
    public CMesh
{
    friend class CResourceManager;
    friend class CSceneResource;

protected:
    CStaticMesh();
    virtual ~CStaticMesh();

public:
    virtual bool LoadMeshFullPathMultibyte(const char* FullPath);
    virtual void Render();
    virtual void Render(int MaterialSlotIndex);
    virtual void RenderInstancing(unsigned int Count);
    virtual void RenderInstancing(int MaterialSlotIndex, unsigned int Count);

private:
    virtual bool ConvertFBX(class CFBXLoader* Loader, const char* FullPath);

protected:
    virtual bool SaveMesh(FILE* File);
    virtual bool LoadMesh(FILE* File);
};


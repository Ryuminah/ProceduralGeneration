#include "StaticMesh.h"
#include "FBXLoader.h"

CStaticMesh::CStaticMesh()
{
    m_MeshType = Mesh_Type::StaticMesh;
}

CStaticMesh::~CStaticMesh()
{
}

bool CStaticMesh::LoadMeshFullPathMultibyte(const char* FullPath)
{
    char    Ext[_MAX_EXT] = {};

    _splitpath_s(FullPath, 0, 0, 0, 0, 0, 0, Ext, _MAX_EXT);
    _strupr_s(Ext);

    if (strcmp(Ext, ".FBX") == 0)
    {
        CFBXLoader  Loader;

        if (!Loader.LoadFBX(FullPath))
            return false;

        return ConvertFBX(&Loader, FullPath);
    }

    return LoadMeshFile(FullPath);
}

void CStaticMesh::Render()
{
    CMesh::Render();
}

void CStaticMesh::Render(int MaterialSlotIndex)
{
    CMesh::Render(MaterialSlotIndex);
}

void CStaticMesh::RenderInstancing(unsigned int Count)
{
    CMesh::RenderInstancing(Count);
}

void CStaticMesh::RenderInstancing(int MaterialSlotIndex, unsigned int Count)
{
    CMesh::RenderInstancing(MaterialSlotIndex, Count);
}

bool CStaticMesh::ConvertFBX(CFBXLoader* Loader, const char* FullPath)
{
    return CMesh::ConvertFBX(Loader, FullPath);
}

bool CStaticMesh::SaveMesh(FILE* File)
{
    if (!CMesh::SaveMesh(File))
        return false;

    return true;
}

bool CStaticMesh::LoadMesh(FILE* File)
{
    if (!CMesh::LoadMesh(File))
        return false;

    return true;
}

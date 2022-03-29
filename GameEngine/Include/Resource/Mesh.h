#pragma once

#include "../Ref.h"
#include "Material.h"

struct MeshContainer
{
    VertexBuffer    VB;
    D3D11_PRIMITIVE_TOPOLOGY    Primitive;
    std::vector<IndexBuffer*>    vecIB;
};

struct MeshSlot
{
    VertexBuffer*    VB;
    D3D11_PRIMITIVE_TOPOLOGY    Primitive;
    IndexBuffer*    IB;

    MeshSlot()  :
        VB(nullptr),
        IB(nullptr)
    {
    }
};

class CMesh :
    public CRef
{
    friend class CResourceManager;
    friend class CSceneResource;

protected:
    CMesh();
    virtual ~CMesh() = 0;

protected:
    class CScene* m_pScene;
    std::vector<MeshContainer*>     m_vecMeshContainer;
    std::vector<MeshSlot*>          m_vecMeshSlot;
    std::vector<CSharedPtr<CMaterial>>   m_vecMaterialSlot;
    Vector3     m_Min;
    Vector3     m_Max;
    Mesh_Type   m_MeshType;

public:
    const std::vector<CSharedPtr<CMaterial>>* GetMaterialSlots()    const
    {
        return &m_vecMaterialSlot;
    }

    CMaterial* GetMaterialSlot(int SlotIndex) const
    {
        return m_vecMaterialSlot[SlotIndex].Get();
    }

    Mesh_Type GetMeshType() const
    {
        return m_MeshType;
    }

    Vector3 GetMin()    const
    {
        return m_Min;
    }

    Vector3 GetMax()    const
    {
        return m_Max;
    }

public:
    void AddMaterialSlot(CMaterial* pMaterial)
    {
        m_vecMaterialSlot.push_back(pMaterial);
    }

    void AddMaterialSlot(const std::string& Name);

public:
    virtual bool CreateMesh(void* pVertices, int VtxCount, int VtxSize, D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
        void* pIndices = nullptr, int IdxCount = 0, int IdxSize = 0, D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT,
        DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);
    bool LoadMesh(const TCHAR* Filename, const std::string& PathName = MESH_PATH);
    bool LoadMeshFullPath(const TCHAR* FullPath);
    bool LoadMeshMultibyte(const char* Filename, const std::string& PathName = MESH_PATH);
    virtual bool LoadMeshFullPathMultibyte(const char* FullPath);
    virtual void Render();
    virtual void Render(int MaterialSlotIndex);
    virtual void RenderInstancing(unsigned int Count);
    virtual void RenderInstancing(int MaterialSlotIndex, unsigned int Count);
    void Clear();

protected:
    bool CreateVertex(VertexBuffer* Buffer,
        void* Data, int Count, int Size, D3D11_USAGE Usage);
    bool CreateIndex(IndexBuffer* Buffer,
        void* Data, int Count, int Size, D3D11_USAGE Usage, DXGI_FORMAT Fmt);

public:
    bool SaveMeshFile(const char* FullPath);
    bool LoadMeshFile(const char* FullPath);

protected:
    virtual bool SaveMesh(FILE* File);
    virtual bool LoadMesh(FILE* File);

protected:
    virtual bool ConvertFBX(class CFBXLoader* Loader, const char* FullPath);
};


#pragma once

#include "PrimitiveComponent.h"
#include "../Resource/StaticMesh.h"
#include "../Resource/LandScapeCBuffer.h"

class CLandScapeComponent :
    public CPrimitiveComponent
{
    friend class CGameObject;
    friend class CNavigation3D;

protected:
    CLandScapeComponent();
    CLandScapeComponent(const CLandScapeComponent& com);
    virtual ~CLandScapeComponent();

protected:
    CSharedPtr<CStaticMesh>     m_pMesh;
    CLandScapeCBuffer* m_CBuffer;
    float       m_DetailLevel;
    int         m_CountX;
    int         m_CountZ;
    float       m_CellWidth;
    float       m_CellHeight;
    std::vector<Vertex3D>   m_vecVtx;
    std::vector<Vector3>    m_vecFaceNormal;
    std::vector<unsigned int>   m_vecIndex;
    int         m_SplatCount;

public:
    void SetSplatCount(int Count)
    {
        m_SplatCount = Count;
    }

public:
    virtual class CMesh* GetMesh()  const;
    virtual void SetMesh(class CMesh* pMesh);
    virtual void SetMesh(const std::string& Name);
    virtual void SetMaterial(int SlotIndex, CMaterial* pMaterial);
    virtual void SetMaterial(int SlotIndex, const std::string& Name);
    virtual void AddMaterial(CMaterial* pMaterial);
    virtual void AddMaterial(const std::string& Name);
    void CreateLandScape(const std::string& Name,
        int CountX, int CountZ, float CellWidth, float CellHeight,
        const TCHAR* HeightMap = nullptr, const std::string& PathName = TEXTURE_PATH);

private:
    void ComputeNormal();
    void ComputeTangent();

public:
    void SetDetailLevel(float Level);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual CLandScapeComponent* Clone();
    virtual void RenderShadow(float DeltaTime);
};


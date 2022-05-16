
#include "Mesh.h"
#include "../Device.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CMesh::CMesh()  :
    m_pScene(nullptr)
{
    m_Min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
    m_Max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

CMesh::~CMesh()
{
    auto    iter = m_vecMeshContainer.begin();
    auto    iterEnd = m_vecMeshContainer.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_RELEASE((*iter)->VB.pBuffer);
        SAFE_DELETE_ARRAY((*iter)->VB.pData);

        auto    iter1 = (*iter)->vecIB.begin();
        auto    iter1End = (*iter)->vecIB.end();

        for (; iter1 != iter1End; ++iter1)
        {
            SAFE_DELETE_ARRAY((*iter1)->pData);
            SAFE_RELEASE((*iter1)->pBuffer);
            SAFE_DELETE((*iter1));
        }

        SAFE_DELETE((*iter));
    }

    auto    iterSlot = m_vecMeshSlot.begin();
    auto    iterSlotEnd = m_vecMeshSlot.end();

    for (; iterSlot != iterSlotEnd; ++iterSlot)
    {
        SAFE_DELETE((*iterSlot));
    }

    m_vecMeshSlot.clear();
}

void CMesh::AddMaterialSlot(const std::string& Name)
{
    CMaterial* pMaterial = nullptr;

    if (m_pScene)
    {
        pMaterial = m_pScene->GetResource()->FindMaterial(Name);
    }

    else
    {
        pMaterial = CResourceManager::GetInst()->FindMaterial(Name);
    }

    if (pMaterial)
        m_vecMaterialSlot.push_back(pMaterial);
}

bool CMesh::CreateMesh(void* pVertices, int VtxCount, int VtxSize, D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
    void* pIndices, int IdxCount, int IdxSize, D3D11_USAGE IdxUsage, DXGI_FORMAT Fmt)
{
    MeshContainer* pContainer = new MeshContainer;

    m_vecMeshContainer.push_back(pContainer);

    pContainer->Primitive = Primitive;

    pContainer->VB.Count = VtxCount;
    pContainer->VB.Size = VtxSize;
    pContainer->VB.Usage = VtxUsage;
    pContainer->VB.pData = new char[VtxCount * VtxSize];
    memcpy(pContainer->VB.pData, pVertices, VtxCount * VtxSize);

    // 버텍스 버퍼를 생성한다.
    D3D11_BUFFER_DESC   VtxDesc = {};

    VtxDesc.Usage = VtxUsage;
    VtxDesc.ByteWidth = VtxSize * VtxCount;
    VtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    // 생성하는 버텍스 버퍼에 채워줄 정보를 지정한다.
    
    D3D11_SUBRESOURCE_DATA  VtxData = {};
    VtxData.pSysMem = pVertices;

    if (FAILED(DEVICE->CreateBuffer(&VtxDesc, &VtxData, &pContainer->VB.pBuffer)))
        return false;

    char* pVertexData = (char*)pVertices;

    for (int i = 0; i < VtxCount; ++i)
    {
        Vector3* pPos = (Vector3*)pVertexData;
        pVertexData += VtxSize;

        if (m_Min.x > pPos->x)
            m_Min.x = pPos->x;

        if (m_Min.y > pPos->y)
            m_Min.y = pPos->y;

        if (m_Min.z > pPos->z)
            m_Min.z = pPos->z;

        if (m_Max.x < pPos->x)
            m_Max.x = pPos->x;

        if (m_Max.y < pPos->y)
            m_Max.y = pPos->y;

        if (m_Max.z < pPos->z)
            m_Max.z = pPos->z;
    }

    if (pIndices)
    {
        IndexBuffer* IB = new IndexBuffer;
        pContainer->vecIB.push_back(IB);

        IB->Count = IdxCount;
        IB->Size = IdxSize;
        IB->Usage = IdxUsage;
        IB->Fmt = Fmt;
        IB->pData = new char[IdxCount * IdxSize];
        memcpy(IB->pData, pIndices, IdxCount * IdxSize);

        D3D11_BUFFER_DESC   IdxBufferDesc = {};

        IdxBufferDesc.Usage = IdxUsage;
        IdxBufferDesc.ByteWidth = IdxCount * IdxSize;
        IdxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA  IdxData = {};

        IdxData.pSysMem = pIndices;

        if (FAILED(DEVICE->CreateBuffer(&IdxBufferDesc, &IdxData, &IB->pBuffer)))
            return false;
    }

    MeshSlot* pSlot = new MeshSlot;

    pSlot->VB = pContainer->VB;

    if (!pContainer->vecIB.empty())
        pSlot->IB = pContainer->vecIB[0];

    pSlot->Primitive = pContainer->Primitive;

    m_vecMeshSlot.push_back(pSlot);


    return true;
}

void CMesh::Render()
{
    size_t  Size = m_vecMeshContainer.size();

    for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
    {
        unsigned int    Stride = m_vecMeshContainer[i]->VB.Size;
        unsigned int    Offset = 0;

        CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[i]->Primitive);
        CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[i]->VB.pBuffer, &Stride, &Offset);

        size_t  Size1 = m_vecMeshContainer[i]->vecIB.size();

        if (Size1 > 0)
        {
            for (size_t j = 0; j < Size1; ++j)
            {
                if (m_vecMeshContainer[i]->vecIB[j]->pBuffer)
                {
                    CONTEXT->IASetIndexBuffer(m_vecMeshContainer[i]->vecIB[j]->pBuffer, 
                        m_vecMeshContainer[i]->vecIB[j]->Fmt, 0);
                    CONTEXT->DrawIndexed(m_vecMeshContainer[i]->vecIB[j]->Count, 0, 0);
                }
            }
        }

        else
        {
            CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
            CONTEXT->Draw(m_vecMeshContainer[i]->VB.Count, 0);
        }
    }
}

void CMesh::Render(int MaterialSlotIndex)
{
    MeshSlot* pSlot = m_vecMeshSlot[MaterialSlotIndex];

    unsigned int    Stride = pSlot->VB.Size;
    unsigned int    Offset = 0;

    CONTEXT->IASetPrimitiveTopology(pSlot->Primitive);
    CONTEXT->IASetVertexBuffers(0, 1, &pSlot->VB.pBuffer, &Stride, &Offset);

    if (pSlot->IB)
    {
        CONTEXT->IASetIndexBuffer(pSlot->IB->pBuffer,
            pSlot->IB->Fmt, 0);
        CONTEXT->DrawIndexed(pSlot->IB->Count, 0, 0);
    }

    else
    {
        CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
        CONTEXT->Draw(pSlot->VB.Count, 0);
    }
}

void CMesh::RenderInstancing(unsigned int Count)
{
    size_t  Size = m_vecMeshContainer.size();

    for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
    {
        unsigned int    Stride = m_vecMeshContainer[i]->VB.Size;
        unsigned int    Offset = 0;

        CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[i]->Primitive);
        CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[i]->VB.pBuffer, &Stride, &Offset);

        size_t  Size1 = m_vecMeshContainer[i]->vecIB.size();

        if (Size1 > 0)
        {
            for (size_t j = 0; j < Size1; ++j)
            {
                if (m_vecMeshContainer[i]->vecIB[j]->pBuffer)
                {
                    CONTEXT->IASetIndexBuffer(m_vecMeshContainer[i]->vecIB[j]->pBuffer,
                        m_vecMeshContainer[i]->vecIB[j]->Fmt, 0);
                    //CONTEXT->DrawIndexed(m_vecMeshContainer[i]->vecIB[j]->Count, 0, 0);
                    CONTEXT->DrawIndexedInstanced(m_vecMeshContainer[i]->vecIB[j]->Count,
                        Count, 0, 0, 0);
                }
            }
        }

        else
        {
            CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
            //CONTEXT->Draw(m_vecMeshContainer[i]->VB.Count, 0);
            CONTEXT->DrawInstanced(m_vecMeshContainer[i]->VB.Count, Count, 0, 0);
        }
    }
}

void CMesh::RenderInstancing(int MaterialSlotIndex, unsigned int Count)
{
    MeshSlot* pSlot = m_vecMeshSlot[MaterialSlotIndex];

    unsigned int    Stride = pSlot->VB.Size;
    unsigned int    Offset = 0;

    CONTEXT->IASetPrimitiveTopology(pSlot->Primitive);
    CONTEXT->IASetVertexBuffers(0, 1, &pSlot->VB.pBuffer, &Stride, &Offset);

    if (pSlot->IB)
    {
        CONTEXT->IASetIndexBuffer(pSlot->IB->pBuffer,
            pSlot->IB->Fmt, 0);
        //CONTEXT->DrawIndexed(pSlot->IB->Count, 0, 0);
        CONTEXT->DrawIndexedInstanced(pSlot->IB->Count,
            Count, 0, 0, 0);
    }

    else
    {
        CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
        //CONTEXT->Draw(pSlot->VB.Count, 0);
        CONTEXT->DrawInstanced(pSlot->VB.Count, Count, 0, 0);
    }
}

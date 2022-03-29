
#include "Mesh.h"
#include "../Device.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../PathManager.h"
#include "FBXLoader.h"

CMesh::CMesh()  :
    m_pScene(nullptr)
{
    m_Min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
    m_Max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

CMesh::~CMesh()
{
    Clear();
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

    pSlot->VB = &pContainer->VB;

    if (!pContainer->vecIB.empty())
        pSlot->IB = pContainer->vecIB[0];

    pSlot->Primitive = pContainer->Primitive;

    m_vecMeshSlot.push_back(pSlot);


    return true;
}

bool CMesh::LoadMesh(const TCHAR* Filename, const std::string& PathName)
{
    TCHAR   FullPath[MAX_PATH] = {};

    const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

    if (Path)
        lstrcpy(FullPath, Path->pPath);

    lstrcat(FullPath, Filename);

    return LoadMeshFullPath(FullPath);
}

bool CMesh::LoadMeshFullPath(const TCHAR* FullPath)
{
    char    FullPathMultibyte[MAX_PATH] = {};

    int Length = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, 0, 0, 0, 0);
    WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, Length, 0, 0);

    return LoadMeshFullPathMultibyte(FullPathMultibyte);
}

bool CMesh::LoadMeshMultibyte(const char* Filename, const std::string& PathName)
{
    char    FullPath[MAX_PATH] = {};

    const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

    if (Path)
        strcpy_s(FullPath, Path->pPathMultibyte);

    strcat_s(FullPath, Filename);


    return LoadMeshFullPathMultibyte(FullPath);
}

bool CMesh::LoadMeshFullPathMultibyte(const char* FullPath)
{
    return false;
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

    unsigned int    Stride = pSlot->VB->Size;
    unsigned int    Offset = 0;

    CONTEXT->IASetPrimitiveTopology(pSlot->Primitive);
    CONTEXT->IASetVertexBuffers(0, 1, &pSlot->VB->pBuffer, &Stride, &Offset);

    if (pSlot->IB)
    {
        CONTEXT->IASetIndexBuffer(pSlot->IB->pBuffer,
            pSlot->IB->Fmt, 0);
        CONTEXT->DrawIndexed(pSlot->IB->Count, 0, 0);
    }

    else
    {
        CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
        CONTEXT->Draw(pSlot->VB->Count, 0);
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

    unsigned int    Stride = pSlot->VB->Size;
    unsigned int    Offset = 0;

    CONTEXT->IASetPrimitiveTopology(pSlot->Primitive);
    CONTEXT->IASetVertexBuffers(0, 1, &pSlot->VB->pBuffer, &Stride, &Offset);

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
        CONTEXT->DrawInstanced(pSlot->VB->Count, Count, 0, 0);
    }
}

void CMesh::Clear()
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

bool CMesh::CreateVertex(VertexBuffer* Buffer, void* Data, int Count, int Size, D3D11_USAGE Usage)
{
    Buffer->Count = Count;
    Buffer->Size = Size;
    Buffer->Usage = Usage;
    Buffer->pData = new char[Count * Size];
    memcpy(Buffer->pData, Data, Count * Size);

    // 버텍스 버퍼를 생성한다.
    D3D11_BUFFER_DESC   VtxDesc = {};

    VtxDesc.Usage = Usage;
    VtxDesc.ByteWidth = Count * Size;
    VtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    // 생성하는 버텍스 버퍼에 채워줄 정보를 지정한다.

    D3D11_SUBRESOURCE_DATA  VtxData = {};
    VtxData.pSysMem = Data;

    if (FAILED(DEVICE->CreateBuffer(&VtxDesc, &VtxData, &Buffer->pBuffer)))
        return false;

    char* pVertexData = (char*)Data;

    for (int i = 0; i < Count; ++i)
    {
        Vector3* pPos = (Vector3*)pVertexData;
        pVertexData += Size;

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

    return true;
}

bool CMesh::CreateIndex(IndexBuffer* Buffer, void* Data, int Count, int Size, D3D11_USAGE Usage, 
    DXGI_FORMAT Fmt)
{
    Buffer->Count = Count;
    Buffer->Size = Size;
    Buffer->Usage = Usage;
    Buffer->Fmt = Fmt;
    Buffer->pData = new char[Count * Size];
    memcpy(Buffer->pData, Data, Count * Size);

    D3D11_BUFFER_DESC   IdxBufferDesc = {};

    IdxBufferDesc.Usage = Usage;
    IdxBufferDesc.ByteWidth = Count * Size;
    IdxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA  IdxData = {};

    IdxData.pSysMem = Data;

    if (FAILED(DEVICE->CreateBuffer(&IdxBufferDesc, &IdxData, &Buffer->pBuffer)))
        return false;

    return true;
}

bool CMesh::SaveMeshFile(const char* FullPath)
{
    FILE* File = nullptr;

    fopen_s(&File, FullPath, "wb");

    if (!File)
        return false;

    SaveMesh(File);

    fclose(File);

    return true;
}

bool CMesh::LoadMeshFile(const char* FullPath)
{
    FILE* File = nullptr;

    fopen_s(&File, FullPath, "rb");

    if (!File)
        return false;

    LoadMesh(File);

    fclose(File);

    return true;
}

bool CMesh::SaveMesh(FILE* File)
{
    fwrite(&m_MeshType, sizeof(Mesh_Type), 1, File);

    int Length = (int)m_Name.length();

    fwrite(&Length, sizeof(int), 1, File);
    fwrite(m_Name.c_str(), sizeof(char), Length, File);

    fwrite(&m_Min, sizeof(Vector3), 1, File);
    fwrite(&m_Max, sizeof(Vector3), 1, File);

    int ContainerCount = (int)m_vecMeshContainer.size();

    fwrite(&ContainerCount, sizeof(int), 1, File);

    for (int i = 0; i < ContainerCount; ++i)
    {
        MeshContainer* Container = m_vecMeshContainer[i];

        fwrite(&Container->Primitive, sizeof(D3D11_PRIMITIVE_TOPOLOGY), 1, File);

        fwrite(&Container->VB.Size, sizeof(int), 1, File);
        fwrite(&Container->VB.Count, sizeof(int), 1, File);
        fwrite(&Container->VB.Usage, sizeof(D3D11_USAGE), 1, File);
        fwrite(Container->VB.pData, Container->VB.Size, Container->VB.Count, File);

        int IndexCount = (int)Container->vecIB.size();

        fwrite(&IndexCount, sizeof(int), 1, File);

        for (int j = 0; j < IndexCount; ++j)
        {
            fwrite(&Container->vecIB[j]->Size, sizeof(int), 1, File);
            fwrite(&Container->vecIB[j]->Count, sizeof(int), 1, File);
            fwrite(&Container->vecIB[j]->Usage, sizeof(D3D11_USAGE), 1, File);
            fwrite(&Container->vecIB[j]->Fmt, sizeof(DXGI_FORMAT), 1, File);
            fwrite(Container->vecIB[j]->pData, Container->vecIB[j]->Size, Container->vecIB[j]->Count, File);
        }
    }

    int MaterialCount = (int)m_vecMaterialSlot.size();

    fwrite(&MaterialCount, sizeof(int), 1, File);

    for (int i = 0; i < MaterialCount; ++i)
    {
        m_vecMaterialSlot[i]->Save(File);
    }

    return true;
}

bool CMesh::LoadMesh(FILE* File)
{
    Clear();

    fread(&m_MeshType, sizeof(Mesh_Type), 1, File);

    int Length = 0;
    char    Name[256] = {};

    fread(&Length, sizeof(int), 1, File);
    fread(Name, sizeof(char), Length, File);
    m_Name = Name;

    fread(&m_Min, sizeof(Vector3), 1, File);
    fread(&m_Max, sizeof(Vector3), 1, File);

    int ContainerCount = 0;

    fread(&ContainerCount, sizeof(int), 1, File);

    for (int i = 0; i < ContainerCount; ++i)
    {
        MeshContainer* Container = new MeshContainer;

        m_vecMeshContainer.push_back(Container);

        fread(&Container->Primitive, sizeof(D3D11_PRIMITIVE_TOPOLOGY), 1, File);

        int VtxSize = 0, VtxCount = 0;
        D3D11_USAGE VtxUsage;

        fread(&VtxSize, sizeof(int), 1, File);
        fread(&VtxCount, sizeof(int), 1, File);
        fread(&VtxUsage, sizeof(D3D11_USAGE), 1, File);

        void* VtxData = new char[VtxSize * VtxCount];

        fread(VtxData, VtxSize, VtxCount, File);

        if (!CreateVertex(&Container->VB, VtxData, VtxCount, VtxSize, VtxUsage))
        {
            fclose(File);
            return false;
        }

        SAFE_DELETE_ARRAY(VtxData);

        int IndexCount = 0;

        fread(&IndexCount, sizeof(int), 1, File);

        for (int j = 0; j < IndexCount; ++j)
        {
            IndexBuffer* IdxBuffer = new IndexBuffer;

            Container->vecIB.push_back(IdxBuffer);

            int IdxSize = 0, IdxCount = 0;
            D3D11_USAGE IdxUsage;
            DXGI_FORMAT Fmt;

            fread(&IdxSize, sizeof(int), 1, File);
            fread(&IdxCount, sizeof(int), 1, File);
            fread(&IdxUsage, sizeof(D3D11_USAGE), 1, File);
            fread(&Fmt, sizeof(DXGI_FORMAT), 1, File);

            void* IdxData = new char[IdxSize * IdxCount];

            fread(IdxData, IdxSize, IdxCount, File);

            if (!CreateIndex(IdxBuffer, IdxData, IdxCount, IdxSize, IdxUsage, Fmt))
            {
                fclose(File);
                return false;
            }

            SAFE_DELETE_ARRAY(IdxData);

            MeshSlot* Slot = new MeshSlot;

            m_vecMeshSlot.push_back(Slot);

            Slot->VB = &Container->VB;
            Slot->IB = IdxBuffer;
            Slot->Primitive = Container->Primitive;
        }
    }

    int MaterialCount = (int)m_vecMaterialSlot.size();

    fread(&MaterialCount, sizeof(int), 1, File);

    for (int i = 0; i < MaterialCount; ++i)
    {
        CMaterial* Material = new CMaterial;

        Material->Init();

        Material->Load(File);

        m_vecMaterialSlot.push_back(Material);

        SAFE_RELEASE(Material);
    }

    return true;
}

bool CMesh::ConvertFBX(CFBXLoader* Loader, const char* FullPath)
{
    const std::vector<std::vector<PFBXMATERIAL>>* vecMaterials = Loader->GetMaterials();
    const std::vector<PFBXMESHCONTAINER>* vecContainers = Loader->GetMeshContainers();

    auto    iter = vecContainers->begin();
    auto    iterEnd = vecContainers->end();

    bool    BumpEnable = false;
    bool    AnimationEnable = false;

    std::vector<std::vector<bool>>  vecEmptyIndex;

    int ContainerIndex = 0;

    for (; iter != iterEnd; ++iter, ++ContainerIndex)
    {
        MeshContainer* Container = new MeshContainer;

        m_vecMeshContainer.push_back(Container);

        std::vector<bool>    vecEmpty;
        vecEmptyIndex.push_back(vecEmpty);

        if ((*iter)->Bump)
            BumpEnable = true;

        if ((*iter)->Animation)
            AnimationEnable = true;

        std::vector<Vertex3D>   vecVtx;

        size_t  VtxCount = (*iter)->vecPos.size();

        vecVtx.resize(VtxCount);

        for (size_t i = 0; i < VtxCount; ++i)
        {
            Vertex3D    Vtx = {};

            Vtx.Pos = (*iter)->vecPos[i];
            Vtx.Normal = (*iter)->vecNormal[i];
            Vtx.UV = (*iter)->vecUV[i];

            if (BumpEnable)
            {
                if (!(*iter)->vecTangent.empty())
                    Vtx.Tangent = (*iter)->vecTangent[i];

                if (!(*iter)->vecBinormal.empty())
                    Vtx.Binormal = (*iter)->vecBinormal[i];
            }

            if (!(*iter)->vecBlendWeight.empty() && AnimationEnable)
            {
                Vtx.BlendWeight = (*iter)->vecBlendWeight[i];
                Vtx.BlendIndex = (*iter)->vecBlendIndex[i];
            }

            vecVtx[i] = Vtx;
        }

        // Mesh의 Vertex 생성
        if (!CreateVertex(&Container->VB, &vecVtx[0], (int)vecVtx.size(), sizeof(Vertex3D), D3D11_USAGE_DEFAULT))
            return false;

        Container->Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

        size_t  IdxCount = (*iter)->vecIndices.size();

        for (size_t i = 0; i < IdxCount; ++i)
        {
            // 인덱스 버퍼를 채워줄 데이터가 없다면 해당 서브셋은 폐기한다.
            if ((*iter)->vecIndices[i].empty())
            {
                vecEmptyIndex[ContainerIndex].push_back(false);
                continue;
            }

            vecEmptyIndex[ContainerIndex].push_back(true);

            MeshSlot* Slot = new MeshSlot;

            m_vecMeshSlot.push_back(Slot);

            Slot->VB = &Container->VB;
            Slot->Primitive = Container->Primitive;

            IndexBuffer* IdxBuffer = new IndexBuffer;

            Container->vecIB.push_back(IdxBuffer);

            if (!CreateIndex(IdxBuffer, &(*iter)->vecIndices[i][0], (int)(*iter)->vecIndices[i].size(),
                sizeof(UINT), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT))
                return false;

            Slot->IB = IdxBuffer;
        }
    }


    // 재질 정보를 읽어온다.
    auto    iterM = vecMaterials->begin();
    auto    iterMEnd = vecMaterials->end();

    ContainerIndex = 0;

    for (; iterM != iterMEnd; ++iterM, ++ContainerIndex)
    {
        // 서브셋 수만큼 반복한다.
        size_t  Size = (*iterM).size();

        for (size_t i = 0; i < Size; ++i)
        {
            if (!vecEmptyIndex[ContainerIndex][i])
                continue;

            PFBXMATERIAL    Mtrl = (*iterM)[i];

            CMaterial* Material = new CMaterial;

            Material->Init();

            Material->SetBaseColor(Mtrl->Dif);
            Material->SetAmbientColor(Mtrl->Amb);
            Material->SetSpecularColor(Mtrl->Spc);
            Material->SetEmissiveColor(Mtrl->Emv);
            Material->SetSpecularPower(Mtrl->Shininess);

            Material->SetShader("Standard3DShader");

            AddMaterialSlot(Material);

            // Texture
            char    FileName[MAX_PATH] = {};
            _splitpath_s(Mtrl->DifTex.c_str(), 0, 0, 0, 0, FileName, MAX_PATH, 0, 0);

            TCHAR   FullPath[MAX_PATH] = {};

#ifdef UNICODE
            int PathLength = MultiByteToWideChar(CP_ACP, 0, Mtrl->DifTex.c_str(),
                -1, 0, 0);
            MultiByteToWideChar(CP_ACP, 0, Mtrl->DifTex.c_str(), -1, FullPath, PathLength);
#else
            strcpy_s(FullPath, Mtrl->DifTex.c_str());
#endif // UNICODE

            Material->AddTextureFullPath(FileName, FullPath);
            Material->SetTextureLink(FileName, Texture_Link::BaseTexture);
            Material->SetTextureShaderType(FileName, TST_PIXEL);

            if (!Mtrl->BumpTex.empty())
            {
                memset(FileName, 0, MAX_PATH);
                _splitpath_s(Mtrl->BumpTex.c_str(), 0, 0, 0, 0, FileName, MAX_PATH, 0, 0);

                memset(FullPath, 0, sizeof(TCHAR) * MAX_PATH);

#ifdef UNICODE
                int PathLength = MultiByteToWideChar(CP_ACP, 0, Mtrl->BumpTex.c_str(),
                    -1, 0, 0);
                MultiByteToWideChar(CP_ACP, 0, Mtrl->BumpTex.c_str(), -1, FullPath, PathLength);
#else
                strcpy_s(FullPath, Mtrl->BumpTex.c_str());
#endif // UNICODE

                Material->AddTextureFullPath(FileName, FullPath);
                Material->SetTextureLink(FileName, Texture_Link::NormalTexture);
                Material->SetTextureShaderType(FileName, TST_PIXEL);
            }

            if (!Mtrl->SpcTex.empty())
            {
                memset(FileName, 0, MAX_PATH);
                _splitpath_s(Mtrl->SpcTex.c_str(), 0, 0, 0, 0, FileName, MAX_PATH, 0, 0);

                memset(FullPath, 0, sizeof(TCHAR) * MAX_PATH);

#ifdef UNICODE
                int PathLength = MultiByteToWideChar(CP_ACP, 0, Mtrl->SpcTex.c_str(),
                    -1, 0, 0);
                MultiByteToWideChar(CP_ACP, 0, Mtrl->SpcTex.c_str(), -1, FullPath, PathLength);
#else
                strcpy_s(FullPath, Mtrl->SpcTex.c_str());
#endif // UNICODE

                Material->AddTextureFullPath(FileName, FullPath);
                Material->SetTextureLink(FileName, Texture_Link::SpecularTexture);
                Material->SetTextureShaderType(FileName, TST_PIXEL);
            }

            if (BumpEnable)
                Material->EnableBump();

            if (AnimationEnable)
                Material->EnableAnimation3D();

            SAFE_RELEASE(Material);
        }
    }


    // 자체포맷으로 저장해준다.
    char    MeshFullPath[MAX_PATH] = {};

    strcpy_s(MeshFullPath, FullPath);
    int PathLength = (int)strlen(FullPath);
    memcpy(&MeshFullPath[PathLength - 3], "msh", 3);

    SaveMeshFile(MeshFullPath);

    return true;
}

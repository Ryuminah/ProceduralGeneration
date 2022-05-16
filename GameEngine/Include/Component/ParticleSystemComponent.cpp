
#include "ParticleSystemComponent.h"
#include "../Render/RenderManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/StructuredBuffer.h"
#include "../Resource/ParticleConstantBuffer.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"
#include "Transform.h"

CParticleSystemComponent::CParticleSystemComponent()    :
    m_CBuffer(nullptr),
    //m_CopyBuffer(nullptr),
    m_ParticleInfo{},
    m_ParticleInfoShare{},
    m_SpawnTime(0.f),
    m_SpawnTimeMax(0.01f)
{
    m_SceneComponentType = SceneComponent_Type::Primitive;
    m_PrimitiveType = PrimitiveComponent_Type::Primitive2D;

    m_2DType = RT2D_Particle;

    m_pTransform->SetDefaultZ(0.f);
}

CParticleSystemComponent::CParticleSystemComponent(const CParticleSystemComponent& com) :
    CPrimitiveComponent(com)
{
    m_SpawnTime = 0.f;

    m_Particle = com.m_Particle;

    auto    iter = m_vecBuffer.begin();
    auto    iterEnd = m_vecBuffer.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE((*iter));
    }

    m_Particle->CloneStructuredBuffer(m_vecBuffer);
    m_UpdateShader = m_Particle->GetUpdateShader();
    SAFE_DELETE(m_CBuffer);
    m_CBuffer = m_Particle->CloneConstantBuffer();

    m_SpawnTimeMax = m_Particle->GetSpawnTime();
}

CParticleSystemComponent::~CParticleSystemComponent()
{
    auto    iter = m_vecBuffer.begin();
    auto    iterEnd = m_vecBuffer.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE((*iter));
    }
    SAFE_DELETE(m_CBuffer);

    //SAFE_DELETE(m_CopyBuffer);
}

void CParticleSystemComponent::SetParticle(const std::string& Name)
{
    CParticleSystem* Particle = m_pScene->GetResource()->FindParticle(Name);

    SetParticle(Particle);
}

void CParticleSystemComponent::SetParticle(CParticleSystem* Particle)
{
    m_Particle = Particle;

    CMaterial* pCloneMaterial = m_Particle->CloneMaterial();

    pCloneMaterial->SetScene(m_pScene);
    m_vecMaterialSlot.push_back(pCloneMaterial);
    pCloneMaterial->Release();

    auto    iter = m_vecBuffer.begin();
    auto    iterEnd = m_vecBuffer.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE((*iter));
    }

    m_Particle->CloneStructuredBuffer(m_vecBuffer);
    m_UpdateShader = m_Particle->GetUpdateShader();
    SAFE_DELETE(m_CBuffer);
    m_CBuffer = m_Particle->CloneConstantBuffer();

    m_CBuffer->SetDefaultZ(m_pTransform->GetDefaultZ());

    m_SpawnTimeMax = m_Particle->GetSpawnTime();
}

void CParticleSystemComponent::SetSpawnTime(float Time)
{
    m_SpawnTimeMax = Time;
}

void CParticleSystemComponent::Start()
{
    CPrimitiveComponent::Start();

    /*m_CopyBuffer = new CStructuredBuffer;

    m_CopyBuffer->InitRead("Copy", sizeof(ParticleInfo),
        m_CBuffer->GetSpawnCountMax(), 0);*/
}

bool CParticleSystemComponent::Init()
{
    if (!CPrimitiveComponent::Init())
        return false;

    m_Mesh = CResourceManager::GetInst()->FindMesh("ParticlePoint");
    m_RenderShader = CShaderManager::GetInst()->FindShader("ParticleShader");

    return true;
}

void CParticleSystemComponent::Update(float DeltaTime)
{
    CPrimitiveComponent::Update(DeltaTime);

    m_SpawnTime += DeltaTime;

    if (m_SpawnTime >= m_SpawnTimeMax)
    {
        m_SpawnTime -= m_SpawnTimeMax;

        m_CBuffer->SetSpawnCount(1);
    }

    else
        m_CBuffer->SetSpawnCount(0);
}

void CParticleSystemComponent::PostUpdate(float DeltaTime)
{
    CPrimitiveComponent::PostUpdate(DeltaTime);

    m_CBuffer->SetPos(GetWorldPos());

    m_CBuffer->UpdateCBuffer();

    size_t  Size = m_vecBuffer.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecBuffer[i]->SetShader((int)i);
    }

    // ParticleUpdateShader에서 Thread를 64, 1, 1 을 쓰고 있다.
    // 즉, X그룹은 64개당 그룹 1개가 필요하므로 현재 생성된 파티클수 / 64를
    // 이용해서 그룹 번호를 구하고 + 1을 해서 그룹 개수를 구한다.
    int GroupCount = (m_CBuffer->GetSpawnCountMax() / 64) + 1;
    m_UpdateShader->Excute(GroupCount, 1, 1);

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecBuffer[i]->ResetShader((int)i);
    }
}

void CParticleSystemComponent::Collision(float DeltaTime)
{
    CPrimitiveComponent::Collision(DeltaTime);
}

void CParticleSystemComponent::PrevRender(float DeltaTime)
{
    CPrimitiveComponent::PrevRender(DeltaTime);
}

void CParticleSystemComponent::Render(float DeltaTime)
{
    CPrimitiveComponent::Render(DeltaTime);

    //m_vecBuffer[0]->CopyResource(m_CopyBuffer);

    //m_CopyBuffer->CopyData(m_Info);

    size_t  Size = m_vecBuffer.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecBuffer[i]->SetShader((int)i + 30, CBT_GEOMETRY);
    }

    if (!m_vecMaterialSlot.empty())
        m_vecMaterialSlot[0]->SetMaterialEmptyShader();

    m_RenderShader->SetShader();

    m_Mesh->RenderInstancing(m_CBuffer->GetSpawnCountMax());

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecBuffer[i]->ResetShader((int)i + 30, CBT_GEOMETRY);
    }

    if (!m_vecMaterialSlot.empty())
        m_vecMaterialSlot[0]->ResetMaterialEmptyShader();
}

CParticleSystemComponent* CParticleSystemComponent::Clone()
{
    return new CParticleSystemComponent(*this);
}

void CParticleSystemComponent::SetStartColor(const Vector4& Color)
{
    m_CBuffer->SetStartColor(Color);
}

void CParticleSystemComponent::SetStartColor(float r, float g, float b, float a)
{
    m_CBuffer->SetStartColor(r, g, b, a);
}

void CParticleSystemComponent::SetEndColor(const Vector4& Color)
{
    m_CBuffer->SetEndColor(Color);
}

void CParticleSystemComponent::SetEndColor(float r, float g, float b, float a)
{
    m_CBuffer->SetEndColor(r, g, b, a);
}

void CParticleSystemComponent::SetStartScale(const Vector3& Scale)
{
    m_CBuffer->SetStartScale(Scale);
}

void CParticleSystemComponent::SetStartScale(float x, float y, float z)
{
    m_CBuffer->SetStartScale(x, y, z);
}

void CParticleSystemComponent::SetEndScale(const Vector3& Scale)
{
    m_CBuffer->SetEndScale(Scale);
}

void CParticleSystemComponent::SetEndScale(float x, float y, float z)
{
    m_CBuffer->SetEndScale(x, y, z);
}

void CParticleSystemComponent::SetLifeTimeMin(float Min)
{
    m_CBuffer->SetLifeTimeMin(Min);
}

void CParticleSystemComponent::SetLifeTimeMax(float Max)
{
    m_CBuffer->SetLifeTimeMax(Max);
}

void CParticleSystemComponent::SetRange(const Vector3& Range)
{
    m_CBuffer->SetRange(Range);
}

void CParticleSystemComponent::SetRange(float x, float y, float z)
{
    m_CBuffer->SetRange(x, y, z);
}

void CParticleSystemComponent::SetMinSpeed(float Min)
{
    m_CBuffer->SetMinSpeed(Min);
}

void CParticleSystemComponent::SetMaxSpeed(float Max)
{
    m_CBuffer->SetMaxSpeed(Max);
}

void CParticleSystemComponent::SetMoveEnable(bool Move)
{
    m_CBuffer->SetMoveEnable(Move);
}

void CParticleSystemComponent::SetParticle2D(bool b2D)
{
    m_CBuffer->SetParticle2D(b2D);
}

void CParticleSystemComponent::SetMoveDir(const Vector3& Dir)
{
    m_CBuffer->SetMoveDir(Dir);
}

void CParticleSystemComponent::SetMoveDir(float x, float y, float z)
{
    m_CBuffer->SetMoveDir(x, y, z);
}

void CParticleSystemComponent::SetMoveAngle(const Vector3& Angle)
{
    m_CBuffer->SetMoveAngle(Angle);
}

void CParticleSystemComponent::SetMoveAngle(float x, float y, float z)
{
    m_CBuffer->SetMoveAngle(x, y, z);
}

void CParticleSystemComponent::SetGravityEnable(bool Enable)
{
    m_CBuffer->SetGravityEnable(Enable);
}

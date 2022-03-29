
#include "Transform.h"
#include "../Resource/TransformConstantBuffer.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "Camera.h"
#include "../Resource/BoneSocket.h"
#include "../Navigation3D.h"
#include "../NavigationManager.h"

CTransform::CTransform()    :
    m_Animation2DEnable(false),
    m_pParent(nullptr),
    m_InheritScale(false),
    m_InheritRotX(false),
    m_InheritRotY(false),
    m_InheritRotZ(false),
    m_InheritPosZ(true),
    m_UpdateScale(true),
    m_UpdateRot(true),
    m_UpdatePos(true),
    m_UpdatePosZ(false),
    m_DefaultZ(0.f),
    m_PhysicsSimulate(false),
    m_BoneSocket(nullptr),
    m_State(Transform_State::Ground)
{
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i];
    }

    m_MeshSize = Vector3(1.f, 1.f, 1.f);
    m_RelativeScale = Vector3(1.f, 1.f, 1.f);
    m_WorldScale = Vector3(1.f, 1.f, 1.f);
}

CTransform::CTransform(const CTransform& transform)
{
    *this = transform;

    m_pCBuffer = transform.m_pCBuffer->Clone();

    m_State = Transform_State::Ground;
}

CTransform::~CTransform()
{
    SAFE_DELETE(m_pCBuffer);
}

void CTransform::InheritScale()
{
    if (m_pParent && m_InheritScale)
        m_WorldScale = m_RelativeScale * m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::InheritRot()
{
    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;

        if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
            InheritPos();
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::InheritPos()
{
    // 부모의 회전값이 존재한다면 부모의 회전으로부터 영향을 받아야 할 경우 해당 회전을
    // 적용받아서 공전을 할 수 있게 해야한다.
    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

CTransform* CTransform::Clone()
{
    return new CTransform(*this);
}

void CTransform::ClearState()
{
    m_VelocityScale = Vector3::Zero;
    m_VelocityRot = Vector3::Zero;
    m_Velocity = Vector3::Zero;
}

void CTransform::SetRelativeScale(const Vector3& Scale)
{
    m_VelocityScale = Scale - m_RelativeScale;

    m_RelativeScale = Scale;

    m_WorldScale = m_RelativeScale;

    if (m_pParent && m_InheritScale)
        m_WorldScale = m_RelativeScale * m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::SetRelativeScale(float x, float y, float z)
{
    SetRelativeScale(Vector3(x, y, z));
}

void CTransform::SetRelativeRotation(const Vector3& Rot)
{
    m_VelocityRot = Rot - m_RelativeRot;

    m_RelativeRot = Rot;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetRelativeRotation(float x, float y, float z)
{
    Vector3 Rot(x, y, z);

    m_VelocityRot = Rot - m_RelativeRot;

    m_RelativeRot = Rot;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetRelativeRotationX(float x)
{
    m_VelocityRot.x = x - m_RelativeRot.x;

    m_RelativeRot.x = x;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetRelativeRotationY(float y)
{
    m_VelocityRot.y = y - m_RelativeRot.y;

    m_RelativeRot.y = y;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetRelativeRotationZ(float z)
{
    m_VelocityRot.z = z - m_RelativeRot.z;

    m_RelativeRot.z = z;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetRelativePos(const Vector3& Pos)
{
    m_Velocity = Pos - m_RelativePos;

    m_RelativePos = Pos;

    m_WorldPos = m_RelativePos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetRelativePos(float x, float y, float z)
{
    Vector3 Pos(x, y, z);

    m_Velocity = Pos - m_RelativePos;

    m_RelativePos = Pos;

    m_WorldPos = m_RelativePos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::AddRelativeScale(const Vector3& Scale)
{
    m_VelocityScale += Scale;

    m_RelativeScale += Scale;

    m_WorldScale = m_RelativeScale;

    if (m_pParent && m_InheritScale)
        m_WorldScale = m_RelativeScale * m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::AddRelativeScale(float x, float y, float z)
{
    Vector3 Scale(x, y, z);

    m_VelocityScale += Scale;

    m_RelativeScale += Scale;

    m_WorldScale = m_RelativeScale;

    if (m_pParent && m_InheritScale)
        m_WorldScale = m_RelativeScale * m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::AddRelativeRotation(const Vector3& Rot)
{
    m_VelocityRot += Rot;

    m_RelativeRot += Rot;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddRelativeRotation(float x, float y, float z)
{
    Vector3 Rot(x, y, z);

    m_VelocityRot += Rot;

    m_RelativeRot += Rot;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddRelativeRotationX(float x)
{
    m_VelocityRot.x += x;

    m_RelativeRot.x += x;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_pParent->GetWorldRotation().x;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddRelativeRotationY(float y)
{
    m_VelocityRot.y += y;

    m_RelativeRot.y += y;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_pParent->GetWorldRotation().y;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddRelativeRotationZ(float z)
{
    m_VelocityRot.z += z;

    m_RelativeRot.z += z;

    m_WorldRot = m_RelativeRot;

    if (m_pParent)
    {
        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddRelativePos(const Vector3& Pos)
{
    m_Velocity += Pos;

    m_RelativePos += Pos;

    m_WorldPos = m_RelativePos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::AddRelativePos(float x, float y, float z)
{
    Vector3 Pos(x, y, z);

    m_Velocity += Pos;

    m_RelativePos += Pos;

    m_WorldPos = m_RelativePos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        m_WorldPos = m_RelativePos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetWorldScale(const Vector3& Scale)
{
    m_VelocityScale = Scale - m_WorldScale;

    m_WorldScale = Scale;

    m_RelativeScale = m_WorldScale;

    if (m_pParent && m_InheritScale)
        m_RelativeScale = m_WorldScale / m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::SetWorldScale(float x, float y, float z)
{
    Vector3 Scale(x, y, z);

    m_VelocityScale = Scale - m_WorldScale;

    m_WorldScale = Scale;

    m_RelativeScale = m_WorldScale;

    if (m_pParent && m_InheritScale)
        m_RelativeScale = m_WorldScale / m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::SetWorldRotation(const Vector3& Rot)
{
    m_VelocityRot = Rot - m_WorldRot;

    m_WorldRot = Rot;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_RelativeRot.x = m_WorldRot.x - m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_RelativeRot.y = m_WorldRot.y - m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_RelativeRot.z = m_WorldRot.z - m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetWorldRotation(float x, float y, float z)
{
    Vector3 Rot(x, y, z);

    m_VelocityRot = Rot - m_WorldRot;

    m_WorldRot = Rot;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_RelativeRot.x = m_WorldRot.x - m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_RelativeRot.y = m_WorldRot.y - m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_RelativeRot.z = m_WorldRot.z - m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetWorldRotationX(float x)
{
    m_VelocityRot.x = x - m_WorldRot.x;

    m_WorldRot.x = x;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_RelativeRot.x = m_WorldRot.x - m_pParent->GetWorldRotation().x;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetWorldRotationY(float y)
{
    m_VelocityRot.y = y - m_WorldRot.y;

    m_WorldRot.y = y;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotY)
            m_RelativeRot.y = m_WorldRot.y - m_pParent->GetWorldRotation().y;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetWorldRotationZ(float z)
{
    m_VelocityRot.z = z - m_WorldRot.z;

    m_WorldRot.z = z;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotZ)
            m_RelativeRot.z = m_WorldRot.z - m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetWorldPos(const Vector3& Pos)
{
    m_Velocity = Pos - m_WorldPos;

    m_WorldPos = Pos;

    m_RelativePos = m_WorldPos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::SetWorldPos(float x, float y, float z)
{
    Vector3 Pos(x, y, z);

    m_Velocity = Pos - m_WorldPos;

    m_WorldPos = Pos;

    m_RelativePos = m_WorldPos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::AddWorldScale(const Vector3& Scale)
{
    m_VelocityScale += Scale;

    m_WorldScale += Scale;

    m_RelativeScale = m_WorldScale;

    if (m_pParent && m_InheritScale)
        m_RelativeScale = m_WorldScale / m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::AddWorldScale(float x, float y, float z)
{
    Vector3 Scale(x, y, z);

    m_VelocityScale += Scale;

    m_WorldScale += Scale;

    m_RelativeScale = m_WorldScale;

    if (m_pParent && m_InheritScale)
        m_RelativeScale = m_WorldScale / m_pParent->GetWorldScale();

    m_UpdateScale = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritScale();
    }
}

void CTransform::AddWorldRotation(const Vector3& Rot)
{
    m_VelocityRot += Rot;

    m_WorldRot += Rot;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_RelativeRot.x = m_WorldRot.x - m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_RelativeRot.y = m_WorldRot.y - m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_RelativeRot.z = m_WorldRot.z - m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddWorldRotation(float x, float y, float z)
{
    Vector3 Rot(x, y, z);

    m_VelocityRot += Rot;

    m_WorldRot += Rot;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_RelativeRot.x = m_WorldRot.x - m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            m_RelativeRot.y = m_WorldRot.y - m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            m_RelativeRot.z = m_WorldRot.z - m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddWorldRotationX(float x)
{
    m_VelocityRot.x += x;

    m_WorldRot.x += x;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotX)
            m_RelativeRot.x = m_WorldRot.x - m_pParent->GetWorldRotation().x;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddWorldRotationY(float y)
{
    m_VelocityRot.y += y;

    m_WorldRot.y += y;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotY)
            m_RelativeRot.y = m_WorldRot.y - m_pParent->GetWorldRotation().y;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddWorldRotationZ(float z)
{
    m_VelocityRot.z += z;

    m_WorldRot.z += z;

    m_RelativeRot = m_WorldRot;

    if (m_pParent)
    {
        if (m_InheritRotZ)
            m_RelativeRot.z = m_WorldRot.z - m_pParent->GetWorldRotation().z;
    }

    Matrix  matRot;
    matRot.Rotation(m_WorldRot);

    // 위에서 구해준 최종 회전행렬을 이용해서 x, y, z 축을 곱하여 회전된 월드상에서의
    // 최종 축을 구해주도록 한다.
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_Axis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_Axis[i].Normalize();
    }

    m_UpdateRot = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::AddWorldPos(const Vector3& Pos)
{
    m_Velocity += Pos;

    m_WorldPos += Pos;

    m_RelativePos = m_WorldPos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::AddWorldPos(float x, float y, float z)
{
    Vector3 Pos(x, y, z);

    m_Velocity += Pos;

    m_WorldPos += Pos;

    m_RelativePos = m_WorldPos;

    if (m_pParent)
    {
        Matrix  matRot;

        Vector3 ParentRot;

        if (m_InheritRotX)
            ParentRot.x = m_pParent->GetWorldRotation().x;

        if (m_InheritRotY)
            ParentRot.y = m_pParent->GetWorldRotation().y;

        if (m_InheritRotZ)
            ParentRot.z = m_pParent->GetWorldRotation().z;

        matRot.Rotation(ParentRot);

        // 회전의 중심점을 부모의 위치로 변환한다.
        Vector3 ParentPos = m_pParent->GetWorldPos();

        if (!m_InheritPosZ)
            ParentPos.z = 0.f;

        memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

        // 역행렬을 구한다.
        matRot.Inverse();

        m_RelativePos = m_WorldPos.TransformCoord(matRot);
    }

    m_UpdatePos = true;

    // 자식 Transform이 있을 경우 모두 갱신해준다.
    size_t  Size = m_vecChild.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecChild[i]->InheritPos();
    }
}

void CTransform::LookAt(const Vector3& Pos)
{
    Vector3	Dir = Pos - m_WorldPos;
    Dir.Normalize();

    Vector3	RotAxis = m_Axis[AXIS_Z].Cross(Dir);
    RotAxis.Normalize();

    float   Angle = m_Axis[AXIS_Z].Angle(Dir);

    m_matRot = XMMatrixRotationAxis(RotAxis.Convert(), DegreeToRadian(Angle));

    m_UpdateRot = false;
}

void CTransform::LookAt(const Vector3& Pos, const Vector3& OriginDir)
{
    Vector3	Dir = Pos - m_WorldPos;
    Dir.Normalize();

    Vector3	RotAxis = OriginDir.Cross(Dir);
    RotAxis.Normalize();

    float   Angle = OriginDir.Angle(Dir);

    m_matRot = XMMatrixRotationAxis(RotAxis.Convert(), DegreeToRadian(Angle));

    m_UpdateRot = false;
}

void CTransform::LookAtYAxis(const Vector3& Pos)
{
    Vector3	Dir = Pos - m_WorldPos;
    Dir.y = 0.f;
    Dir.Normalize();

    Vector3	RotAxis = m_Axis[AXIS_Z].Cross(Dir);
    RotAxis.Normalize();

    float   Angle = m_Axis[AXIS_Z].Angle(Dir);

    m_matRot = XMMatrixRotationAxis(RotAxis.Convert(), DegreeToRadian(Angle));

    m_UpdateRot = false;
}

void CTransform::LookAtYAxis(const Vector3& Pos, const Vector3& OriginDir)
{
    Vector3	Dir = Pos - m_WorldPos;
    Dir.y = 0.f;
    Dir.Normalize();

    Vector3	RotAxis = OriginDir.Cross(Dir);
    RotAxis.Normalize();

    float   Angle = OriginDir.Angle(Dir);

    m_matRot = XMMatrixRotationAxis(RotAxis.Convert(), DegreeToRadian(Angle));

    m_UpdateRot = false;
}

void CTransform::Start()
{
    m_VelocityScale = Vector3::Zero;
    m_VelocityRot = Vector3::Zero;
    m_Velocity = Vector3::Zero;

    InheritScale();
    InheritRot();
    InheritPos();
}

bool CTransform::Init()
{
    m_pCBuffer = new CTransformConstantBuffer;

    m_pCBuffer->Init();

    return true;
}

void CTransform::Update(float DeltaTime)
{
}

void CTransform::PostUpdate(float DeltaTime)
{
    // 중력을 적용한다.
    if (m_PhysicsSimulate)
    {
    }

    if (m_State == Transform_State::Ground)
    {
        // Y값을 구한다.
        CNavigation3D* Nav = (CNavigation3D*)CNavigationManager::GetInst()->GetNavigation();

        float Height = Nav->GetHeight(m_WorldPos);

        if (m_WorldPos.y != Height)
        {
            m_WorldPos.y = Height;

            InheritPos();
        }
    }

    if (m_UpdatePosZ)
    {
        CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

        float   CameraBottom = Camera->GetCameraBottom();

        Resolution  RS = CDevice::GetInst()->GetResolution();

        float   PivotY = m_WorldPos.y - m_Pivot.y * m_WorldScale.y;

        float   ConvertY = PivotY - CameraBottom;

        float   Ratio = ConvertY / (RS.Height * 2.f);

        // Back
        if (m_DefaultZ >= 0.7f)
        {
            // Min : 0.7f   Max : 0.99999f 
            m_WorldPos.z = (0.99999f - 0.7f) * Ratio + 0.7f;
        }
        
        // Default
        else if (m_DefaultZ >= 0.3f)
        {
            // Min : 0.3f   Max : 0.69999f 
            m_WorldPos.z = (0.69999f - 0.3f) * Ratio + 0.3f;
        }
        
        // Particle
        else
        {
            // Min : 0.f   Max : 0.29999f 
            m_WorldPos.z = 0.29999f * Ratio;
        }
    }

    if (m_UpdateScale)
        m_matScale.Scaling(m_WorldScale);

    if (m_UpdateRot)
        m_matRot.Rotation(m_WorldRot);

    if (m_UpdatePos)
        m_matPos.Translation(m_WorldPos);

    m_matWorld = m_matScale * m_matRot * m_matPos;

    if (m_BoneSocket)
        m_matWorld *= m_BoneSocket->GetSocketMatrix();
}

void CTransform::SetTransform()
{
    m_pCBuffer->SetAnimation2DEnable(m_Animation2DEnable);

    m_pCBuffer->SetWorldMatrix(m_matWorld);

    CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();
    
    Matrix  matView = Camera->GetViewMatrix();

    m_pCBuffer->SetViewMatrix(matView);

    Matrix  matProj = Camera->GetProjMatrix();

    m_pCBuffer->SetProjMatrix(matProj);

    m_pCBuffer->SetPivot(m_Pivot);
    m_pCBuffer->SetMeshSize(m_MeshSize);

    m_pCBuffer->UpdateCBuffer();
    
    m_prevWorldPos = m_WorldPos;
}

void CTransform::SetShadowTransform()
{
    m_pCBuffer->SetAnimation2DEnable(m_Animation2DEnable);

    m_pCBuffer->SetWorldMatrix(m_matWorld);

    CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

    Matrix  matView = Camera->GetShadowViewMatrix();

    m_pCBuffer->SetViewMatrix(matView);

    Matrix  matProj = Camera->GetShadowProjMatrix();

    m_pCBuffer->SetProjMatrix(matProj);

    m_pCBuffer->SetPivot(m_Pivot);
    m_pCBuffer->SetMeshSize(m_MeshSize);

    m_pCBuffer->UpdateCBuffer();
}

void CTransform::ComputeWorld()
{
    m_matWorld = m_matScale * m_matRot * m_matPos;
}

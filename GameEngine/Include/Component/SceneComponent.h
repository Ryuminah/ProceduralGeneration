#pragma once

#include "Component.h"

class CSceneComponent :
    public CComponent
{
    friend class CGameObject;

protected:
    CSceneComponent();
    CSceneComponent(const CSceneComponent& com);
    virtual ~CSceneComponent();

protected:
    class CTransform* m_pTransform;
    CSceneComponent* m_pParent;
    std::vector<CSharedPtr<CSceneComponent>>    m_vecChild;
    SceneComponent_Type m_SceneComponentType;
    std::vector<Vector3>        m_vecNavPath;
    float       m_MoveSpeed;

public:
    void SetMoveSpeed(float Speed)
    {
        m_MoveSpeed = Speed;
    }

    const std::vector<Vector3>& GetNavPath()    const
    {
        return m_vecNavPath;
    }

    void StopMovement()
    {
        m_vecNavPath.clear();
    }

    void Move(const Vector2& Target);
    void Move(const Vector3& Target);
    void NavigationCallback(const std::vector<Vector3>& vecPath);

public:
    virtual void Active(bool bActive);
    void AddChild(CSceneComponent* Child, const std::string& SocketName = "");
    void DeleteChild(CSceneComponent* Child);
    void GetAllComponentName(std::vector<HierarchyName>& vecName);
    void GetAllComponent(std::vector<CSceneComponent*>& vecComponent);
    void DetatchChild(CSceneComponent* Child);
    CSceneComponent* FindComponent(const std::string& Name);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void PrevRender(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual CSceneComponent* Clone();

public:
    void SetAnimation2DEnable(bool Enable);

public:
    Vector3 GetVelocityScale()  const;
    Vector3 GetVelocityRot()    const;
    Vector3 GetVelocity()   const;
    Vector3 GetAxis(AXIS Axis)  const;

public:
    void SetInheritScale(bool Inherit);
    void SetInheritRotX(bool Inherit);
    void SetInheritRotY(bool Inherit);
    void SetInheritRotZ(bool Inherit);
    void SetInheritPosZ(bool Inherit);
    void SetUpdatePosZ(bool UpdatePosZ);
    void InheritScale();
    void InheritRot();
    void InheritPos();

public:
    bool GetInheritPosZ()	const;

public:
    Vector3 GetRelativeScale()  const;
    Vector3 GetRelativeRotation()    const;
    Vector3 GetRelativePos()    const;

public:
    void SetRelativeScale(const Vector3& Scale);
    void SetRelativeScale(float x, float y, float z);
    void SetRelativeRotation(const Vector3& Rot);
    void SetRelativeRotation(float x, float y, float z);
    void SetRelativeRotationX(float x);
    void SetRelativeRotationY(float y);
    void SetRelativeRotationZ(float z);
    void SetRelativePos(const Vector3& Pos);
    void SetRelativePos(float x, float y, float z);
    void AddRelativeScale(const Vector3& Scale);
    void AddRelativeScale(float x, float y, float z);
    void AddRelativeRotation(const Vector3& Rot);
    void AddRelativeRotation(float x, float y, float z);
    void AddRelativeRotationX(float x);
    void AddRelativeRotationY(float y);
    void AddRelativeRotationZ(float z);
    void AddRelativePos(const Vector3& Pos);
    void AddRelativePos(float x, float y, float z);

public:
    Vector3 GetWorldScale() const;
    Vector3 GetWorldRotation()  const;
    Vector3 GetWorldPos()   const;
    Vector3 GetPivot()  const;
    Vector3 GetMeshSize()   const;
    Matrix GetScaleMatrix() const;
    Matrix GetRotatinMatrix()   const;
    Matrix GetTranslationMatrix()   const;
    Matrix GetWorldMatrix() const;

public:
    void SetPivot(const Vector3& Pivot);
    void SetPivot(float x, float y, float z);
    void SetMeshSize(const Vector3& Size);
    void SetMeshSize(float x, float y, float z);

public:
    void SetWorldScale(const Vector3& Scale);
    void SetWorldScale(float x, float y, float z);
    void SetWorldRotation(const Vector3& Rot);
    void SetWorldRotation(float x, float y, float z);
    void SetWorldRotationX(float x);
    void SetWorldRotationY(float y);
    void SetWorldRotationZ(float z);
    void SetWorldPos(const Vector3& Pos);
    void SetWorldPos(float x, float y, float z);
    void AddWorldScale(const Vector3& Scale);
    void AddWorldScale(float x, float y, float z);
    void AddWorldRotation(const Vector3& Rot);
    void AddWorldRotation(float x, float y, float z);
    void AddWorldRotationX(float x);
    void AddWorldRotationY(float y);
    void AddWorldRotationZ(float z);
    void AddWorldPos(const Vector3& Pos);
    void AddWorldPos(float x, float y, float z);

public:
    void ClearTransformState();
};


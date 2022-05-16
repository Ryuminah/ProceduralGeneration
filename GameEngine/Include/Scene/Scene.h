#pragma once

#include "../GameEngine.h"
#include "../GameObject.h"
#include "SceneMode.h"

class CScene
{
    friend class CSceneManager;

private:
    CScene();
    ~CScene();

private:
    class CSceneMode* m_pSceneMode;
    class CSceneResource* m_pSceneResource;
    class CCameraManager* m_pCameraManager;
    class CSceneCollision* m_pCollision;
    class CViewport* m_pViewport;

private:
    bool m_StartScene;

public:
    bool IsStart()  const
    {
        return m_StartScene;
    }

public:
    template <typename T>
    T* GetSceneMode()    const
    {
        return (T*)m_pSceneMode;
    }

    class CSceneResource* GetResource() const
    {
        return m_pSceneResource;
    }

    class CCameraManager* GetCameraManager()    const
    {
        return m_pCameraManager;
    }

    class CSceneCollision* GetCollisionManager()    const
    {
        return m_pCollision;
    }

    class CViewport* GetViewport()  const
    {
        return m_pViewport;
    }

private:
    std::list<CSharedPtr<CGameObject>>  m_ObjList;

public:
    CGameObject* FindObject(const std::string& Name);

public:
    void Start();
    bool Init();
    void Update(float DeltaTime);
    void PostUpdate(float DeltaTime);
    void Collision(float DeltaTime);
    void Render(float DeltaTime);
    void Clear();

public:
    template <typename T>
    T* SpawnObject(const std::string& Name, const Vector3& Pos = Vector3::Zero, 
        const Vector3& Rot = Vector3::Zero, const Vector3& Scale = Vector3::One)
    {
        T* pObj = new T;

        pObj->SetName(Name);
        pObj->m_pScene = this;
        pObj->SetRelativePos(Pos);
        pObj->SetRelativeRotation(Rot);
        pObj->SetRelativeScale(Scale);

        if (!pObj->Init())
        {
            SAFE_RELEASE(pObj);
            return nullptr;
        }

        m_ObjList.push_back(pObj);
        pObj->Release();

        return pObj;
    }

    template <typename T>
    bool SetSceneMode()
    {
        SAFE_DELETE(m_pSceneMode);

        m_pSceneMode = new T;

        m_pSceneMode->m_pScene = this;

        if (!m_pSceneMode->Init())
            return false;

        return true;
    }

    template <typename T>
    T* CreateWidgetWindow(const std::string& Name)
    {
        T* Window = new T;

        Window->m_Viewport = m_pViewport;

        if (!Window->Init())
        {
            SAFE_DELETE(Window);
            return nullptr;
        }

        return Window;
    }
};


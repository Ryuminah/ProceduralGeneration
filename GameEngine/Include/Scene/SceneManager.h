#pragma once

#include "../GameEngine.h"
#include "Scene.h"

class CSceneManager
{
private:
    CScene* m_pScene;
    CScene* m_pNextScene;

public:
    CScene* GetScene()  const
    {
        return m_pScene;
    }

public:
    bool Init();
    bool Update(float DeltaTime);
    bool PostUpdate(float DeltaTime);
    bool Collision(float DeltaTime);
    bool Render(float DeltaTime);
    void Clear();
    void CreateNextScene();
    class CScene* CreateScene();
    void SetNextScene(class CScene* Scene);

private:
    bool ChangeScene();

public:
    template <typename T>
    bool SetSceneMode(bool Current = true)
    {
        if (Current)
            return m_pScene->SetSceneMode<T>();

        return m_pNextScene->SetSceneMode<T>();
    }

    DECLARE_SINGLE(CSceneManager)
};


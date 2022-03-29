
#include "Engine.h"
#include "resource.h"
#include "ClientManager.h"

#ifdef _DEBUG

#pragma comment(lib, "GameEngine_Debug.lib")

#else

#pragma comment(lib, "GameEngine.lib")

#endif // _DEBUG

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    if (!CEngine::GetInst()->Init(TEXT("Client2D"), TEXT("Client2D"), hInstance, IDI_ICON1, IDI_ICON1,
        1280, 720))
    {
        CEngine::DestroyInst();
        return 0;
    }

    if (!CClientManager::GetInst()->Init())
    {
        CClientManager::DestroyInst();
        CEngine::DestroyInst();
        return 0;
    }

    int Ret = CEngine::GetInst()->Run();

    CEngine::DestroyInst();
    CClientManager::DestroyInst();

    return Ret;
}


#include "PathManager.h"

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
    auto    iter = m_mapPath.begin();
    auto    iterEnd = m_mapPath.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE(iter->second);
    }
}

bool CPathManager::Init()
{
    TCHAR   Path[MAX_PATH] = {};

    // 실행파일 이름까지 전체경로가 나오게 된다.
    // 이 경로에서 실행파일 이름을 지우면 Bin폴더 까지의 경로가 나와서 이 경로를 BasePath로 사용한다.
    GetModuleFileName(0, Path, MAX_PATH);

    int Length = lstrlen(Path);

    for (int i = Length - 1; i > 0; --i)
    {
        if (Path[i] == '\\')
        {
            memset(&Path[i + 1], 0, sizeof(TCHAR) * (MAX_PATH - 1 - i));
            break;
        }
    }

    PathInfo* pInfo = new PathInfo;

    lstrcpy(pInfo->pPath, Path);

#ifdef UNICODE

    int ConvertLength = WideCharToMultiByte(CP_ACP, 0, pInfo->pPath, -1, nullptr, 0, nullptr, nullptr);
    WideCharToMultiByte(CP_ACP, 0, pInfo->pPath, -1, pInfo->pPathMultibyte, ConvertLength, nullptr, nullptr);

#else

    strcpy_s(pInfo->pPathMultibyte, pInfo->pPath);

#endif // UNICODE

    m_mapPath.insert(std::make_pair(BASE_PATH, pInfo));

    AddPath(SHADER_PATH, TEXT("Shader\\"));
    AddPath(TEXTURE_PATH, TEXT("Texture\\"));
    AddPath(FONT_PATH, TEXT("Font\\"));
    AddPath(SOUND_PATH, TEXT("Sound\\"));
    AddPath(MAP_PATH, TEXT("Map\\"));
    AddPath(MESH_PATH, TEXT("Mesh\\"));
    AddPath(ANIMATION_PATH, TEXT("Animation\\"));


    return true;
}

bool CPathManager::AddPath(const std::string& Name, const TCHAR* pAddPath, const std::string& BaseName)
{
    if (FindPath(Name))
        return false;

    const PathInfo* pBaseInfo = FindPath(BaseName);

    if (!pBaseInfo)
        return false;

    PathInfo* pInfo = new PathInfo;

    lstrcpy(pInfo->pPath, pBaseInfo->pPath);
    lstrcat(pInfo->pPath, pAddPath);

#ifdef UNICODE

    int ConvertLength = WideCharToMultiByte(CP_ACP, 0, pInfo->pPath, -1, nullptr, 0, nullptr, nullptr);
    WideCharToMultiByte(CP_ACP, 0, pInfo->pPath, -1, pInfo->pPathMultibyte, ConvertLength, nullptr, nullptr);

#else

    strcpy_s(pInfo->pPathMultibyte, pInfo->pPath);

#endif // UNICODE

    m_mapPath.insert(std::make_pair(Name, pInfo));

    return true;
}

const PathInfo* CPathManager::FindPath(const std::string& Name)
{
    auto    iter = m_mapPath.find(Name);

    if (iter == m_mapPath.end())
        return nullptr;

    return iter->second;
}

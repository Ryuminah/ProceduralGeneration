#pragma once

#include "GameEngine.h"

struct PathInfo
{
    TCHAR* pPath;
    char* pPathMultibyte;

    PathInfo()
    {
        pPath = new TCHAR[MAX_PATH];
        memset(pPath, 0, sizeof(TCHAR) * MAX_PATH);

        pPathMultibyte = new char[MAX_PATH];
        memset(pPathMultibyte, 0, sizeof(char) * MAX_PATH);
    }

    ~PathInfo()
    {
        SAFE_DELETE_ARRAY(pPathMultibyte);
        SAFE_DELETE_ARRAY(pPath);
    }
};

class CPathManager
{
private:
    std::unordered_map<std::string, PathInfo*>  m_mapPath;

public:
    bool Init();
    bool AddPath(const std::string& Name, const TCHAR* pAddPath, const std::string& BaseName = BASE_PATH);
    const PathInfo* FindPath(const std::string& Name);

    DECLARE_SINGLE(CPathManager)
};


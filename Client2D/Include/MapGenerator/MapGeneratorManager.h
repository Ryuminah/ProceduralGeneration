#pragma once
#include <GameEngine.h>

class CMapGeneratorManager
{
    class CMapGenerator* m_pCurrentMapGenerator;

public:
    std::unordered_map<std::string, class CMapGenerator*> m_AllMapGenerator;

public:
    class CMapGenerator* FindMapGenerator(const std::string& Name);

public:
    template <typename T>
    bool CreateMapGenerator(const std::string& Name)
    {
        T* pMapGenerator = (T*)FindMapGenerator(Name);

        if (pMapGenerator)
            return true;

        pMapGenerator = new T;

        pMapGenerator->SetName(Name);

       /* if (!pMapGenerator->Init())
        {
            SAFE_RELEASE(pShader);
            return false;
        }*/

        m_AllMapGenerator.insert(std::make_pair(Name, pMapGenerator));

        if (!m_pCurrentMapGenerator)
        {
            m_pCurrentMapGenerator = pMapGenerator;
        }

        return true;
    }

    DECLARE_SINGLE(CMapGeneratorManager)

};


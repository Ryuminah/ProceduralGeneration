#include "MapGeneratorManager.h"
#include "IMapGenerator.h"

DEFINITION_SINGLE(CMapGeneratorManager)

CMapGeneratorManager::CMapGeneratorManager() : m_pCurrentMapGenerator(nullptr)
{
}

CMapGeneratorManager::~CMapGeneratorManager()
{
    {
        auto    iter = m_AllMapGenerator.begin();
        auto    iterEnd = m_AllMapGenerator.end();

        for (; iter != iterEnd; ++iter)
        {
            SAFE_DELETE(iter->second);
        }
    }
}

CMapGenerator* CMapGeneratorManager::FindMapGenerator(const std::string& Name)
{
    auto    iter = m_AllMapGenerator.find(Name);

    if (iter == m_AllMapGenerator.end())
        return nullptr;

    return iter->second;
}

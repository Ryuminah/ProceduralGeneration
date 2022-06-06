#include "GenerateOptionManager.h"
#include "MapGeneratorManager.h"

DEFINITION_SINGLE(CGenerateOptionManager)

CGenerateOptionManager::CGenerateOptionManager() 
{
}

CGenerateOptionManager::~CGenerateOptionManager()
{
    /*{
        auto    iter = m_AllMapGenerator.begin();
        auto    iterEnd = m_AllMapGenerator.end();

        for (; iter != iterEnd; ++iter)
        {
            SAFE_DELETE(iter->second);
        }
    }*/
}

bool CGenerateOptionManager::Init()
{
    // ¾ÆÁ÷ ,,
    return true;
}

AGenerateOption* CGenerateOptionManager::FindGenerateOption(const std::string& Name)
{
    CMapGenerator* CurrentMapGenerator = CMapGeneratorManager::GetInst()->GetCurrentMapGenerator();

    if (!CurrentMapGenerator)
    {
        return nullptr;
    }

    
}


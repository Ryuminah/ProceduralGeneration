#pragma once
#pragma once
#include <GameEngine.h>
#include "MapEnum.h"

// �ɼ��� �����ϴ� ��ɸ� ���.
// ������ �ɼ��� �ش� ���� MapGenerator�� ����ȴ�.
class CGenerateOptionManager
{
private:
  /*  class CMapGenerator* m_pCurrentMapGenerator;
    std::unordered_map<std::string, class CMapGenerator*> m_AllMapGenerator;*/

public:
    bool Init();

public:
    class AGenerateOption* FindGenerateOption(const std::string& Name);

public:
    // �̰� ����� .. . .
    template <typename T>
    T* CreateGenerateOption(const std::string& Name, OPTION_LEVEL OptionLevel = OPTION_LEVEL::NORMAL)
    {
        T* pGenerateOption = (T*)FindGenerateOption(Name);

        if (pGenerateOption)
            return pGenerateOption;

        pGenerateOption = new T;

        

        //if (!pGenerateOption->Init(pRandomMap))
        //{
        //    SAFE_DELETE(pMapGenerator);
        //    return nullptr;
        //}

        //m_AllMapGenerator.insert(std::make_pair(Name, pMapGenerator));

        //if (!m_pCurrentMapGenerator)
        //{
        //    m_pCurrentMapGenerator = pMapGenerator;
        //}

        return pGenerateOption;
    }

    DECLARE_SINGLE(CGenerateOptionManager)
};


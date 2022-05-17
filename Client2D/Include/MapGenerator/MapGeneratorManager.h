#pragma once
#include <GameEngine.h>

class CMapGeneratorManager
{
private:
    class CMapGenerator* m_pCurrentMapGenerator;
    std::unordered_map<std::string, class CMapGenerator*> m_AllMapGenerator;

public :
    class CMapGenerator* FindMapGenerator(const std::string& Name);

public:
    // ���� RandomMap ��ü�� �����Ҷ��� �� �����⸦ ������ �� ����.
    template <typename T>
    CMapGenerator* CreateMapGenerator(const std::string& Name, class CRandomMap* pRandomMap)
    {
        T* pMapGenerator = (T*)FindMapGenerator(Name);

        if (pMapGenerator)
            return pMapGenerator;

        pMapGenerator = new T;

        // �̰� �ʿ��������
        pMapGenerator->SetName(Name);

        if (!pMapGenerator->Init(pRandomMap))
        {
            SAFE_DELETE(pMapGenerator);
            return nullptr;
        }

        m_AllMapGenerator.insert(std::make_pair(Name, pMapGenerator));

        if (!m_pCurrentMapGenerator)
        {
            m_pCurrentMapGenerator = pMapGenerator;
        }

        return pMapGenerator;
    }

    DECLARE_SINGLE(CMapGeneratorManager)

};


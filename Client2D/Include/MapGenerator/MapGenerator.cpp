#include <time.h>
#include <random>
#include "MapGenerator.h"
#include "RandomMap.h"
#include "Engine.h"
#include "Timer.h"
#include "Input.h"

CMapGenerator::CMapGenerator() : m_IsGenerateWorldEnd(false)
{
}

CMapGenerator::~CMapGenerator()
{
	// ���� �Ҹ��� ȣǮ �ȵ� ����
	//int a = 0;
}

bool CMapGenerator::Init()
{

	return true;
}

bool CMapGenerator::Init(CRandomMap* pRandomMap)
{
	if (!m_pRandomMap)
	{
		m_pRandomMap = pRandomMap;
	}

	// CMapGenerator���� RandomMap ��ü�� ���� �������ش�.
	CTileMapComponent* pMapComponent = m_pRandomMap->CreateSceneComponent<CTileMapComponent>("RandomMap");
	m_pRandomMap->m_MapComponent = pMapComponent;
	m_pRandomMap->SetRootComponent(pMapComponent);

	// Tile���� �����Ѵ�.
	pMapComponent->CreateTile<CTile>(Tile_Shape::Rect, m_pRandomMap->m_MapSizeX, m_pRandomMap->m_MapSizeY, Vector2(TILE_SIZE_SMALL, TILE_SIZE_SMALL));

	return true;
}

// virtual
void CMapGenerator::GenerateWorld(LAND_STATE _tileState)
{
	switch (_tileState)
	{
	case LAND_STATE::BASE:
		GenerateBase();
		break;
	case LAND_STATE::LAND:
		GenerateLand();
		break;
	case LAND_STATE::SEA:
		GenerateSea();
		break;
	case LAND_STATE::COAST:
		GenerateCoast();
		break;
	case LAND_STATE::LAKE:
		GenerateLake();
		break;
	case LAND_STATE::FOREST:
		GenerateForest();
		break;
	default:
		break;
	}
}

void CMapGenerator::GenerateVegetation(LAND_STATE _landState)
{
	if (!m_IsGenerateWorldEnd)
	{
		return;
	}


	// ������� ������ Ÿ�� ������ ������� 
	// �Ļ��� �����Ѵ�.
}


// Member Function
void CMapGenerator::GenerateBase()
{
	// �⺻ �� ���� ����
	// �õ尪�� �̿��ؼ� ���� ����� ����

	CEngine::GetInst()->OnDebugLog();
	m_IsGenerateWorldEnd = false;

	for (int x = 0; x < m_pRandomMap->m_MapSizeX; ++x)
	{
		std::vector<LAND_STATE> tileData;

		for (int y = 0; y < m_pRandomMap->m_MapSizeY; ++y)
		{
			tileData.push_back(LAND_STATE::LAND);
		}

		m_TileData.push_back(tileData);
	}

	clock_t start = clock(); // ���� �ð� ����

	CellularAutomata();

	clock_t end = clock();
	double clockTime = (double)(end - start) / CLOCKS_PER_SEC;
	char buffer[BUFSIZ];
	sprintf_s(buffer, "%f", clockTime);
}

void CMapGenerator::GenerateLand()
{
	// Land �ɼ� �߰�... ��� ����...
}

void CMapGenerator::GenerateSea()
{
	
}

void CMapGenerator::GenerateCoast()
{
	// �ؾȰ� ���� �ڵ�
}

void CMapGenerator::GenerateLake()
{
	// ȣ�� Ÿ�� ����
}

void CMapGenerator::GenerateForest()
{
	// ȣ�� ��ó�� �� ������ ����.
}

void CMapGenerator::CellularAutomata()
{
}

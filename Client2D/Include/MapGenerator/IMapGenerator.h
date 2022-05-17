#pragma once
#include <GameEngine.h>

enum LAND_STATE
{
	BASE = -1,
	LAND, 
	SEA,
	COAST,
	LAKE,
	FOREST
};

// °í¹Î
enum VEGETATION_STATE
{
	
};

class IMapGenerator
{
public:
	std::string m_Name;

public:
	virtual ~IMapGenerator();

public:
	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	const std::string& GetName()    const
	{
		return m_Name;
	}

public:
	virtual void GenerateWorld(LAND_STATE _landState) = 0;
	virtual void GenerateVegetation(LAND_STATE _landState) = 0;

};


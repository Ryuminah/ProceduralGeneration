#pragma once
#include <GameEngine.h>
#include "MapEnum.h"

class IMapGenerator
{
private:
	// 이름 안쓸수도
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
	virtual void GenerateWorld(TILE_STATE _landState) = 0;
	virtual void GenerateVegetation(TILE_STATE _landState) = 0;

};


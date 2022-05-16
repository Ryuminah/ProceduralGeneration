#pragma once
#include <GameEngine.h>

enum LAND_STATE
{
	BASE,
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
	virtual ~IMapGenerator();

public:
	virtual void GenerateWorld(LAND_STATE _landState) = 0;
	virtual void GenerateVegetation(LAND_STATE _landState) = 0;


};


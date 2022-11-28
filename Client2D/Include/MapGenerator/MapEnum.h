#pragma once
#define TILE_STATE_SIZE 5

enum TILE_STATE
{
	BASE = -1,
	LAND,
	SEA,
	COAST,
	LAKE,
	FOREST,
	CLEAR
};

// 고민
enum VEGETATION_STATE
{

};

// 식생분포, 스무딩 강도 등 정할때 옵션
enum class OPTION_LEVEL
{
	MIN,
	NORMAL,
	MAX
};

enum class MAPBASE_SHAPE
{
	DEFAULT = 0,
	WIDTH,
	HEIGHT
};
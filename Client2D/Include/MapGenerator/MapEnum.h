#pragma once

enum TILE_STATE
{
	BASE = -1,
	LAND,
	SEA,
	COAST,
	LAKE,
	FOREST
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
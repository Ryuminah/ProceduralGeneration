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

// ���
enum VEGETATION_STATE
{

};

// �Ļ�����, ������ ���� �� ���Ҷ� �ɼ�
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
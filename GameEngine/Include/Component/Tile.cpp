
#include "Tile.h"
#include "TileMapComponent.h"

CTile::CTile()	:
	m_Shape(Tile_Shape::Rect),
	m_TileType(Tile_Type::None),
	m_IndexX(-1),
	m_IndexY(-1),
	m_Index(-1),
	m_FrameX(0),
	m_FrameY(0)
{
}

CTile::CTile(const CTile& tile)
{
	*this = tile;
}

CTile::~CTile()
{
}

Vector2 CTile::GetPos()	const
{
	Vector3	OwnerPos = m_Owner->GetWorldPos();
	Vector2	Pos = m_Pos + Vector2(OwnerPos.x, OwnerPos.y);

	return Pos;
}

bool CTile::Init()
{
	return true;
}

void CTile::Start()
{
	m_Center = m_Pos + m_Size / 2.f;

	m_NodeDistW = m_Size.x * m_Size.x;
	m_NodeDistH = m_Size.y * m_Size.y;
	m_NodeDistD = m_Size.x * m_Size.x + m_Size.y * m_Size.y;

	float	HalfW = m_Size.x * 0.5f;
	float	HalfH = m_Size.y * 0.5f;

	m_NodeDistDHalf = HalfW * HalfW + HalfH * HalfH;
}

void CTile::Update(float DeltaTime)
{
}

void CTile::PostUpdate(float DeltaTime)
{
	Vector3	OwnerPos = m_Owner->GetWorldPos();

	Vector2	Pos = m_Pos + Vector2(OwnerPos.x, OwnerPos.y);

	Matrix	matScale, matTranslate;

	matScale.Scaling(m_Size.x, m_Size.y, 1.f);
	matTranslate.Translation(Pos.x, Pos.y, 0.f);

	m_matWorld = matScale * matTranslate;
}

void CTile::Render(float DeltaTime)
{
}

void CTile::Save(FILE* pFile)
{
	fwrite(&m_Shape, sizeof(Tile_Shape), 1, pFile);
	fwrite(&m_TileType, sizeof(Tile_Type), 1, pFile);
	fwrite(&m_Pos, sizeof(Vector2), 1, pFile);
	fwrite(&m_Size, sizeof(Vector2), 1, pFile);
	fwrite(&m_Center, sizeof(Vector2), 1, pFile);
	fwrite(&m_FrameStart, sizeof(Vector2), 1, pFile);
	fwrite(&m_FrameEnd, sizeof(Vector2), 1, pFile);
	fwrite(&m_IndexX, sizeof(int), 1, pFile);
	fwrite(&m_IndexY, sizeof(int), 1, pFile);
	fwrite(&m_Index, sizeof(int), 1, pFile);
}

void CTile::Load(FILE* pFile)
{
	fread(&m_Shape, sizeof(Tile_Shape), 1, pFile);
	fread(&m_TileType, sizeof(Tile_Type), 1, pFile);
	fread(&m_Pos, sizeof(Vector2), 1, pFile);
	fread(&m_Size, sizeof(Vector2), 1, pFile);
	fread(&m_Center, sizeof(Vector2), 1, pFile);
	fread(&m_FrameStart, sizeof(Vector2), 1, pFile);
	fread(&m_FrameEnd, sizeof(Vector2), 1, pFile);
	fread(&m_IndexX, sizeof(int), 1, pFile);
	fread(&m_IndexY, sizeof(int), 1, pFile);
	fread(&m_Index, sizeof(int), 1, pFile);
}

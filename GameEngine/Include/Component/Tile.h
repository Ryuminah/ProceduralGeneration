#pragma once

#include "../GameEngine.h"

class CTile
{
	friend class CTileMapComponent;

private:
	CTile();
	CTile(const CTile& tile);
	virtual ~CTile();

private:
	Tile_Shape	m_Shape;
	Tile_Type	m_TileType;
	Vector2		m_Pos;
	Vector2		m_Size;
	Vector2		m_Center;
	Matrix		m_matWorld;
	Vector2		m_FrameStart;
	Vector2		m_FrameEnd;
	int			m_FrameX;
	int			m_FrameY;
	int			m_IndexX;
	int			m_IndexY;
	int			m_Index;
	class CTileMapComponent* m_Owner;
	float		m_NodeDistW;	// 가로 길이
	float		m_NodeDistH;	// 세로 길이
	float		m_NodeDistD;	// 대각선 길이
	float		m_NodeDistDHalf;	// 대각선 반 길이

public:
	float GetDistW()	const
	{
		return m_NodeDistW;
	}

	float GetDistH()	const
	{
		return m_NodeDistH;
	}

	float GetDistD()	const
	{
		return m_NodeDistD;
	}

	float GetDistDHalf()	const
	{
		return m_NodeDistDHalf;
	}

	bool GetRender()	const
	{
		return m_FrameStart.x != 0.f || m_FrameStart.y != 0.f || m_FrameEnd.x != 0.f || m_FrameEnd.y != 0.f;
	}

	int GetIndexX()	const
	{
		return m_IndexX;
	}

	int GetIndexY()	const
	{
		return m_IndexY;
	}

	int GetIndex()	const
	{
		return m_Index;
	}

	int GetFrameX()	const
	{
		return m_FrameX;
	}

	int GetFrameY()	const
	{
		return m_FrameY;
	}

	Tile_Type GetTileType()	const
	{
		return m_TileType;
	}

	Tile_Shape GetTileShape()	const
	{
		return m_Shape;
	}

	const Matrix& GetWorldMatrix()	const
	{
		return m_matWorld;
	}

	const Vector2& GetFrameStart()	const
	{
		return m_FrameStart;
	}

	const Vector2& GetFrameEnd()	const
	{
		return m_FrameEnd;
	}

	Vector2 GetPos()	const;

	const Vector2& GetSize()	const
	{
		return m_Size;
	}

	const Vector2& GetCenter()	const
	{
		return m_Center;
	}

public:
	void SetIndex(int IndexX, int IndexY, int Index)
	{
		m_IndexX = IndexX;
		m_IndexY = IndexY;
		m_Index = Index;
	}

	void SetShape(Tile_Shape Shape)
	{
		m_Shape = Shape;
	}

	void SetTileType(Tile_Type Type)
	{
		m_TileType = Type;
	}

	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}

	void SetSize(float x, float y)
	{
		m_Size = Vector2(x, y);
	}

	void SetFrameStart(const Vector2& FrameStart)
	{
		m_FrameStart = FrameStart;
	}

	void SetFrameStart(float x, float y)
	{
		m_FrameStart = Vector2(x, y);
	}

	void SetFrameEnd(const Vector2& FrameEnd)
	{
		m_FrameEnd = FrameEnd;
	}

	void SetFrameEnd(float x, float y)
	{
		m_FrameEnd = Vector2(x, y);
	}

	void SetFrame(int x, int y)
	{
		m_FrameX = x;
		m_FrameY = y;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};


#pragma once
#include "ConstantBufferData.h"
class CTileMapCBuffer :
    public CConstantBufferData
{
public:
	CTileMapCBuffer();
	CTileMapCBuffer(const CTileMapCBuffer& buffer);
	virtual ~CTileMapCBuffer();

protected:
	TileMapCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CTileMapCBuffer* Clone();

public:
	void SetTileImageSize(float x, float y)
	{
		m_BufferData.TileImageSize = Vector2(x, y);
	}

	void SetTileImageSize(const Vector2& Size)
	{
		m_BufferData.TileImageSize = Size;
	}

	void SetTileSize(float x, float y)
	{
		m_BufferData.TileSize = Vector2(x, y);
	}

	void SetTileSize(const Vector2& Size)
	{
		m_BufferData.TileSize = Size;
	}

	void SetTileIndex(int StartX, int StartY, int EndX, int EndY)
	{
		m_BufferData.TileStartX = StartX;
		m_BufferData.TileStartY = StartY;
		m_BufferData.TileEndX = EndX;
		m_BufferData.TileEndY = EndY;
	}
};


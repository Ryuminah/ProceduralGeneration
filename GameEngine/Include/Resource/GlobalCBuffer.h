#pragma once
#include "ConstantBufferData.h"
class CGlobalCBuffer :
    public CConstantBufferData
{
public:
	CGlobalCBuffer();
	CGlobalCBuffer(const CGlobalCBuffer& buffer);
	virtual ~CGlobalCBuffer();

protected:
	GlobalCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CGlobalCBuffer* Clone();

public:
	void SetCameraBottom(float Bottom)
	{
		m_BufferData.CameraBottom = Bottom;
	}

	void SetDeltaTime(float DeltaTime)
	{
		m_BufferData.DeltaTime = DeltaTime;
	}

	void SetAccTime(float AccTime)
	{
		m_BufferData.AccTime = AccTime;
	}

	void SetResolution(const Vector2& Resolution)
	{
		m_BufferData.Resolution = Resolution;
	}

	void SetNoiseResolution(const Vector2& NoiseResolution)
	{
		m_BufferData.NoiseResolution = NoiseResolution;
	}
};


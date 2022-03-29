#pragma once

#include "../GameEngine.h"

class CRenderState
{
	friend class CRenderStateManager;

protected:
	CRenderState();
	virtual ~CRenderState();

protected:
	std::string			m_Name;
	RenderState_Type	m_Type;
	ID3D11DeviceChild* m_State;
	ID3D11DeviceChild* m_PrevState;

public:
	const std::string& GetName()	const
	{
		return m_Name;
	}

	RenderState_Type GetType()	const
	{
		return m_Type;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;
};


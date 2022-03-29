#pragma once
#include "IMGUIWindow.h"
class CDetailWindow :
    public CIMGUIWindow
{
public:
	CDetailWindow();
	virtual ~CDetailWindow();

private:
	class CIMGUITextInput* m_InputPosX;
	class CIMGUITextInput* m_InputPosY;
	class CIMGUITextInput* m_InputPosZ;

public:
	void SetPosition(const Vector3& Pos);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void InputPosX();
	void InputPosY();
	void InputPosZ();
};


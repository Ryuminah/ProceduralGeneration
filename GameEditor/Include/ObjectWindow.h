#pragma once
#include "IMGUIWindow.h"
#include "GameObject.h"
#include "Component/SceneComponent.h"
class CObjectWindow :
    public CIMGUIWindow
{
public:
	CObjectWindow();
	virtual ~CObjectWindow();

private:
	unsigned int	m_CreateObjectCount;
	class CIMGUIListBox* m_ObjListBox;
	class CIMGUIListBox* m_ComponentListBox;
	CSharedPtr<CGameObject> m_SelectObject;
	int		m_SelectObjectIndex;
	CSharedPtr<CSceneComponent> m_SelectComponent;
	int		m_SelectComponentIndex;
	int		m_CreateComponentIndex;

	class CIMGUITextInput* m_NameInput;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void ListCallback(int SelectIndex, const char* Item);
	void ComponentListCallback(int SelectIndex, const char* Item);
	void ComponentComboCallback(int SelectIndex, const char* Item);
	void CreateObjectButtonClick();
	void InputNamePopupButton();

public:
	void SetPosX(float x);
	void SetPosY(float y);
	void SetPosZ(float z);
	void SaveButton();
	void LoadButton();
};


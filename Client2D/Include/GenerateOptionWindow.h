#pragma once
#include "IMGUIWindow.h"

// 맵 처음 생성 시의 옵션을 지정하는 UI
// 생성이 끝난 후, Map Info UI를 띄운다.
class CGenerateOptionWindow : public CIMGUIWindow
{
public:
	CGenerateOptionWindow();
	virtual ~CGenerateOptionWindow();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	class CIMGUITextInput* m_MapSizeX;
	class CIMGUITextInput* m_MapSizeY;
	class CIMGUITextInput* m_InputPosZ;

	class CIMGUIComboBox* m_TileTypeComboBox;

	class CIMGUIListBox* m_TileTypeListBox;

	// 현재 맵
private:
	//class CMapGenerator* m_CurrentGenerator;

private:
	void CreateGenerateOptionBox();

public:
	void ComboBoxCallback(int SelectIndex, const char* Item);
	void Button_CreateMapCallBack();
	void Button_ClearCallBack();

	//Get, Set
//public:
//	void SetGenerateOwner(CMapGenerator* pGenerator)
//	{
//		//m_CurrentGenerator = pGenerator;
//	}

};


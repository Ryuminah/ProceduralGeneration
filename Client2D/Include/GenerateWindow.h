#pragma once
#include "IMGUIWindow.h"
//#include "MapGenerator/"

class CGenerateWindow :
    public CIMGUIWindow
{
public:
	CGenerateWindow();
	virtual ~CGenerateWindow();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	class CIMGUIComboBox* m_TileTypeComboBox;
	int m_SelectTileTypeIndex;

	class CIMGUIListBox* m_TileTypeListBox;


	// 현재 맵
	// 맵 저장하고 불러오기까지 제작 가능할까?
	class CMapGenerator* m_CurrentGenerator;

public:
	void ComboBoxCallback(int SelectIndex, const char* Item);

};


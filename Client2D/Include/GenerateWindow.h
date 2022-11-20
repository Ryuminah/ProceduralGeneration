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


	// ���� ��
	// �� �����ϰ� �ҷ�������� ���� �����ұ�?
	class CMapGenerator* m_CurrentGenerator;

public:
	void ComboBoxCallback(int SelectIndex, const char* Item);

};


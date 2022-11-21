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
private:
	class CMapGenerator* m_CurrentGenerator;

private:
	void CreateGenerateComboBox();

public:
	void ComboBoxCallback(int SelectIndex, const char* Item);
	void Button_GenerateCallBack();
	void Button_ClearCallBack();

	//Get, Set
public:
	void SetGenerateOwner(CMapGenerator* pGenerator)
	{
		m_CurrentGenerator = pGenerator;
	}

	void ChangeMapGenerator(std::string generatorName)
	{
		// GenratorManager에서 이름을 탐색한 뒤,
		// 맵 정보를 갱신한다.
	}
};


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
		// GenratorManager���� �̸��� Ž���� ��,
		// �� ������ �����Ѵ�.
	}
};


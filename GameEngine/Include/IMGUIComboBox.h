#pragma once
#include "IMGUIWidget.h"
class CIMGUIComboBox :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIComboBox();
	virtual ~CIMGUIComboBox();

protected:
	std::vector<std::string>	m_vecItem;
	std::string	m_PrevViewName;
	bool	m_Select;
	int		m_SelectIndex;
	bool	m_Sort;
	ImGuiComboFlags	m_Flag;
	std::function<void(int, const char*)>	m_SelectCallback;

public:
	/*int GetSelectIndex()	const
	{
		return m_SelectIndex;
	}

	const char* GetSelectItem()	const
	{
		return m_vecItem[m_SelectIndex].c_str();
	}*/

public:
	int GetItemCount()	const
	{
		return (int)m_vecItem.size();
	}

	void AddItem(const char* Item)
	{
		m_vecItem.push_back(Item);

		if (m_Sort)
			std::sort(m_vecItem.begin(), m_vecItem.end());
	}

	void DeleteItem(int Index)
	{
		auto	iter = m_vecItem.begin() + Index;

		m_vecItem.erase(iter);
	}

	void DeleteAllItem()
	{
		m_vecItem.clear();
	}

	void Sort(bool bSort)
	{
		m_Sort = bSort;

		if (m_Sort)
			std::sort(m_vecItem.begin(), m_vecItem.end());
	}

	void AddFlag(ImGuiComboFlags_ Flag)
	{
		m_Flag |= Flag;
	}

public:
	virtual bool Init();
	virtual void Render();

public:
	template <typename T>
	void SetSelectCallback(T* Obj, void(T::* Func)(int, const char*))
	{
		m_SelectCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}
};


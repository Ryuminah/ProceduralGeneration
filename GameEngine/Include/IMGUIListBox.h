#pragma once
#include "IMGUIWidget.h"
class CIMGUIListBox :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIListBox();
	virtual ~CIMGUIListBox();

protected:
	std::vector<std::string>	m_vecItem;
	bool	m_Select;
	int		m_SelectIndex;
	int		m_PageItemCount;
	bool	m_Sort;
	std::function<void(int, const char*)>	m_SelectCallback;

public:
	const std::string& GetItem(int Index)
	{
		return m_vecItem[Index];
	}

public:
	void AddItem(const char* Item)
	{
		m_vecItem.push_back(Item);

		if (m_Sort)
			std::sort(m_vecItem.begin(), m_vecItem.end());
	}

	void SetPageItemCount(int Count)
	{
		m_PageItemCount = Count;
	}

	void DeleteItem(int Index)
	{
		auto	iter = m_vecItem.begin() + Index;

		m_vecItem.erase(iter);
	}

	void Clear()
	{
		m_vecItem.clear();
	}

	void Sort(bool bSort)
	{
		m_Sort = bSort;

		if (m_Sort)
			std::sort(m_vecItem.begin(), m_vecItem.end());
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


#pragma once
#include "MapEnum.h"

class AGenerateOption
{
protected:
	OPTION_LEVEL m_OptionLevel;
	class CMapGenerator* m_pOwner;


public:
	AGenerateOption();
	virtual ~AGenerateOption();

	virtual void PerformOptionLevel();

public:
	virtual void Level_MIN() = 0;
	virtual void Level_NORMAL() = 0;
	virtual void Level_MAX() = 0;


public:
	OPTION_LEVEL GetOptionLevel()
	{
		return m_OptionLevel;
	}

	void SetOptionLevel(OPTION_LEVEL optionLevel)
	{
		m_OptionLevel = optionLevel;
	}
};


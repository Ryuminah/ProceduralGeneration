#pragma once
#include "AMapGeneratorOption.h"

class CSmoothingOption :
    public AMapGeneratorOption
{
public:
	CSmoothingOption();
	CSmoothingOption(OPTION_LEVEL optionLevel);
	virtual ~CSmoothingOption();

public:
	virtual void Level_MIN();
	virtual void Level_NORMAL();
	virtual void Level_MAX();
};


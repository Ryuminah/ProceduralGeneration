#include "SmoothingOption.h"

CSmoothingOption::CSmoothingOption()
{

}

CSmoothingOption::CSmoothingOption(OPTION_LEVEL optionLevel)
{
	m_OptionLevel = optionLevel;
}

CSmoothingOption::~CSmoothingOption()
{

}

void CSmoothingOption::Level_MIN()
{
	// m_pOwner-> 해당 기능 가져와
	// 몇번 실행시킬지만 옵션 별로 정해 !!!
	// 
	// 최대, 최소, 중간의 기준을 어떻게 정할지 생각해보깅
}

void CSmoothingOption::Level_NORMAL()
{
}

void CSmoothingOption::Level_MAX()
{
}

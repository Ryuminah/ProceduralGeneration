#include "AMapGeneratorOption.h"

AMapGeneratorOption::AMapGeneratorOption() : m_OptionLevel(OPTION_LEVEL::NORMAL)
{

}

AMapGeneratorOption::~AMapGeneratorOption()
{

}

void AMapGeneratorOption::PerformOptionLevel()
{
	switch (m_OptionLevel)
	{
	case OPTION_LEVEL::MIN:
		Level_MIN();
		break;

	case OPTION_LEVEL::NORMAL:
		Level_NORMAL();
		break;

	case OPTION_LEVEL::MAX:
		Level_MAX();
		break;

	default:
		break;
	}
}

#include "AGenerateOption.h"

AGenerateOption::AGenerateOption() : m_OptionLevel(OPTION_LEVEL::NORMAL), m_pOwner(nullptr)
{

}

AGenerateOption::~AGenerateOption()
{

}

void AGenerateOption::PerformOptionLevel()
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

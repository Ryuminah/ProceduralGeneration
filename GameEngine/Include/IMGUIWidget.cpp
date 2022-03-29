#include "IMGUIWidget.h"
#include "IMGUIManager.h"

CIMGUIWidget::CIMGUIWidget()    :
    m_Name{},
    m_UnicodeName{},
    m_Font(nullptr)
{
    SetColor(1.f, 1.f, 1.f);
}

CIMGUIWidget::~CIMGUIWidget()
{
}

void CIMGUIWidget::SetFont(const std::string& Name)
{
    m_Font = CIMGUIManager::GetInst()->FindFont(Name);
}

bool CIMGUIWidget::Init()
{
    return true;
}

void CIMGUIWidget::Render()
{
}

#include "Ref.h"

CRef::CRef()    :
    m_RefCount(1),
    m_Active(true),
    m_Enable(true)
{
}

CRef::~CRef()
{
}

int CRef::AddRef()
{
    ++m_RefCount;

    return m_RefCount;
}

int CRef::Release()
{
    --m_RefCount;

    if (m_RefCount == 0)
    {
        delete  this;
        return 0;
    }

    return m_RefCount;
}

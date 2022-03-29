#pragma once

#include "GameEngine.h"

class CRef
{
public:
    CRef();
    virtual ~CRef();

protected:
    std::string m_Name;
    int         m_RefCount;
    bool        m_Enable;
    bool        m_Active;

public:
    void Enable(bool bEnable)
    {
        m_Enable = bEnable;
    }

    virtual void Active(bool bActive)
    {
        m_Active = bActive;
    }

    bool IsActive() const
    {
        return m_Active;
    }

    bool IsEnable() const
    {
        return m_Enable;
    }

    void SetName(const std::string& Name)
    {
        m_Name = Name;
    }

    const std::string& GetName()    const
    {
        return m_Name;
    }

    int GetRefCount()   const
    {
        return m_RefCount;
    }
    int AddRef();
    int Release();
};


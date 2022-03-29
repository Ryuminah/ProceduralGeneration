#pragma once

template <typename T>
class CSharedPtr
{
public:
    CSharedPtr()    :
        m_pPtr(nullptr)
    {
    }

    CSharedPtr(T* pPtr)
    {
        m_pPtr = pPtr;

        if (m_pPtr)
            m_pPtr->AddRef();
    }

    CSharedPtr(const CSharedPtr<T>& Ptr)
    {
        m_pPtr = Ptr.m_pPtr;

        if (m_pPtr)
            m_pPtr->AddRef();
    }

    ~CSharedPtr()
    {
        if (m_pPtr)
            m_pPtr->Release();
    }

private:
    T* m_pPtr;

public:
    T* Get()    const
    {
        return m_pPtr;
    }

public:
    void operator = (T* pPtr)
    {
        if (m_pPtr)
            m_pPtr->Release();

        m_pPtr = pPtr;

        if (m_pPtr)
            m_pPtr->AddRef();
    }

    void operator = (const CSharedPtr<T>& Ptr)
    {
        if (m_pPtr)
            m_pPtr->Release();

        m_pPtr = Ptr.m_pPtr;

        if (m_pPtr)
            m_pPtr->AddRef();
    }

    bool operator == (T* pPtr)  const
    {
        return m_pPtr == pPtr;
    }

    bool operator == (const CSharedPtr<T>& Ptr) const
    {
        return m_pPtr == Ptr.m_pPtr;
    }

    bool operator != (T* pPtr)  const
    {
        return m_pPtr != pPtr;
    }

    bool operator != (const CSharedPtr<T>& Ptr) const
    {
        return m_pPtr != Ptr.m_pPtr;
    }

    // 변환연산자(operator conversion)
    operator T* ()  const
    {
        return m_pPtr;
    }

    T* operator -> ()   const
    {
        return m_pPtr;
    }
};


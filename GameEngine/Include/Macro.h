#pragma once

#define SAFE_DELETE(p)  if(p)   { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p)  if(p)   { delete[] p; p = nullptr; }
#define SAFE_RELEASE(p)  if(p)   { p->Release(); p = nullptr; }

#define DECLARE_SINGLE(Type)    \
private:\
    static Type*    m_pInst;\
public:\
    static Type* GetInst()\
    {\
        if (!m_pInst)\
            m_pInst = new Type;\
        return m_pInst;\
    }\
    static void DestroyInst()\
    {\
        SAFE_DELETE(m_pInst);\
    }\
private:\
    Type();\
    ~Type();

#define DEFINITION_SINGLE(Type) Type* Type::m_pInst = nullptr;

#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()
#define SWAPCHAIN   CDevice::GetInst()->GetSwapChain()

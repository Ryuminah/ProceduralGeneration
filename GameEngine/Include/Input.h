#pragma once

#include "GameEngine.h"

#define DIK_MOUSELBUTTON    0xfc
#define DIK_MOUSERBUTTON    0xfd
#define DIK_MOUSEWHEEL      0xfe

struct KeyState
{
    unsigned char   Key;
    float           PushTime;
    bool            State[KT_End];
    
    KeyState()   :
        State{},
        Key(0),
        PushTime(0.f)
    {
    }
};

struct KeyInfo
{
    std::string     Name;
    KeyState        State;
    std::vector<std::function<void(float)>>  vecCallback[KT_End];
    bool            CtrlState;
    bool            AltState;
    bool            ShiftState;

    KeyInfo()
    {
        CtrlState = false;
        AltState = false;
        ShiftState = false;
    }
};

class CInput
{
private:
    HINSTANCE               m_hInst;
    HWND                    m_hWnd;
    IDirectInput8*          m_pInput;
    IDirectInputDevice8* m_pKeyboard;
    IDirectInputDevice8* m_pMouse;
    Input_Type          m_InputType;
    unsigned char       m_DirectInputKeyResult[256];
    DIMOUSESTATE        m_MouseState;
    bool                m_ControlState;
    bool                m_AltState;
    bool                m_ShiftState;
    Vector2             m_MousePos;
    Vector2             m_MouseMove;

public:
    Vector2 GetMousePos()   const
    {
        return m_MousePos;
    }

    Vector2 GetMouseMove()  const
    {
        return m_MouseMove;
    }

    Vector2 GetMouse2DWorldPos()    const;

private:
    std::vector<KeyState>   m_vecKeyState;
    std::unordered_map<std::string, KeyInfo*>   m_mapInfo;
    std::vector<unsigned int>  m_vecAddIndexKey;

private:
    bool InitWindow();
    bool InitDirectInput();
    void ReadKeyboard();
    void ReadMouse();

public:
    bool Init(HINSTANCE hInst, HWND hWnd);
    void Update(float DeltaTime);

private:
    void UpdateMouse(float DeltaTime);
    void UpdateWindowKeyState(float DeltaTime);
    void UpdateWindowKey(float DeltaTime);
    void UpdateDirectInputKeyState(float DeltaTime);
    void UpdateDirectInputKey(float DeltaTime);

public:
    void ClearCallback();
    bool CreateKey(const std::string& Name, unsigned char Key);
    bool SetControlKey(const std::string& Name, bool State);
    bool SetAltKey(const std::string& Name, bool State);
    bool SetShiftKey(const std::string& Name, bool State);

private:
    KeyInfo* FindKeyInfo(const std::string& Name);

    unsigned char ConvertKey(unsigned char Key);

public:
    template <typename T>
    void AddKeyCallback(const std::string& Name, Key_Type Type, T* pObj, void(T::* pFunc)(float))
    {
        KeyInfo* pInfo = FindKeyInfo(Name);

        if (!pInfo)
            return;

        pInfo->vecCallback[Type].push_back(std::bind(pFunc, pObj, std::placeholders::_1));
    }

    DECLARE_SINGLE(CInput)
};


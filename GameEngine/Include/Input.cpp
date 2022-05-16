#include "Input.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/CameraManager.h"
#include "Component/Camera.h"

DEFINITION_SINGLE(CInput)

CInput::CInput()	:
	m_DirectInputKeyResult{},
	m_ControlState(false),
	m_AltState(false),
	m_ShiftState(false)
{
	// vector resize와 reserve의 차이
	m_vecKeyState.resize(256);

	for(int i = 0; i < 256; ++i)
	{
		m_vecKeyState[i].Key = (unsigned char)i;
	}
}

CInput::~CInput()
{
    {
        auto    iter = m_mapInfo.begin();
        auto    iterEnd = m_mapInfo.end();

        for (; iter != iterEnd; ++iter)
        {
            SAFE_DELETE(iter->second);
        }
    }

    SAFE_RELEASE(m_pKeyboard);
    SAFE_RELEASE(m_pMouse);
    SAFE_RELEASE(m_pInput);
}

Vector2 CInput::GetMouse2DWorldPos() const
{
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	CCameraManager* CameraManager = Scene->GetCameraManager();

	CCamera* Camera = CameraManager->GetCurrentCamera();

	return m_MousePos + Vector2(Camera->GetWorldPos().x, Camera->GetWorldPos().y);
}

bool CInput::InitWindow()
{
    return true;
}

bool CInput::InitDirectInput()
{
    if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
        return false;

    if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
        return false;

    if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
        return false;

    if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
        return false;

	m_vecKeyState.clear();
	m_vecKeyState.resize(256);

	for (int i = 0; i < 256; ++i)
	{
		unsigned char	Key = ConvertKey((unsigned char)i);
		m_vecKeyState[Key].Key = Key;
	}

    return true;
}

void CInput::ReadKeyboard()
{
	HRESULT result = m_pKeyboard->GetDeviceState(256, m_DirectInputKeyResult);

	if (FAILED(result))
	{
		// 장치를 잃어버렸거나 하는 경우 키보드 장치를 다시 습득하도록 한다.
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_pKeyboard->Acquire();
	}
}

void CInput::ReadMouse()
{
	HRESULT result = m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

	if (FAILED(result))
	{
		// 장치를 잃어버렸거나 하는 경우 키보드 장치를 다시 습득하도록 한다.
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_pMouse->Acquire();
	}
}

bool CInput::Init(HINSTANCE hInst, HWND hWnd)
{
    m_hInst = hInst;
    m_hWnd = hWnd;

    HRESULT hr = DirectInput8Create(m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8,
        (void**)&m_pInput, nullptr);

    m_InputType = Input_Type::DirectInput;

    if (FAILED(hr))
        m_InputType = Input_Type::Window;

    switch (m_InputType)
    {
    case Input_Type::Window:
        if (!InitWindow())
            return false;
        break;
    case Input_Type::DirectInput:
        if (!InitDirectInput())
            return false;
        break;
    }

	CreateKey("MouseLButton", VK_LBUTTON);
	CreateKey("MouseRButton", VK_RBUTTON);

    return true;
}

void CInput::Update(float DeltaTime)
{
	UpdateMouse(DeltaTime);

	switch (m_InputType)
	{
	case Input_Type::Window:
		UpdateWindowKey(DeltaTime);
		break;
	case Input_Type::DirectInput:
		UpdateDirectInputKey(DeltaTime);
		break;
	}
}

void CInput::UpdateMouse(float DeltaTime)
{
	POINT	ptMouse;

	GetCursorPos(&ptMouse);

	ScreenToClient(m_hWnd, &ptMouse);

	Vector2	ResolutionRatio = CDevice::GetInst()->GetResolutionRatio();

	Vector2	MousePos;

	MousePos.x = ptMouse.x * ResolutionRatio.x;
	MousePos.y = ptMouse.y * ResolutionRatio.y;

	MousePos.y = CDevice::GetInst()->GetResolution().Height - MousePos.y;

	m_MouseMove = MousePos - m_MousePos;

	m_MousePos = MousePos;
}

void CInput::UpdateWindowKeyState(float DeltaTime)
{
	size_t	Size = m_vecAddIndexKey.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned int	Index = m_vecAddIndexKey[i];

		if (GetAsyncKeyState(m_vecKeyState[Index].Key) & 0x8000)
		{
			m_vecKeyState[Index].PushTime += DeltaTime;
			if (!m_vecKeyState[Index].State[KT_Down] && !m_vecKeyState[Index].State[KT_Push])
			{
				m_vecKeyState[Index].State[KT_Down] = true;
				m_vecKeyState[Index].State[KT_Push] = true;
			}

			else
			{
				m_vecKeyState[Index].State[KT_Down] = false;
			}
		}

		else if (m_vecKeyState[Index].State[KT_Push])
		{
			m_vecKeyState[Index].PushTime = 0.f;
			m_vecKeyState[Index].State[KT_Up] = true;
			m_vecKeyState[Index].State[KT_Down] = false;
			m_vecKeyState[Index].State[KT_Push] = false;
		}

		else if (m_vecKeyState[Index].State[KT_Up])
		{
			m_vecKeyState[Index].PushTime = 0.f;
			m_vecKeyState[Index].State[KT_Up] = false;
		}
	}

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		m_ControlState = true;

	else
		m_ControlState = false;

	if (GetAsyncKeyState(VK_MENU) & 0x8000)
		m_AltState = true;

	else
		m_AltState = false;

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_ShiftState = true;

	else
		m_ShiftState = false;
}

void CInput::UpdateWindowKey(float DeltaTime)
{
	UpdateWindowKeyState(DeltaTime);

	auto	iter = m_mapInfo.begin();
	auto	iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		unsigned int	Index = (unsigned int)iter->second->State.Key;

		iter->second->State.PushTime = m_vecKeyState[Index].PushTime;

		if (m_vecKeyState[Index].State[KT_Down] && m_ControlState == iter->second->CtrlState &&
			m_AltState == iter->second->AltState && m_ShiftState == iter->second->ShiftState)
		{
			size_t	Size = iter->second->vecCallback[KT_Down].size();

			for (size_t i = 0; i < Size; ++i)
			{
				if (iter->second->vecCallback[KT_Down][i])
					iter->second->vecCallback[KT_Down][i](DeltaTime);
			}
		}

		if (m_vecKeyState[Index].State[KT_Push] && m_ControlState == iter->second->CtrlState &&
			m_AltState == iter->second->AltState && m_ShiftState == iter->second->ShiftState)
		{
			size_t	Size = iter->second->vecCallback[KT_Push].size();

			for (size_t i = 0; i < Size; ++i)
			{
				if (iter->second->vecCallback[KT_Push][i])
					iter->second->vecCallback[KT_Push][i](DeltaTime);
			}
		}

		if (m_vecKeyState[Index].State[KT_Up] && m_ControlState == iter->second->CtrlState &&
			m_AltState == iter->second->AltState && m_ShiftState == iter->second->ShiftState)
		{
			size_t	Size = iter->second->vecCallback[KT_Up].size();

			for (size_t i = 0; i < Size; ++i)
			{
				if (iter->second->vecCallback[KT_Up][i])
					iter->second->vecCallback[KT_Up][i](DeltaTime);
			}
		}
	}
}

void CInput::UpdateDirectInputKeyState(float DeltaTime)
{
	size_t	Size = m_vecAddIndexKey.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned int	Index = m_vecAddIndexKey[i];

		unsigned char Key = m_vecKeyState[Index].Key;

		switch (Key)
		{
		case DIK_MOUSELBUTTON:
			if (m_MouseState.rgbButtons[0])
			{
				m_vecKeyState[Index].PushTime += DeltaTime;
				if (!m_vecKeyState[Index].State[KT_Down] && !m_vecKeyState[Index].State[KT_Push])
				{
					m_vecKeyState[Index].State[KT_Down] = true;
					m_vecKeyState[Index].State[KT_Push] = true;
				}

				else
				{
					m_vecKeyState[Index].State[KT_Down] = false;
				}
			}

			else if (m_vecKeyState[Index].State[KT_Push])
			{
				m_vecKeyState[Index].PushTime = 0.f;
				m_vecKeyState[Index].State[KT_Up] = true;
				m_vecKeyState[Index].State[KT_Down] = false;
				m_vecKeyState[Index].State[KT_Push] = false;
			}

			else if (m_vecKeyState[Index].State[KT_Up])
			{
				m_vecKeyState[Index].PushTime = 0.f;
				m_vecKeyState[Index].State[KT_Up] = false;
			}
			break;
		case DIK_MOUSERBUTTON:
			if (m_MouseState.rgbButtons[1])
			{
				m_vecKeyState[Index].PushTime += DeltaTime;
				if (!m_vecKeyState[Index].State[KT_Down] && !m_vecKeyState[Index].State[KT_Push])
				{
					m_vecKeyState[Index].State[KT_Down] = true;
					m_vecKeyState[Index].State[KT_Push] = true;
				}

				else
				{
					m_vecKeyState[Index].State[KT_Down] = false;
				}
			}

			else if (m_vecKeyState[Index].State[KT_Push])
			{
				m_vecKeyState[Index].PushTime = 0.f;
				m_vecKeyState[Index].State[KT_Up] = true;
				m_vecKeyState[Index].State[KT_Down] = false;
				m_vecKeyState[Index].State[KT_Push] = false;
			}

			else if (m_vecKeyState[Index].State[KT_Up])
			{
				m_vecKeyState[Index].PushTime = 0.f;
				m_vecKeyState[Index].State[KT_Up] = false;
			}
			break;
		case DIK_MOUSEWHEEL:
			break;
		default:
			if (m_DirectInputKeyResult[Index] & 0x80)
			{
				m_vecKeyState[Index].PushTime += DeltaTime;
				if (!m_vecKeyState[Index].State[KT_Down] && !m_vecKeyState[Index].State[KT_Push])
				{
					m_vecKeyState[Index].State[KT_Down] = true;
					m_vecKeyState[Index].State[KT_Push] = true;
				}

				else
				{
					m_vecKeyState[Index].State[KT_Down] = false;
				}
			}

			else if (m_vecKeyState[Index].State[KT_Push])
			{
				m_vecKeyState[Index].PushTime = 0.f;
				m_vecKeyState[Index].State[KT_Up] = true;
				m_vecKeyState[Index].State[KT_Down] = false;
				m_vecKeyState[Index].State[KT_Push] = false;
			}

			else if (m_vecKeyState[Index].State[KT_Up])
			{
				m_vecKeyState[Index].PushTime = 0.f;
				m_vecKeyState[Index].State[KT_Up] = false;
			}
			break;
		}
	}

	if (m_DirectInputKeyResult[DIK_LCONTROL] & 0x80)
		m_ControlState = true;

	else
		m_ControlState = false;

	if (m_DirectInputKeyResult[DIK_LALT] & 0x80)
		m_AltState = true;

	else
		m_AltState = false;

	if (m_DirectInputKeyResult[DIK_LSHIFT] & 0x80)
		m_ShiftState = true;

	else
		m_ShiftState = false;
}

void CInput::UpdateDirectInputKey(float DeltaTime)
{
	ReadKeyboard();
	ReadMouse();

	UpdateDirectInputKeyState(DeltaTime);

	auto	iter = m_mapInfo.begin();
	auto	iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		unsigned int	Index = (unsigned int)iter->second->State.Key;

		iter->second->State.PushTime = m_vecKeyState[Index].PushTime;

		if (m_vecKeyState[Index].State[KT_Down] && m_ControlState == iter->second->CtrlState &&
			m_AltState == iter->second->AltState && m_ShiftState == iter->second->ShiftState)
		{
			size_t	Size = iter->second->vecCallback[KT_Down].size();

			for (size_t i = 0; i < Size; ++i)
			{
				if (iter->second->vecCallback[KT_Down][i])
					iter->second->vecCallback[KT_Down][i](DeltaTime);
			}
		}

		if (m_vecKeyState[Index].State[KT_Push] && m_ControlState == iter->second->CtrlState &&
			m_AltState == iter->second->AltState && m_ShiftState == iter->second->ShiftState)
		{
			size_t	Size = iter->second->vecCallback[KT_Push].size();

			for (size_t i = 0; i < Size; ++i)
			{
				if (iter->second->vecCallback[KT_Push][i])
					iter->second->vecCallback[KT_Push][i](DeltaTime);
			}
		}

		if (m_vecKeyState[Index].State[KT_Up] && m_ControlState == iter->second->CtrlState &&
			m_AltState == iter->second->AltState && m_ShiftState == iter->second->ShiftState)
		{
			size_t	Size = iter->second->vecCallback[KT_Up].size();

			for (size_t i = 0; i < Size; ++i)
			{
				if (iter->second->vecCallback[KT_Up][i])
					iter->second->vecCallback[KT_Up][i](DeltaTime);
			}
		}
	}
}

void CInput::ClearCallback()
{
	auto	iter = m_mapInfo.begin();
	auto	iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < KT_End; ++i)
		{
			iter->second->vecCallback[i].clear();
		}
	}
}

bool CInput::CreateKey(const std::string& Name, unsigned char Key)
{
    KeyInfo* pInfo = FindKeyInfo(Name);

    if (pInfo)
        return true;

	unsigned char KeyResult = ConvertKey(Key);

    pInfo = new KeyInfo;

    pInfo->Name = Name;
    pInfo->State.Key = KeyResult;

    m_mapInfo.insert(std::make_pair(Name, pInfo));

	bool	Acc = false;

	size_t	Size = m_vecAddIndexKey.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecAddIndexKey[i] == KeyResult)
		{
			Acc = true;
			break;
		}
	}

	if (!Acc)
		m_vecAddIndexKey.push_back(KeyResult);

    return true;
}

bool CInput::SetControlKey(const std::string& Name, bool State)
{
	KeyInfo* pInfo = FindKeyInfo(Name);

	if (!pInfo)
		return false;

	pInfo->CtrlState = State;

    return true;
}

bool CInput::SetAltKey(const std::string& Name, bool State)
{
	KeyInfo* pInfo = FindKeyInfo(Name);

	if (!pInfo)
		return false;

	pInfo->AltState = State;

	return true;
}

bool CInput::SetShiftKey(const std::string& Name, bool State)
{
	KeyInfo* pInfo = FindKeyInfo(Name);

	if (!pInfo)
		return false;

	pInfo->ShiftState = State;

	return true;
}

KeyInfo* CInput::FindKeyInfo(const std::string& Name)
{
    auto    iter = m_mapInfo.find(Name);

    if (iter == m_mapInfo.end())
        return nullptr;

    return iter->second;
}

unsigned char CInput::ConvertKey(unsigned char Key)
{
    if (m_InputType == Input_Type::DirectInput)
    {
        switch (Key)
        {
        case VK_ESCAPE:
            return DIK_ESCAPE;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case '0':
			return DIK_0;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case '=':
			return DIK_EQUALS;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case 'Q':
			return DIK_Q;
		case 'W':
			return DIK_W;
		case 'E':
			return DIK_E;
		case 'R':
			return DIK_R;
		case 'T':
			return DIK_T;
		case 'Y':
			return DIK_Y;
		case 'U':
			return DIK_U;
		case 'I':
			return DIK_I;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case '[':
			return DIK_LBRACKET;
		case ']':
			return DIK_RBRACKET;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_CONTROL:
			return DIK_LCONTROL;
		case 'A':
			return DIK_A;
		case 'S':
			return DIK_S;
		case 'D':
			return DIK_D;
		case 'F':
			return DIK_F;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		/*case DIK_L:
			return 'L';
		case DIK_SEMICOLON:
			return ';';
		case DIK_APOSTROPHE:
			return '\'';
		case DIK_GRAVE:
			return '`';
		case DIK_LSHIFT:
			return VK_SHIFT;
		case DIK_BACKSLASH:
			return '\\';
		case DIK_Z:
			return 'Z';
		case DIK_X:
			return 'X';
		case DIK_C:
			return 'C';
		case DIK_V:
			return 'V';
		case DIK_B:
			return 'B';
		case DIK_N:
			return 'N';
		case DIK_M:
			return 'M';
		case DIK_COMMA:
			return ',';
		case DIK_PERIOD:
			return '.';
		case DIK_SLASH:
			return '/';
		case DIK_RSHIFT:
			return VK_RSHIFT;
		case DIK_MULTIPLY:
			return VK_MULTIPLY;
		case DIK_LMENU:
			return VK_LMENU;
		case DIK_CAPITAL:
			return VK_CAPITAL;
		case DIK_F1:
			return VK_F1;
		case DIK_F2:
			return VK_F2;
		case DIK_F3:
			return VK_F3;
		case DIK_F4:
			return VK_F4;
		case DIK_F5:
			return VK_F5;
		case DIK_F6:
			return VK_F6;
		case DIK_F7:
			return VK_F7;
		case DIK_F8:
			return VK_F8;
		case DIK_F9:
			return VK_F9;
		case DIK_F10:
			return VK_F10;
		case DIK_NUMLOCK:
			break;
		case DIK_SCROLL:
			break;
		case DIK_NUMPAD7:
			break;
		case DIK_NUMPAD8:
			break;
		case DIK_NUMPAD9:
			break;
		case DIK_SUBTRACT:
			break;
		case DIK_NUMPAD4:
			break;
		case DIK_NUMPAD5:
			break;
		case DIK_NUMPAD6:
			break;
		case DIK_ADD:
			break;
		case DIK_NUMPAD1:
			break;
		case DIK_NUMPAD2:
			break;
		case DIK_NUMPAD3:
			break;
		case DIK_NUMPAD0:
			break;
		case DIK_DECIMAL:
			break;
		case DIK_OEM_102:
			break;
		case DIK_F11:
			return VK_F11;
		case DIK_F12:
			return VK_F12;
		case DIK_F13:
			break;
		case DIK_F14:
			break;
		case DIK_F15:
			break;
		case DIK_NEXTTRACK:
			break;
		case DIK_NUMPADENTER:
			break;
		case DIK_RCONTROL:
			return VK_RCONTROL;
		case DIK_MUTE:
			break;
		case DIK_CALCULATOR:
			break;
		case DIK_PLAYPAUSE:
			break;
		case DIK_MEDIASTOP:
			break;
		case DIK_VOLUMEDOWN:
			break;
		case DIK_VOLUMEUP:
			break;
		case DIK_WEBHOME:
			break;
		case DIK_DIVIDE:
			break;
		case DIK_SYSRQ:
			break;
		case DIK_RMENU:
			return VK_RMENU;
		case DIK_PAUSE:
			break;
		case DIK_HOME:
			return VK_HOME;
		case DIK_UP:
			return VK_UP;
		case DIK_PRIOR:
			return VK_PRIOR;
		case DIK_LEFT:
			return VK_LEFT;
		case DIK_RIGHT:
			return VK_RIGHT;
		case DIK_END:
			return VK_END;
		case DIK_DOWN:
			return VK_DOWN;
		case DIK_NEXT:
			return VK_NEXT;
		case DIK_INSERT:
			return VK_INSERT;
		case DIK_DELETE:
			return VK_DELETE;
		case DIK_LWIN:
			break;
		case DIK_RWIN:
			break;
		case DIK_APPS:
			break;
		case DIK_POWER:
			break;
		case DIK_SLEEP:
			break;
		case DIK_WAKE:
			break;
		case DIK_WEBSEARCH:
			break;
		case DIK_WEBFAVORITES:
			break;
		case DIK_WEBREFRESH:
			break;
		case DIK_WEBSTOP:
			break;
		case DIK_WEBFORWARD:
			break;
		case DIK_WEBBACK:
			break;
		case DIK_MYCOMPUTER:
			break;
		case DIK_MAIL:
			break;
		case DIK_MEDIASELECT:
			break;
		case DIK_MOUSELBUTTON:
			return VK_LBUTTON;
		case DIK_MOUSERBUTTON:
			return VK_RBUTTON;
		case DIK_MOUSEWHEEL:
			return DIK_MOUSEWHEEL;*/
        }

		return 0xff;
    }

    return Key;
}

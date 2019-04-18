#define STRICT

#include "D3DInput.h"

#define MOUSE_BUFF_SIZE 16

CGameController *gThis = NULL;


//Dynamic creating the Input System's object
bool CreateDIInput(CInputInterface **pObj, HWND hwnd, HINSTANCE hInst, bool exclusive)
{
	if (!*pObj) *pObj = new CDirectInputSystem(hwnd, hInst, exclusive);
	else return false;

	return true;
}

//callback function
BOOL gJSEnumDeviceCallBack(const DIDEVICEINSTANCE *inst, void* pData)
{
	return gThis->EnumDeviceCallBack(inst, pData);
}

//Keyboard class
CKeyboard::CKeyboard(LPDIRECTINPUT8 input, HWND hwnd)
{
	// Initialize the keyboard.
	if (input->CreateDevice(GUID_SysKeyboard, &m_device, NULL) == DI_OK)
	{
		if (m_device->SetDataFormat(&c_dfDIKeyboard) == DI_OK)
		{
			if (m_device->SetCooperativeLevel(hwnd,
				DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) == DI_OK)
			{
				m_device->Acquire();
			}
		}
	}

	// Clear keys will clear out the array of keys we have.
	memset(m_keys, 0, sizeof(m_keys));
}

int CKeyboard::ButtonUp(size_t key)
{
	// If the key in the variable is not pressed then return false.
	return (!(m_keys[key] & 0x80) && m_keys[key] != m_oldKeys[key]);
}


int CKeyboard::ButtonDown(size_t key)
{
	// If the key in the variable is not pressed then return false.
	return m_keys[key] & 0x80;
}


POINT CKeyboard::GetPosition()
{
	// Doesn't have position.
	POINT p = { 0, 0 };
	return p;
}


POINT CKeyboard::GetZPosition()
{
	// Doesn't have position.
	POINT p = { 0, 0 };
	return p;
}


bool CKeyboard::UpdateDevice()
{
	if (m_device)
	{
		// Save old state for input comparing.
		memcpy(m_oldKeys, m_keys, sizeof(m_keys));

		// If error getting device state, re-aquire.
		if (FAILED(m_device->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
		{
			if (FAILED(m_device->Acquire())) return false;
			if (FAILED(m_device->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
				return false;
		}
	}

	return true;
}


void CKeyboard::Shutdown()
{
	if (m_device)
	{
		m_device->Unacquire();
		SAFE_RELEASE(m_device);
	}
}

//Mouse class
CMouse::CMouse(LPDIRECTINPUT8 input, HWND hwnd, bool exclusive)
{
	// Initialize the Mouse.
	DWORD flags;

	if (input->CreateDevice(GUID_SysMouse, &m_device, NULL) == DI_OK)
	{
		if (m_device->SetDataFormat(&c_dfDIMouse) == DI_OK)
		{
			if (exclusive) flags = DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
			else flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;

			if (m_device->SetCooperativeLevel(hwnd, flags) == DI_OK)
			{
				m_device->Acquire();
			}
		}
	}
}


int CMouse::ButtonUp(size_t button)
{
	// If the button is not clicked we return false.
	return (!(m_mouseState.rgbButtons[button] & 0x80) &&
		m_mouseState.rgbButtons[button] != m_oldMouseState.rgbButtons[button]);
}


int CMouse::ButtonDown(size_t button)
{
	// If the button is clicked we return true.
	return m_mouseState.rgbButtons[button] & 0x80;
}


bool CMouse::UpdateDevice()
{
	// Get the device state.
	if (m_device)
	{
		// Save old state for input comparing.
		memcpy(&m_oldMouseState, &m_mouseState, sizeof(m_mouseState));

		// If error getting device state, re-aquire.
		if (FAILED(m_device->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState)))
		{
			if (FAILED(m_device->Acquire())) return false;
			if (FAILED(m_device->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState)))
				return false;
		}

		m_xMPos += m_mouseState.lX;
		m_yMPos += m_mouseState.lY;
		m_zMPos = m_mouseState.lZ;
	}

	return true;
}

POINT CMouse::GetPosition()
{
	POINT pos;

	pos.x = m_xMPos;
	pos.y = m_yMPos;
	return pos;
}


POINT CMouse::GetZPosition()
{
	POINT p = { m_zMPos, m_zMPos };
	return p;
}


void CMouse::Shutdown()
{
	if (m_device)
	{
		m_device->Unacquire();
		SAFE_RELEASE(m_device);
	}
}

//Game Controller
CGameController::CGameController(LPDIRECTINPUT8 input, HWND hwnd)
{
	// Save copies.
	gThis = this;
	m_hwnd = hwnd;
	m_inputSystem = input;

	// Initialize the game controller.
	DIPROPRANGE range;
	DIDEVCAPS caps;

	//负责创建设备
	m_inputSystem->EnumDevices(DI8DEVCLASS_GAMECTRL,  //限制搜索某一类型的设备筛选类型
		(LPDIENUMDEVICESCALLBACK)gJSEnumDeviceCallBack,//创建设备而被调用的回调函数的地址
		NULL, 		//想要传给该回调函数的数值
		DIEDFL_ATTACHEDONLY);			//指定设备枚举方式的标示符

	if (m_device)
	{
		range.diph.dwSize = sizeof(DIPROPRANGE);
		range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		range.diph.dwHow = DIPH_BYOFFSET;
		range.lMin = -1000;
		range.lMax = 1000;
		range.diph.dwObj = DIJOFS_X;
		m_device->SetProperty(DIPROP_RANGE, &range.diph);
		range.diph.dwObj = DIJOFS_Y;
		m_device->SetProperty(DIPROP_RANGE, &range.diph);

		if (SUCCEEDED(m_device->GetCapabilities(&caps))) m_numButtons = caps.dwButtons;
		else m_numButtons = 4;
	}
}


BOOL CGameController::EnumDeviceCallBack(const DIDEVICEINSTANCE *inst, void* pData)
{
	// Set to the first device found.
	if (SUCCEEDED(m_inputSystem->CreateDevice(inst->guidInstance, &m_device, NULL)))
	{
		if (SUCCEEDED(m_device->SetDataFormat(&c_dfDIJoystick2)))
			if (SUCCEEDED(m_device->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
				if (SUCCEEDED(m_device->Acquire()))
				{
					strcpy_s(m_name, (char*)inst->tszProductName);
					return DIENUM_STOP;
				}
	}

	// Return continue to try to init other connected devices.
	return DIENUM_CONTINUE;
}


int CGameController::ButtonUp(size_t button)
{
	if (button < 0 || button >= m_numButtons) return 0;
	return (!(m_gcState.rgbButtons[button] & 0x80) &&
		m_gcState.rgbButtons[button] != m_oldGCState.rgbButtons[button]);
}


int CGameController::ButtonDown(size_t button)
{
	if (button < 0 || button >= m_numButtons) return 0;
	return m_gcState.rgbButtons[button] & 0x80;
}


bool CGameController::UpdateDevice()
{
	if (m_device)
	{
		m_device->Poll();

		// Save old state for input comparing.
		memcpy(&m_oldGCState, &m_gcState, sizeof(m_gcState));

		// If error getting device state, re-aquire.
		if (FAILED(m_device->GetDeviceState(sizeof(DIJOYSTATE2), &m_gcState)))
		{
			if (FAILED(m_device->Acquire())) return false;
			if (FAILED(m_device->GetDeviceState(sizeof(DIJOYSTATE2), &m_gcState)))
				return false;
		}

		m_xGCPos = m_gcState.lX;
		m_yGCPos = m_gcState.lY;

		m_xGCPos2 = m_gcState.lZ;
		m_yGCPos2 = m_gcState.lRz;
	}

	return true;
}


POINT CGameController::GetPosition()
{
	POINT pos;

	pos.x = m_xGCPos;
	pos.y = m_yGCPos;
	return pos;
}


POINT CGameController::GetZPosition()
{
	POINT pos;

	pos.x = m_xGCPos2;
	pos.y = m_yGCPos2;
	return pos;
}


void CGameController::Shutdown()
{
	if (m_device)
	{
		m_device->Unacquire();
		SAFE_RELEASE(m_device);
	}
}

//InputSystem class
CDirectInputSystem::CDirectInputSystem(HWND hwnd, HINSTANCE hInst, bool exclusive)
{
	// Initialize objects...
	m_keyboard = NULL;
	m_mouse = NULL;
	m_gameController = NULL;

	// Create input system.
	if (DirectInput8Create(hInst, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void **)&m_system, NULL) == DI_OK)
	{
		m_keyboard = new CKeyboard(m_system, hwnd);
		m_mouse = new CMouse(m_system, hwnd, exclusive);
		m_gameController = new CGameController(m_system, hwnd);
	}
}


CDirectInputSystem::~CDirectInputSystem()
{
	// Shut everything down.
	Shutdown();
}


bool CDirectInputSystem::Initialize()
{
	// Everything took place in the constructor.
	// Force keyboard and mouse before considering the
	// system initialized.
	return (m_keyboard && m_mouse);
}


bool CDirectInputSystem::UpdateDevices()
{
	int hr;

	// Get the device state.
	if (m_mouse) hr = m_mouse->UpdateDevice();
	if (m_keyboard) hr = m_keyboard->UpdateDevice();
	if (m_gameController) hr = m_gameController->UpdateDevice();

	return true;
}


int CDirectInputSystem::KeyUp(size_t key)
{
	if (!m_keyboard) return 0;
	return m_keyboard->ButtonUp(key);
}


int CDirectInputSystem::KeyDown(size_t key)
{
	if (!m_keyboard) return 0;
	return m_keyboard->ButtonDown(key);
}


int CDirectInputSystem::MouseButtonUp(size_t button)
{
	if (!m_mouse) return 0;
	return m_mouse->ButtonUp(button);
}


int CDirectInputSystem::MouseButtonDown(size_t button)
{
	if (!m_mouse) return 0;
	return m_mouse->ButtonDown(button);
}

float CDirectInputSystem::mouseDX()
{
	if (!m_mouse) return 0;
	return m_mouse->GetPosition().x;
}

float CDirectInputSystem::mouseDY()
{
	if (!m_mouse) return 0;
	return m_mouse->GetPosition().y;
}

float CDirectInputSystem::mouseDZ()
{
	if (!m_mouse)return 0;
	return m_mouse->GetZPosition().y;
}

int CDirectInputSystem::ControllerButtonUp(size_t button)
{
	if (!m_gameController) return 0;
	return m_gameController->ButtonUp(button);
}


int CDirectInputSystem::ControllerButtonDown(size_t button)
{
	if (!m_gameController) return 0;
	return m_gameController->ButtonDown(button);
}


POINT CDirectInputSystem::GetMousePos()
{
	POINT null = { 0, 0 };
	if (!m_mouse) return null;

	return m_mouse->GetPosition();
}


long CDirectInputSystem::GetMouseWheelPos()
{
	if (!m_mouse) return 0;

	POINT wheel = m_mouse->GetZPosition();
	return wheel.y;
}


POINT CDirectInputSystem::GetLeftStickPos()
{
	POINT null = { 0, 0 };
	if (!m_gameController) return null;

	return m_gameController->GetPosition();
}


POINT CDirectInputSystem::GetRightStickPos()
{
	POINT null = { 0, 0 };
	if (!m_gameController) return null;

	return m_gameController->GetZPosition();
}


void CDirectInputSystem::Shutdown()
{
	// Delete each object...
	if (m_keyboard)
	{
		m_keyboard->Shutdown();
		SAFE_DELETE(m_keyboard);
	}

	if (m_mouse)
	{
		m_mouse->Shutdown();
		SAFE_DELETE(m_mouse);
	}

	if (m_gameController)
	{
		m_gameController->Shutdown();
		SAFE_DELETE(m_gameController);
	}

	if (m_system)
	{
		SAFE_RELEASE(m_system);
	}
}
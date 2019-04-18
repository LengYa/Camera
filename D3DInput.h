#pragma once
#ifndef D3DINPUT_H_
#define D3DINPUT_H_

#define DIRECTINPUT_VERSION 0X0800

#include "Interface.h"
#include <dinput.h>


#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")



//------------------------------------------------------------------------------
//Mouse	and KeyBoard
//------------------------------------------------------------------------------
#define LEFT_BUTTON		0
#define RIGHT_BUTTON	1
#define KEYS_SIZE		256

class CKeyboard : public CDeviceInterface
{
public:
	CKeyboard(LPDIRECTINPUT8 input, HWND hwnd);
	~CKeyboard() { Shutdown(); }

	bool UpdateDevice();

	int ButtonUp(size_t key);
	int ButtonDown(size_t key);

	POINT GetPosition();
	POINT GetZPosition();

	void Shutdown();

	// Keyboard device.
	LPDIRECTINPUTDEVICE8 m_device;
	char m_keys[KEYS_SIZE];
	char m_oldKeys[KEYS_SIZE];
};


class CMouse : public CDeviceInterface
{
public:
	CMouse(LPDIRECTINPUT8 input, HWND hwnd, bool exclusive);
	~CMouse() { Shutdown(); }

	bool UpdateDevice();

	int ButtonUp(size_t key);
	int ButtonDown(size_t key);

	POINT GetPosition();
	POINT GetZPosition();

	void Shutdown();

	// Mouse device.
	LPDIRECTINPUTDEVICE8 m_device;
	DIMOUSESTATE m_mouseState;
	DIMOUSESTATE m_oldMouseState;
	bool m_button[3];

	// Mouse x, y, and wheel position.
	long m_xMPos;
	long m_yMPos;
	long m_zMPos;

	// Max x (width) and y (height)
	// Restrict means to cap at width/height.
	bool m_restrict;
	int m_width;
	int m_height;
};


//游戏控制器设备，如手柄等
class CGameController : public CDeviceInterface
{
public:
	CGameController(LPDIRECTINPUT8 input, HWND hwnd);
	~CGameController() { Shutdown(); }

	bool UpdateDevice();

	// Used to create the game controllers.
	BOOL EnumDeviceCallBack(const DIDEVICEINSTANCE *inst, void* pData);

	int ButtonUp(size_t key);
	int ButtonDown(size_t key);

	POINT GetPosition();
	POINT GetZPosition();

	void Shutdown();

	// Game controller device.
	LPDIRECTINPUTDEVICE8 m_device;
	DIJOYSTATE2 m_gcState;
	DIJOYSTATE2 m_oldGCState;
	char m_name[256];
	unsigned long m_numButtons;

	// Left and right stick x and y positions.
	long m_xGCPos;
	long m_yGCPos;
	long m_xGCPos2;
	long m_yGCPos2;

	// Window handled (needed for game controllers).
	HWND m_hwnd;

	// Copy of input system.
	LPDIRECTINPUT8 m_inputSystem;
};


//为支持输入设备系统提供服务
class CDirectInputSystem : public CInputInterface
{
public:
	CDirectInputSystem(HWND hwnd, HINSTANCE hInst, bool exclusive);
	~CDirectInputSystem();

	// Init devics, update devices states.
	bool Initialize();
	bool UpdateDevices();

	// Keyboard functions.
	int KeyUp(size_t key);
	int KeyDown(size_t key);

	// Mouse functions.
	int MouseButtonUp(size_t button);
	int MouseButtonDown(size_t button);
	float mouseDX();
	float mouseDY();
	float mouseDZ();

	// Get mouse position (x, y) and mouse wheel data (z).
	POINT GetMousePos();
	long GetMouseWheelPos();

	// Game controller functions.
	int ControllerButtonUp(size_t button);
	int ControllerButtonDown(size_t button);

	// Get controller main (left) and right stick position.
	POINT GetLeftStickPos();
	POINT GetRightStickPos();

	void Shutdown();

protected:
	LPDIRECTINPUT8 m_system;

	// Devices.	Use the classes already were created
	CKeyboard *m_keyboard;
	CMouse *m_mouse;
	CGameController *m_gameController;
};

bool CreateDIInput(CInputInterface **pObj, HWND hwnd, HINSTANCE hInst, bool exclusive);

#endif

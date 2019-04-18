#pragma once
#ifndef INPUT_INTERFACE_H_
#define INPUT_INTERFACE_H_

#include <d3d9.h> 
#include <d3dx9.h>
#include "Utility.h"

class CInputInterface
{
public:
	CInputInterface() {}
	virtual ~CInputInterface() {}

	virtual bool Initialize() = 0;
	virtual bool UpdateDevices() = 0;

	virtual int KeyUp(size_t key) = 0;
	virtual int KeyDown(size_t key) = 0;

	virtual int MouseButtonUp(size_t button) = 0;
	virtual int MouseButtonDown(size_t button) = 0;
	virtual POINT GetMousePos() = 0;
	virtual long GetMouseWheelPos() = 0;
	virtual float mouseDX() = 0;
	virtual float mouseDY() = 0;
	virtual float mouseDZ() = 0;

	virtual int ControllerButtonUp(size_t button) = 0;
	virtual int ControllerButtonDown(size_t button) = 0;
	virtual POINT GetLeftStickPos() = 0;
	virtual POINT GetRightStickPos() = 0;

	virtual void Shutdown() = 0;
};


class CDeviceInterface
{
public:
	CDeviceInterface() {}
	virtual ~CDeviceInterface() {}

	virtual bool UpdateDevice() = 0;

	virtual int ButtonUp(size_t key) = 0;
	virtual int ButtonDown(size_t key) = 0;

	virtual POINT GetPosition() = 0;
	virtual POINT GetZPosition() = 0;

	virtual void Shutdown() = 0;
};

#endif

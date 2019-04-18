#ifndef STRICT
#define STRICT
#endif // !STRICT

#include "Game.h"

//Variables
IDirect3DDevice9			*g_Device = NULL;

HINSTANCE					 g_hInstance = NULL;

HWND						 g_hwnd = NULL;

bool						 g_LMBDown = FALSE;				//Mouse's left

int							 g_MouseX = 0, g_MouseY = 0;

GameState					g_State = NORMAL;

D3DXMATRIX					g_projection;			//投影矩阵 
D3DXMATRIX					g_view;					//视图矩阵

Camera					   *g_Camera = NULL;

Camera						*pTCamera = NULL;

CSkyModel					*sky_model = NULL;

CInputInterface				*g_Input = NULL;

float g_StartTime;


float g_PosZ = -300.0f;
const float g_speedZ = 50.0f;


int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;


//////////////////////////////////////////////////////////////////////////
//Name:bool Game_Init()
//Describe:Initialize the Game
//////////////////////////////////////////////////////////////////////////
bool Game_Init()
{
	//Initialize the Window
	if (!Init_D3D(D3DDEVTYPE_HAL))
	{
		::MessageBox(NULL, L"Initialize Direct3D Failed", L"Error", MB_OK);
		return FALSE;
	}

	//Initialize the input system
	//create the input system's object
	if (!CreateDIInput(&g_Input, g_hwnd, g_hInstance, false))
	{
		::MessageBox(NULL, L"Failed to create the InputSystem", L"Error", MB_OK);
		return false;
	}

	if (!g_Input->Initialize())
	{
		::MessageBox(NULL, L"Failed to initialize the InputSystem's object", L"Error", MB_OK);
		return false;
	}

	g_Camera = new Camera();
	g_Camera->pos() = D3DXVECTOR3(-50.0f, 50.0f, -80.0f);
	g_Camera->SetSpeed(300.0f);

	sky_model = new CSkyModel(g_Device);
	sky_model->Init_SkyBox(50000);

	WCHAR frontTexture[50] = L"d1.jpg";
	WCHAR backTexture[50] = L"d2.jpg";
	WCHAR leftTextre[50] = L"d3.jpg";
	WCHAR rightTexture[50] = L"d4.jpg";
	WCHAR topTexture[50] = L"d5.jpg";
	WCHAR bottomTexture[50] = L"d6.jpg";

	bool flag = false;
	sky_model->Init_SkyBox(50000);
	flag = sky_model->Load_SkyTextureFromFile(frontTexture, backTexture, leftTextre, \
		rightTexture, topTexture, bottomTexture);
	if (!flag)
	{
		::MessageBox(NULL, L"Failed to open the file", L"Tips", MB_OK);
		return false;
	}

	pTCamera = g_Camera;
	//Set the default world's matrix
	D3DXMATRIX    g_matWorld;
	D3DXMatrixTranslation(&g_matWorld, 0.0f, 0.0f, 0.0f);
	g_Device->SetTransform(D3DTS_WORLD, &g_matWorld);

	//Set the default matrix of projection
	//Set the default projection's matrix
	D3DXMatrixPerspectiveFovLH(&g_projection, D3DX_PI / 4.0f,
		WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 200000.0F);
	g_Device->SetTransform(D3DTS_PROJECTION, &g_projection);

	return true;
}

//////////////////////////////////////////////////////////////////////////
//Name:void Game_Main(float fTimeDelta)
//Describe:the Game's main loop
//////////////////////////////////////////////////////////////////////////
void Game_Main(float fTimeDelta)
{
	D3D_Update(fTimeDelta);
	D3D_Render(fTimeDelta);
}

//////////////////////////////////////////////////////////////////////////
//Name:void Game_Shutdown()
//Describe:the game is over ,so release the all sources
//////////////////////////////////////////////////////////////////////////
void Game_Shutdown()
{
}


//////////////////////////////////////////////////////////////////////////
//Name:void D3D_Update(float fTimeDelta)
//Describe:on-time Update the game's date
//////////////////////////////////////////////////////////////////////////
void D3D_Update(float fTimeDelta)
{
	Updata_Input(fTimeDelta);
	//update the view matrix
	D3DXVECTOR3 v(0, 0, 0);
	//g_Camera->UpdateT(fTimeDelta,0,v,0, 0, 0);
	g_view = g_Camera->viewproj();
	g_Device->SetTransform(D3DTS_VIEW, &g_view);

	//save the view port
	D3DVIEWPORT9 MainViewport;
	g_Device->GetViewport(&MainViewport);
	g_Device->SetViewport(&MainViewport);
}

//////////////////////////////////////////////////////////////////////////
//Name:void D3D_Render(float fTimeDelta)
//Describe:on-time Render the Game
//////////////////////////////////////////////////////////////////////////
void D3D_Render(float fTimeDelta)
{
	//1.Clear the Screen
	g_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xff666666, 1.0F, 0);//D3DCOLOR_XRGB(100, 255, 255), 1.0f, 0);


	//2.Start BeginScene
	g_Device->BeginScene();

	D3DXMATRIX matSky, matTransSky, matRotSky;
	D3DXMatrixTranslation(&matTransSky, 0.0F, -12500, -800.0F);
	D3DXMatrixRotationY(&matRotSky, -0.00002f*timeGetTime());   //旋转天空网格, 简单模拟云彩运动效果
	matSky = matTransSky * matRotSky;
	sky_model->Render_SkyBox(&matSky, FALSE);
	g_Device->EndScene();

	g_Device->Present(NULL, NULL, NULL, NULL);  // 翻转与显示
}

void Updata_Input(float fTimeDelta)
{
	if (!g_Input) return;

	// Update all devices.
	g_Input->UpdateDevices();

	//Move the camera
	D3DXMATRIX g_ViewMatrix;			//观察，相机坐标
	D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 _Look = g_Camera->look();
	D3DXVECTOR3 _Right = g_Camera->right();

	short int state = 0;
	float angle = 0.0f;

	// Keyboard Input.
	if (g_Input->KeyDown(DIK_W))
	{
		dir += _Look;
	}
	else if (g_Input->KeyDown(DIK_S))
	{
		dir -= _Look;
	}
	else if (g_Input->KeyDown(DIK_D))
	{
		dir += _Right;
	}
	else if (g_Input->KeyDown(DIK_A))
	{
		dir -= _Right;
	}
	else if (g_Input->KeyDown(DIK_LEFT))
	{
		state = 0;
		angle = -0.003f;
	}
	else if (g_Input->KeyDown(DIK_RIGHT))
	{
		state = 0;
		angle = 0.003f;
	}
	else if (g_Input->KeyDown(DIK_UP))
	{
		state = 1;
		angle = -0.003f;
	}
	else if (g_Input->KeyDown(DIK_DOWN))
	{
		state = 1;
		angle = 0.003f;
	}
	else if (g_Input->KeyDown(DIK_Q))
	{
		state = 2;
		angle = -0.003f;
	}
	else if (g_Input->KeyDown(DIK_E))
	{
		state = 2;
		angle = 0.003f;
	}
	g_Camera->update(fTimeDelta, dir, state, angle);
}

//////////////////////////////////////////////////////////////////////////
// Name:Init_D3D()
// Describe:Initialize D3D window
//////////////////////////////////////////////////////////////////////////
bool Init_D3D(D3DDEVTYPE deviceType)
{
	HWND hwnd;

	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hInstance;
	wc.hIcon = (HICON)LoadImage(0, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(g_hInstance, NULL);
	wc.hIconSm = (HICON)LoadImage(0, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = WINDOW_TITLE;

	if (!RegisterClassEx(&wc))
	{
		::MessageBox(0, L"RegisterClass() - FAILED", 0, 0);
		return false;
	}

	hwnd = ::CreateWindowEx(NULL, wc.lpszClassName, wc.lpszClassName,
		WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		0 /*parent hwnd*/, 0 /* menu */, g_hInstance, 0 /*extra*/);

	if (!hwnd)
	{
		::MessageBox(0, L"CreateWindow() - FAILED", 0, 0);
		return false;
	}
	g_hwnd = hwnd;

	::ShowWindow(hwnd, SW_SHOW);

	::UpdateWindow(hwnd);

	// disable CTRL-ALT_DEL, ALT_TAB, comment this line out 
	// if it causes your system to crash
	SystemParametersInfo(SPI_SCREENSAVERRUNNING, TRUE, NULL, 0);

	//
	// Init D3D: 
	//

	HRESULT hr = 0;

	// Step 1: Create the IDirect3D9 object.
	int mode;					//Set the full screen or windowed

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		::MessageBox(0, L"Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}

	// Step 2: Check for hardware vp.

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
	d3d9->GetAdapterCount();
	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	mode = MessageBoxW(NULL, L"Use FullScreen mode?", L"Tips", MB_YESNO | MB_ICONQUESTION);

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.

	D3DPRESENT_PARAMETERS d3dpp;

	if (mode == IDYES)
	{
		d3dpp.BackBufferWidth = WINDOW_WIDTH;
		d3dpp.BackBufferHeight = WINDOW_HEIGHT;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount = 2;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = FALSE;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.Flags = 0;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
	{
		d3dpp.BackBufferWidth = WINDOW_WIDTH;
		d3dpp.BackBufferHeight = WINDOW_HEIGHT;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount = 2;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = TRUE;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.Flags = 0;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	// Step 4: Create the device.

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		hwnd,               // window associated with device
		vp,                 // vertex processing
		&d3dpp,             // present parameters
		&g_Device);            // return created device

	if (FAILED(hr))
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hwnd,
			vp,
			&d3dpp,
			&g_Device);

		if (FAILED(hr))
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, L"CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}

	//Step 5: Reset the lose device
	HRESULT _result = g_Device->TestCooperativeLevel();	  //query the lost device
	while (_result == D3DERR_DEVICELOST)
	{
		while (_result != D3DERR_DEVICENOTRESET)
		{
			Sleep(3000);
			_result = g_Device->TestCooperativeLevel();
		}
		if (FAILED(g_Device->Reset(&d3dpp)))
		{
			_result = D3DERR_DEVICELOST;
		}
	}

	//Step 6
	d3d9->Release(); // done with d3d9 object

	//Step 7 (Addition) Check the device's vertex shader
	D3DCAPS9 vcaps;
	HR(g_Device->GetDeviceCaps(&vcaps));

	// Check for vertex shader version 2.0 support.
	if (vcaps.VertexShaderVersion < D3DVS_VERSION(2, 0))
	{
		::MessageBox(NULL, L"Do not support the version of vertex shader", L"Tips", S_OK);
		return false;
	}

	// Check for pixel shader version 2.0 support.
	if (vcaps.PixelShaderVersion < D3DPS_VERSION(2, 0))
	{
		::MessageBox(NULL, L"Do not support the version of pixel shader", L"Tips", S_OK);
		return false;
	}

	return true;
}

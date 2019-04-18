#pragma once
#ifndef GAME_H
#define GAME_H

#include "Utility.h"
#include "Camera.h"
#include "Sky_Model.h"
#include "D3DInput.h"


//////////////////////////////////////////////////////////////////////////
//Set the state of game
//////////////////////////////////////////////////////////////////////////
enum GameState
{
	VICITORY = 0,
	DEFEAT,
	NORMAL,
	NORTH,
	WEST,
	SOUTH,
	EAST
};


//////////////////////////////////////////////////////////////////////////
//Main game and input functions
//////////////////////////////////////////////////////////////////////////
bool Game_Init();
void Game_Main(float fTimeDelta);
void Game_Shutdown();
bool Init_D3D(D3DDEVTYPE deviceType);
void D3D_Update(float fTimeDelta);
void D3D_Render(float fTimeDelta);

void Updata_Input(float fTimeDelta);

//////////////////////////////////////////////////////////////////////////
//Callback functions
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


///////////////////////////////////////////////////////////////////////////
//Global variables
//////////////////////////////////////////////////////////////////////////
extern IDirect3DDevice9			*g_Device;

extern HINSTANCE				 g_hInstance;

extern HWND						 g_hwnd;

extern CInputInterface			*g_Input;

extern bool						g_LMBDown;				//Mouse's left

extern int						g_MouseX, g_MouseY;

extern CSkyModel			   *sky_model;
#endif // !GAME_H
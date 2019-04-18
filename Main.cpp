#ifndef STRICT
#define STRICT
#endif // !STRICT

#define WIN32_LEAN_AND_MEAN

#include "Game.h"

bool Paused = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_CREATE:
		return 0;
		break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);

		ValidateRect(hwnd, NULL);				//Update the client
		EndPaint(hwnd, &ps);
		return 0;
	}
	break;

	case WM_KEYDOWN:
	{
		short exit = 0;
		if (wParam == VK_ESCAPE)
		{
			exit = MessageBoxW(hwnd, L"Are you sure exit?", L"Tips", MB_YESNO | MB_ICONQUESTION);
			if (exit == IDYES)
			{
				Game_Shutdown();
				::PostQuitMessage(0);
				return 0;
			}
		}
	}
	break;

	case WM_DESTROY:
	{
		Game_Shutdown();
		::PostQuitMessage(0);
		return 0;
	}
	break;
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	MSG msg;

	//Enable run-time memory check for debug builds
#if defined(DEBUG)|defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	g_hInstance = hInstance;

	srand(time(0));

	if (Game_Init())
	{
		__int64 cntsPerSec = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
		float secsPerCnt = 1.0f / (float)cntsPerSec;

		__int64 prevTimeStamp = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

		::ZeroMemory(&msg, sizeof(MSG));

		while (true)
		{
			/*static FLOAT fLastTime = (float)::timeGetTime();
			static FLOAT fCurrTime = (float)::timeGetTime();
			static FLOAT fTimeDelta = 0.0f;
			fCurrTime = (float)::timeGetTime();
			fTimeDelta = (fCurrTime - fLastTime) * 0.001f;
			fLastTime = fCurrTime;*/

			if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
			{
				//test if this is a quit
				if (msg.message == WM_QUIT)
				{
					break;
				}
				//translate any accelerator keys
				TranslateMessage(&msg);

				//send the message to the window proc
				DispatchMessageW(&msg);
			}//end if
			else
			{
				__int64 currTimeStamp = 0;
				QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
				float dt = (currTimeStamp - prevTimeStamp)*secsPerCnt;

				if (Paused)
				{
					Sleep(500);
					continue;
				}
				else
				{
					Game_Main(dt);
					prevTimeStamp = currTimeStamp;
				}
			}
		}
	}

	Game_Shutdown();

	UnregisterClass(WINDOW_TITLE, hInstance);
	return 0;
}

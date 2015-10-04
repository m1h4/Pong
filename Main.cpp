#include "Globals.h"
#include "Main.h"
#include "Resource.h"

#define POD_MARGIN 2.f
#define POD_WIDTH 10.f
#define POD_HEIGHT 60.f
#define POD_SPEED 8.f
#define BALL_WIDTH 10.f
#define BALL_HEIGHT 10.f

FLOAT pod1 = -1.f;
FLOAT pod2 = -1.f;
FLOAT ballx = -1.f;
FLOAT bally = -1.f;
FLOAT ballvx = 8.f;
FLOAT ballvy = 8.f;

LARGE_INTEGER	g_TimerLast;
LARGE_INTEGER	g_TimerCurrent;
LARGE_INTEGER	g_TimerFrequency;
DOUBLE			g_TimerElapsed;

INT WinServer(HINSTANCE hInstance)
{
	return 0;
}

INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR cmdLine,INT cmdShow)
{
	if(!lstrcmpA(cmdLine,"/server"))
		return WinServer(hInstance);

	TCHAR application[512];
	LoadString(GetModuleHandle(NULL),IDS_APPLICATION,application,sizeof(application)/sizeof(TCHAR));

	WNDCLASSEX wcex;
	ZeroMemory(&wcex,sizeof(wcex));
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_CLASSDC|CS_DBLCLKS;
	wcex.lpfnWndProc	= WinProcedure;
	wcex.hInstance		= GetModuleHandle(NULL);
	wcex.hIcon			= LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_MAIN));
	wcex.hIconSm		= LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_MAIN));
	wcex.hCursor		= LoadCursor(NULL,IDC_ARROW);
	wcex.lpszClassName	= application;

	if(!RegisterClassEx(&wcex))
	{
		TRACE(L"Error: Failed to register window class.\n");
		return 1;
	}

	HACCEL hAccelerators = LoadAccelerators(GetModuleHandle(NULL),MAKEINTRESOURCE(IDA_MAIN));
	if(!hAccelerators)
	{
		TRACE(L"Error: Failed to load accelerators.\n");
		return 1;
	}

	//HMENU hMenu = LoadMenu(GetModuleHandle(NULL),MAKEINTRESOURCE(IDM_MAIN));
	//if(!hMenu)
	//{
	//	TRACE(L"Error: Failed to load menu.\n");
	//	return 1;
	//}

	HWND hWnd = CreateWindowEx(NULL,application,application,WS_OVERLAPPEDWINDOW|WS_VISIBLE|WS_CLIPCHILDREN,CW_USEDEFAULT,CW_USEDEFAULT,MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT,GetDesktopWindow(),/*hMenu*/ NULL,GetModuleHandle(NULL),NULL);
	if(!hWnd)
	{
		TRACE(L"Error: Failed to create window.\n");
		return 1;
	}

	MSG msg;
	ZeroMemory(&msg,sizeof(msg));

	while(GetMessage(&msg,NULL,NULL,NULL))
	{
		if(!TranslateAccelerator(hWnd,hAccelerators,&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	DestroyWindow(hWnd);
	UnregisterClass(application,GetModuleHandle(NULL));

	return (INT)msg.wParam;
}

LRESULT WINAPI WinProcedure(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
			return WinCreate(hWnd) ? 0 : -1;

		case WM_DESTROY:
			return WinDestroy(hWnd) ? 0 : -1;

		case WM_CLOSE:
			return WinClose(hWnd) ? 0 : -1;

		case WM_COMMAND:
			return WinCommand(hWnd,wParam,lParam) ? 0 : -1;

		case WM_CONTEXTMENU:
			return WinContextMenu(hWnd,wParam,lParam) ? 0 : -1;

		case WM_LBUTTONDOWN:
			return WinButtonDown(hWnd,msg,LOWORD(lParam),HIWORD(lParam)) ? 0 : -1;

		case WM_PAINT:
			return WinPaint(hWnd) ? 0 : -1;

		case WM_TIMER:
			return WinTimer(hWnd,(UINT)wParam) ? 0 : -1;
	}

	return DefWindowProc(hWnd,msg,wParam,lParam);
}
BOOL WinCreate(HWND hWnd)
{
	QueryPerformanceFrequency(&g_TimerFrequency);
	QueryPerformanceCounter(&g_TimerCurrent);
	g_TimerLast = g_TimerCurrent;
	g_TimerElapsed = 0.0;

	SetTimer(hWnd,NULL,10,NULL);

	return TRUE;
}

DOUBLE GetElapsedTime(VOID)
{
	return g_TimerElapsed;
}

BOOL WinDestroy(HWND hWnd)
{
	KillTimer(hWnd,NULL);

	return TRUE;
}

BOOL WinTimer(HWND hWnd,UINT eventId)
{
	QueryPerformanceCounter(&g_TimerCurrent);
	g_TimerElapsed = (g_TimerCurrent.QuadPart - g_TimerLast.QuadPart) / (DOUBLE)g_TimerFrequency.QuadPart;
	g_TimerElapsed *= 30.0;
	g_TimerLast = g_TimerCurrent;

	InvalidateRect(hWnd,NULL,FALSE);

	ballx += ballvx * GetElapsedTime();
	bally += ballvy * GetElapsedTime();

	RECT client;
	GetClientRect(hWnd,&client);

	if(bally + BALL_HEIGHT / 2.f >= client.bottom - client.top)
	{
		bally = client.bottom - client.top - BALL_HEIGHT / 2.f;
		ballvy = -ballvy;
	}
	else if(bally - BALL_HEIGHT / 2.f <= 0.f)
	{
		bally = BALL_HEIGHT / 2.f;
		ballvy = -ballvy;
	}

	if(ballx + BALL_WIDTH / 2.f >= client.right - client.left - POD_MARGIN)
	{
		if(bally - BALL_HEIGHT / 2.f >= pod2 - POD_HEIGHT / 2.f && bally + BALL_HEIGHT / 2.f <= pod2 + POD_HEIGHT / 2.f)
		{
			ballx = client.right - client.left - BALL_WIDTH / 2.f - POD_MARGIN;
			ballvx = -ballvx;
		}
		else
		{
			ballx = -1.f;
			bally = -1.f;
			pod1 = -1.f;
			pod2 = -1.f;
		}
	}
	else if(ballx - BALL_WIDTH / 2.f <= POD_MARGIN)
	{
		if(bally - BALL_HEIGHT / 2.f >= pod1 - POD_HEIGHT / 2.f && bally + BALL_HEIGHT / 2.f <= pod1 + POD_HEIGHT / 2.f)
		{
			ballx = BALL_WIDTH / 2.f;
			ballvx = -ballvx;
		}
		else
		{
			ballx = -1.f;
			bally = -1.f;
			pod1 = -1.f;
			pod2 = -1.f;
		}
	}

	FLOAT right;
	
	if(ballvx > 0.f)
		right = (client.right - client.left - ballx - POD_MARGIN) / ballvx * ballvy + bally;
	else
		right = pod2;

	FLOAT left;
	
	if(ballvx < 0.f)
		left = (ballx - POD_MARGIN) / ballvx * -ballvy + bally;
	else
		left = pod1;

	if(pod2 < right)
		pod2 += POD_SPEED * GetElapsedTime();
	else if(pod2 > right)
		pod2 -= POD_SPEED * GetElapsedTime();

	if(pod2 - POD_HEIGHT / 2.f <= POD_MARGIN)
		pod2 = POD_MARGIN + POD_HEIGHT / 2.f;
	else if(pod2 + POD_HEIGHT / 2.f >= client.bottom - client.top - POD_MARGIN)
		pod2 = client.bottom - client.top - POD_MARGIN - POD_HEIGHT / 2.f;

	if(pod1 < left)
		pod1 += POD_SPEED * GetElapsedTime();
	else if(pod1 > left)
		pod1 -= POD_SPEED * GetElapsedTime();

	if(pod1 - POD_HEIGHT / 2.f <= POD_MARGIN)
		pod1 = POD_MARGIN + POD_HEIGHT / 2.f;
	else if(pod1 + POD_HEIGHT / 2.f >= client.bottom - client.top - POD_MARGIN)
		pod1 = client.bottom - client.top - POD_MARGIN - POD_HEIGHT / 2.f;

	return TRUE;
}

BOOL WinClose(HWND hWnd)
{
	PostQuitMessage(NULL);

	return TRUE;
}

BOOL WinCommand(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case ID_GAME_EXIT:
		SendMessage(hWnd,WM_CLOSE,NULL,NULL);
		break;

	case ID_ABOUT_TETRIS:
		return WinAbout(hWnd);

	case ID_ABOUT_HELP:
		return WinHelp(hWnd,TEXT("Help.hlp"),HELP_INDEX,NULL);
	}

	return TRUE;
}

BOOL WinContextMenu(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	return TRUE;
}

BOOL WinButtonDown(HWND hWnd,UINT button,LONG mx,LONG my)
{
	return TRUE;
}

BOOL WinPaint(HWND hWnd)
{
	RECT clientRect;
	GetClientRect(hWnd,&clientRect);

	if(clientRect.right - clientRect.left <= 0 || clientRect.bottom - clientRect.top <= 0)
		return FALSE;

	PAINTSTRUCT paint;
	HDC hDC = BeginPaint(hWnd,&paint);
	HDC hCompatibleDC = CreateCompatibleDC(hDC);
	HBITMAP hBackbuffer = CreateCompatibleBitmap(hDC,clientRect.right-clientRect.left,clientRect.bottom-clientRect.top);
	HBITMAP hOldBitmap  = (HBITMAP)SelectObject(hCompatibleDC,hBackbuffer);

	SetBkMode(hCompatibleDC,TRANSPARENT);
	DeleteObject(SelectObject(hCompatibleDC,GetStockObject(DEFAULT_GUI_FONT)));

	HBRUSH brush = CreateSolidBrush(RGB(0,0,0));
	FillRect(hCompatibleDC,&clientRect,brush);
	DeleteObject(brush);

	brush = CreateSolidBrush(RGB(255,255,255));

	RECT rect;

	if(pod1 == -1.f)
		pod1 = (clientRect.bottom - clientRect.top) / 2.f;

	if(pod2 == -1.f)
		pod2 = (clientRect.bottom - clientRect.top) / 2.f;

	if(ballx == -1.f)
		ballx = (clientRect.right - clientRect.left) / 2.f;

	if(bally == -1.f)
		bally = (clientRect.bottom - clientRect.top) / 2.f;

	rect.left = POD_MARGIN;
	rect.bottom = pod1 + POD_HEIGHT / 2.f;
	rect.top = pod1 - POD_HEIGHT / 2.f;
	rect.right = rect.left + POD_WIDTH;

	FillRect(hCompatibleDC,&rect,brush);

	rect.right = clientRect.right - clientRect.left - POD_MARGIN;
	rect.left = rect.right - POD_WIDTH;
	rect.bottom = pod2 + POD_HEIGHT / 2.f;
	rect.top = pod2 - POD_HEIGHT / 2.f;

	FillRect(hCompatibleDC,&rect,brush);

	rect.left = ballx - BALL_WIDTH / 2.f;
	rect.bottom = bally - BALL_HEIGHT / 2.f;
	rect.top = bally + BALL_HEIGHT / 2.f;
	rect.right = ballx + BALL_WIDTH / 2.f;

	FillRect(hCompatibleDC,&rect,brush);

	DeleteObject(brush);

	BitBlt(hDC,0,0,clientRect.right-clientRect.left,clientRect.bottom-clientRect.top,hCompatibleDC,0,0,SRCCOPY);
	SelectObject(hCompatibleDC, hOldBitmap);
	DeleteObject(hBackbuffer);
	DeleteDC(hCompatibleDC);

	EndPaint(hWnd,&paint);

	return TRUE;
}

BOOL WinAbout(HWND hWnd)
{
	TCHAR application[0x100];
	TCHAR credits[0x100];

	LoadString(GetModuleHandle(NULL),IDS_APPLICATION,application,sizeof(application)/sizeof(TCHAR));
	LoadString(GetModuleHandle(NULL),IDS_CREDITS,credits,sizeof(credits)/sizeof(TCHAR));

	return ShellAbout(hWnd,application,credits,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_MAIN)));
}
#pragma once

#define MAIN_WINDOW_WIDTH 640
#define MAIN_WINDOW_HEIGHT 480

INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR cmdLine,INT cmdShow);
LRESULT WINAPI WinProcedure(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

BOOL WinCreate(HWND hWnd);
BOOL WinDestroy(HWND hWnd);
BOOL WinClose(HWND hWnd);
BOOL WinCommand(HWND hWnd,WPARAM wParam,LPARAM lParam);
BOOL WinContextMenu(HWND hWnd,WPARAM wParam,LPARAM lParam);
BOOL WinButtonDown(HWND hWnd,UINT button,LONG mx,LONG my);
BOOL WinAbout(HWND hWnd);
BOOL WinPaint(HWND hWnd);
BOOL WinTimer(HWND hWnd,UINT eventId);

DOUBLE GetElapsedTime(VOID);
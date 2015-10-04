#pragma once

#include <windows.h>
#include <commctrl.h>
#include <assert.h>

#pragma comment(lib,"comctl32.lib")

VOID MessageBox(HWND hWnd,UINT text,UINT caption,UINT type = MB_OK);
VOID MessageBox(HWND hWnd,LPCTSTR caption,UINT type,LPCTSTR fmt,...);
VOID Trace(LPCTSTR fmt,...);

#ifdef _DEBUG
#define ASSERT assert
#define TRACE Trace
#else
#define ASSERT __noop
#define TRACE __noop
#endif

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

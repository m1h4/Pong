#include "Globals.h"

// These definitions are required when using the older runtime library
extern "C"
{
long _ftol2(double);
long _ftol2_sse(double x) { return _ftol2(x); }
void __fastcall __security_check_cookie(uintptr_t _StackCookie) {}
uintptr_t __security_cookie;
}

VOID Trace(LPCTSTR fmt,...)
{
#ifdef _DEBUG
	TCHAR messageBuffer[1024];
	
	va_list args;
    va_start(args,fmt);
    wvsprintf(messageBuffer,fmt,args);
    va_end(args);

	OutputDebugString(messageBuffer);
#endif
}

VOID MessageBox(HWND hWnd,LPCTSTR caption,UINT type,LPCTSTR fmt,...)
{
	TCHAR messageBuffer[1024];
	
	va_list args;
    va_start(args,fmt);
    wvsprintf(messageBuffer,fmt,args);
    va_end(args);

	MessageBox(hWnd,messageBuffer,caption,type);
}

VOID MessageBox(HWND hWnd,UINT text,UINT caption,UINT type)
{
	TCHAR txt[1024];
	TCHAR cap[1024];

	LoadString(GetModuleHandle(NULL),text,txt,sizeof(txt)/sizeof(TCHAR));
	LoadString(GetModuleHandle(NULL),caption,cap,sizeof(cap)/sizeof(TCHAR));

	MessageBox(hWnd,txt,cap,type);
}
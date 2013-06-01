#include<windows.h>
#include<string.h>
#include<stdio.h>
#include "resource.h"
struct
{
	int iStyle ;
	TCHAR * szText ;
}
button[] =
{
	BS_PUSHBUTTON, TEXT ("锁U盘"),
		BS_PUSHBUTTON, TEXT ("解U盘"),
} ;
#define NUM (sizeof button / sizeof button[0])
static int  cxChar = LOWORD (GetDialogBaseUnits ()) ,
cyChar = HIWORD (GetDialogBaseUnits ()) ;
DWORD RegSetUShut();
DWORD RegSetUStart();
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("RegU") ;
	HWND hwnd ;
	MSG msg ;
	WNDCLASS wndclass ;
	wndclass.style = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc = WndProc ;
	wndclass.cbClsExtra = 0 ;
	wndclass.cbWndExtra = 0 ;
	wndclass.hInstance = hInstance ;
	wndclass.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE (IDI_ICON)) ;	
	wndclass.hCursor = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName = NULL ;
	wndclass.lpszClassName = szAppName ;
	if (!RegisterClass (&wndclass))
	{
		MessageBox ( NULL, TEXT ("This program requires Windows	NT!"),
			szAppName,
			MB_ICONERROR) ;
		return 0 ;
	}
	hwnd = CreateWindow ( szAppName, TEXT ("注册表锁U盘"),
		WS_OVERLAPPEDWINDOW,
		400,300,
		23 * cxChar, 8* cyChar,
		NULL, NULL, hInstance, NULL) ;
	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndButton[NUM] ;
	int i ;
	switch (message)
	{
	case WM_CREATE :
		for (i = 0 ; i < NUM ; i++)
			hwndButton[i] = CreateWindow
			( TEXT("button"),button[i].szText,
			WS_CHILD | WS_VISIBLE | button[i].iStyle,
			cxChar, cyChar * (1 + 2 * i),
			20 * cxChar, 7 * cyChar / 4,
			hwnd, (HMENU) i,
			((LPCREATESTRUCT) lParam)->hInstance, NULL) ;
		return 0 ;
	case WM_COMMAND :
		if(lParam==(LPARAM)hwndButton[0])
		{
			DWORD ret=RegSetUShut();	
			if(ret==ERROR_SUCCESS)
			{
				MessageBox (NULL, TEXT ("成功锁住U盘!"), TEXT ("锁住U盘"), 0);
			}
		}
		else if(lParam==(LPARAM)hwndButton[1])
		{
			DWORD ret=RegSetUStart();
			if(ret==ERROR_SUCCESS)
			{
				MessageBox (NULL, TEXT ("成功解锁U盘!"), TEXT ("解锁U盘"), 0);
			}
		}
		break ;
	case WM_DESTROY :
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
DWORD RegSetUShut()
{
	HKEY hkey;
	DWORD Data = 4;
	DWORD ret;
	//打开指定的注册表键：
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\services\\USBSTOR"), 0, KEY_ALL_ACCESS, &hkey);
	if(ret==ERROR_SUCCESS)
	{
		//下面是写入一个名称为Start,类型为REG_DWORD,数据为4
		ret=RegSetValueEx(hkey, TEXT("Start"), 0, REG_DWORD, (LPBYTE)&Data, sizeof(Data)); 
	}
	RegCloseKey(hkey);
	return ret;
}
DWORD RegSetUStart()
{
	HKEY hkey;
	DWORD Data = 3;
	DWORD ret;
	//打开指定的注册表键：
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\services\\USBSTOR"), 0, KEY_ALL_ACCESS, &hkey);
	if(ret==ERROR_SUCCESS)
	{
		//下面是写入一个名称为Start,类型为REG_DWORD,数据为3
		ret=RegSetValueEx(hkey, TEXT("Start"), 0, REG_DWORD, (LPBYTE)&Data, sizeof(Data)); 
	}
	RegCloseKey(hkey);
	return ret;
}
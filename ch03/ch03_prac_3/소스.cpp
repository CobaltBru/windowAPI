#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND		hwnd;
	MSG			msg;
	WNDCLASS	WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_QUESTION);		//윈도우 아이콘
	WndClass.hCursor = LoadCursor(NULL, IDC_IBEAM);		//커서 모양
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//배경 색
	WndClass.lpszMenuName = NULL;	//메뉴 이름
	WndClass.lpszClassName = _T("Window Class Name");	//윈도우 클래스 이름
	RegisterClass(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("Cobaltbru's First Window"),		//윈도우 타이틀 이름
		WS_OVERLAPPEDWINDOW,		//윈도우 스타일
		200,	//윈도우 위치 X
		300,	//윈도우 위치 Y
		600,	//윈도우 가로
		400,	//윈도우 세로
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);		//윈도우 기본 출력 함수
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC				hdc;
	PAINTSTRUCT		ps;
	static int		x, y;
	static RECT		endPoint;
	static BOOL		enter;
	static int		way;

	switch (iMsg)
	{
	case WM_CREATE:
		endPoint = { 0,0,320,320 };
		enter = false;
		x = 20;
		y = 20;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 0, 0, 320, 320);
		Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			enter = !enter;
		}
		else if (wParam == VK_RIGHT)
		{
			way = 1;
			SetTimer(hwnd, 1, 70, NULL);
		}
		else if (wParam == VK_LEFT)
		{
			way = 2;
			SetTimer(hwnd, 1, 70, NULL);
		}
		else if (wParam == VK_UP)
		{
			way = 3;
			SetTimer(hwnd, 1, 70, NULL);
		}
		else if (wParam == VK_DOWN)
		{
			way = 4;
			SetTimer(hwnd, 1, 70, NULL);
		}
		break;
	case WM_TIMER:
		if(way == 1) {
			x += 40;
			if (x + 20 > endPoint.right) x -= 40;
			if (!enter)KillTimer(hwnd, 1);
		}
		else if (way == 2)
		{
			x -= 40;
			if (x - 20 < endPoint.left) x += 40;
			if (!enter)KillTimer(hwnd, 1);
		}
		else if (way == 3)
		{
			y -= 40;
			if (y - 20 < endPoint.top) y += 40;
			if (!enter)KillTimer(hwnd, 1);
		}
		else if (way == 4)
		{
			y += 40;
			if (y + 20 > endPoint.bottom) y -= 40;
			if (!enter)KillTimer(hwnd, 1);
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
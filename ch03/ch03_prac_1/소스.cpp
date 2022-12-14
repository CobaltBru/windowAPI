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
	static RECT		rectView; // 한계 좌표

	switch (iMsg)
	{
	case WM_CREATE:
		rectView = { 0,0,120,120 };
		x = 20;
		y = 20;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 0, 0, 120, 120); // 한계 좌표를 표시한 사각형
		Ellipse(hdc, x - 20, y - 20, x + 20, y + 20); // x,y는 원의 중심 반지름이 20
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RIGHT) // 상하좌우 키 입력. 범위를 넘어가면 되돌림
		{
			x += 40;
			if (x + 20 > rectView.right) x -= 40;
		}
		else if (wParam == VK_LEFT)
		{
			x -= 40;
			if (x - 20 < rectView.left) x += 40;
		}
		else if (wParam == VK_UP)
		{
			y -= 40;
			if (y - 20 < rectView.top) y += 40;
		}
		else if (wParam == VK_DOWN)
		{
			y += 40;
			if (y + 20 > rectView.bottom) y -= 40;
		}
		else if (wParam == VK_HOME) // HOME키를 누르면 초기값으로 돌아감
		{
			x = 20;
			y = 20;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
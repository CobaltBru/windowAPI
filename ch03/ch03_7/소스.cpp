#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <TCHAR.H>
#include <math.h>
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
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//커서 모양
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
	HDC			hdc;
	PAINTSTRUCT	ps;
	static int	startX, startY, oldX, oldY;
	static BOOL	Drag;
	int			endX, endY;

	switch (iMsg)
	{
	case WM_CREATE:
		Drag = FALSE;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		startX = oldX = LOWORD(lParam);
		startY = oldY = HIWORD(lParam);
		Drag = TRUE;
		break;
	case WM_LBUTTONUP:
		startX = oldX = 0;
		startY = oldY = 0;
		Drag = FALSE;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (Drag)
		{
			SetROP2(hdc, R2_XORPEN);
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY);
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);
			oldX = endX; oldY = endY;
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
	


}
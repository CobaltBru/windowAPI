#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <TCHAR.H>
#include <math.h>
#define BSIZE 40
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


void CatMove(POINT *cat, int mx, int my) // 고양이의 움직임을 구현하는 함수
{
	if (cat->x > mx) // 쥐의 좌표와 비교해서 따라감
	{
		cat->x--;
	}
	if (cat->x < mx)
	{
		cat->x++;
	}
	if (cat->y > my)
	{
		cat->y--;
	}
	if (cat->y < my)
	{
		cat->y++;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC				hdc;
	PAINTSTRUCT		ps;
	static int		x, y;
	static BOOL		Selection; // 클릭되어있는지 체크하는 값
	static POINT	catPoint; // 고양이의 좌표를 저장하는 구조체
	switch (iMsg)
	{
	case WM_CREATE:
		x = 50; y = 50;
		catPoint.x = 100; catPoint.y = 100;
		Selection = FALSE;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (Selection) // 클릭되어있으면 쥐 표시
		{
			TextOut(hdc, x, y, _T("쥐"), _tcslen(_T("쥐")));
		}
		TextOut(hdc, catPoint.x, catPoint.y, _T("고양이"), _tcslen(_T("고양이")));

		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		Selection = TRUE;
		SetTimer(hwnd, 1, 1, NULL); // 클릭시 타이머 작동
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		Selection = FALSE;
		KillTimer(hwnd, 1);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_TIMER:
		CatMove(&catPoint, x, y);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
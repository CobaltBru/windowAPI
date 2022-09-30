#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <TCHAR.H>
#include <math.h>
#include <stdlib.h>
#include <time.h>
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
		1000,	//윈도우 가로
		600,	//윈도우 세로
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
struct Circle
{
	POINT p;
	int r;
	BOOL crash;
};

double LengthPts(int x1, int y1, int x2, int y2)
{
	return(sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my, int r1, int r2)
{
	if (LengthPts(x, y, mx, my) < (r1 + r2)) return TRUE;
	else return FALSE;
}

void Checker(Circle c_array[], int x, int y)
{
	for (int i = 0; i < 30; i++)
	{
		if (InCircle(c_array[i].p.x, c_array[i].p.y, x, y, c_array[i].r, 50))
		{
			c_array[i].crash = TRUE;
		}
	}
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC				hdc;
	PAINTSTRUCT		ps;
	static Circle*	c_array;
	static int		x, y;
	static int		mx, my;
	static BOOL		Selection;
	HBRUSH			hBrush, oldBrush;
	static int		counter;
	static BOOL		clear;
	TCHAR			lpOut[1024];
	static clock_t	start, end;
	switch (iMsg)
	{
	case WM_CREATE:
		srand((unsigned)time(NULL));
		c_array = (Circle*)malloc(sizeof(Circle) * 30);
		for (int i = 0; i < 30; i++)
		{
			Circle c;
			c.p.x = rand() % 1000 + 1;
			c.p.y = rand() % 600 + 1;
			c.r = rand() % 80 + 10;
			c.crash = FALSE;
			c_array[i] = c;
		}
		counter = 0;
		Selection = FALSE;
		x = 0; y = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Ellipse(hdc, x - 50, y - 50, x + 50, y + 50);
		Checker(c_array, x, y);
		for (int i = 0; i < 30; i++)
		{
			if (c_array[i].crash == FALSE)
			{
				hBrush = CreateSolidBrush(RGB(0, 255, 0));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Ellipse(hdc, c_array[i].p.x - c_array[i].r, c_array[i].p.y - c_array[i].r,
					c_array[i].p.x + c_array[i].r, c_array[i].p.y + c_array[i].r);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
			else
			{
				hBrush = CreateSolidBrush(RGB(255, 0, 0));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Ellipse(hdc, c_array[i].p.x - c_array[i].r, c_array[i].p.y - c_array[i].r,
					c_array[i].p.x + c_array[i].r, c_array[i].p.y + c_array[i].r);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
		}
		if (clear == FALSE)
		{
			if (InCircle(1000, 600, x, y, 10, 50))
			{
				for (int i = 0; i < 30; i++)
				{
					if (c_array[i].crash == TRUE)
					{
						counter++;
					}
				}
				clear = TRUE;
			}
		}
		else
		{
			end = clock();
			wsprintf(lpOut, TEXT("충돌횟수: %d // 걸린시간: %d"), counter, (end - start)/CLOCKS_PER_SEC);
			TextOut(hdc, 300, 250, lpOut, lstrlen(lpOut));
		}
		
		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(x, y, mx, my, 0, 50))
		{
			start = clock();
			Selection = TRUE;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		
		break;
	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (Selection == TRUE)
		{
			x = mx;
			y = my;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONUP:
		Selection = FALSE;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		free(c_array);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));

}
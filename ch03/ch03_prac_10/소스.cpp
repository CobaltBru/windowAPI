#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
		520,	//윈도우 가로
		550,	//윈도우 세로
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

void MakeMap(HDC hdc)
{
	for (int i = 0; i < 25; i++)
	{
		Rectangle(hdc, i * 20, 0, i * 20 + 20, 20);
	}
	for (int i = 0; i < 23; i++)
	{
		Rectangle(hdc, 0, 20 + i * 20, 20, 40 + i * 20);
		Rectangle(hdc, 480, 20 + i * 20, 500, 40 + i * 20);
	}
	for (int i = 0; i < 25; i++)
	{
		Rectangle(hdc, i * 20, 480, i * 20 + 20, 500);
	}
	Rectangle(hdc, 20, 20, 480, 480);
	return;
}

void MoveCircle(POINT circle[], int x, int y, int counter)
{
	int tmpx = circle[0].x, tmpy = circle[0].y;
	int curx = x, cury = y;
	
	circle[0].x = curx;
	circle[0].y = cury;
	for (int i = 1; i <= counter; i++)
	{
		curx = tmpx;
		cury = tmpy;
		tmpx = circle[i].x;
		tmpy = circle[i].y;
		circle[i].x = curx;
		circle[i].y = cury;
	}
	return;
}

void MakeFood(int* food_count, POINT food[])
{
	srand((unsigned)time(NULL));
	int i = 0;
	while (food[i].x != -1 && food[i].y != -1) ++i;
	food[i].x = 20 * (rand() % 23) + 30;
	food[i].y = 20 * (rand() % 23) + 30;
	*++food_count;
	return;
}

double LengthPts(int x1, int y1, int x2, int y2)
{
	return(sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my)
{
	if (LengthPts(x, y, mx, my) <= 20) return TRUE;
	else return FALSE;
}

void CrashCheck(POINT food[], int* food_count, POINT circle[], int* counter)
{
	int i = 0;
	for (int l = 0; i < *food_count || l < 100; l++)
	{
		if (food[l].x != -1 && food[l].y != -1)
		{
			if ((food[l].x == circle[0].x) && (food[l].y == circle[0].y))
			{
				i++;
				food[l].x = -1;
				food[l].y = -1;
				*--food_count;
				*++counter;
				circle[*counter-1].x = circle[*counter - 2].x;
				circle[*counter-1].y = circle[*counter - 2].y;
			}
			i++;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC				hdc;
	PAINTSTRUCT		ps;
	static int		x, y, oldX, oldY; // 머리 원, 꼬리 원의 좌표
	static int		c_counter;
	static RECT		endPoint; // 한계 좌표
	static int		way; // 방향을 저장할 플래그
	static int		food_count;
	static POINT	food[100];
	static POINT	circle[1000];
	static int		time_counter;
	static int		k;
	HPEN hPen, oldPen;
	switch (iMsg)
	{
	case WM_CREATE:
		endPoint = { 20,20,480,480 };
		c_counter = 5;
		x = 70; y = 430;
		circle[0].x = 70;
		circle[0].y = 430;
		circle[1].x = 50;
		circle[1].y = 430;
		circle[2].x = 30;
		circle[2].y = 430;
		food_count = 0;
		time_counter = 0;
		k = 0;
		for (int i = 0; i < 100; i++)
		{
			food[i].x = -1;
			food[i].y = -1;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		MakeMap(hdc);
		MoveCircle(circle, x, y, c_counter);

		hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		Ellipse(hdc, circle[0].x - 10, circle[0].y - 10, circle[0].x + 10, circle[0].y + 10); // 빨간색 머리 원
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);

		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		for (int i = 1; i < c_counter; i++)
		{
			Ellipse(hdc, circle[i].x - 10, circle[i].y - 10, circle[i].x + 10, circle[i].y + 10); 
		}
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		CrashCheck(food, &food_count, circle, &c_counter);
		for(int i=0;k<food_count || i<100;i++)
		{
			if(food[i].x != -1 && food[i].y != -1)
			{
				Ellipse(hdc, food[i].x - 10, food[i].y - 10, food[i].x + 10, food[i].y + 10);
				k++;
			}
			
		}
		k = 0;
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN: // 이하 예제 3과 동일
		if (wParam == VK_RIGHT)
		{
			way = 1;
		}
		else if (wParam == VK_LEFT)
		{
			way = 2;
		}
		else if (wParam == VK_UP)
		{
			way = 3;
		}
		else if (wParam == VK_DOWN)
		{
			way = 4;
		}
		SetTimer(hwnd, 2, 1000, NULL);
		SetTimer(hwnd, 1, 100, NULL);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (way == 1) {
				x += 20;
				if (x + 10 > endPoint.right) x -= 20;
			}
			else if (way == 2)
			{
				x -= 20;
				if (x - 10 < endPoint.left) x += 20;
			}
			else if (way == 3)
			{
				y -= 20;
				if (y - 10 < endPoint.top) y += 20;
			}
			else if (way == 4)
			{
				y += 20;
				if (y + 10 > endPoint.bottom) y -= 20;
			}
			break;
		case 2:
			food_count++;
			MakeFood(&food_count, food);
			break;
		}
		
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
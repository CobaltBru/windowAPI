#define _CRT_SECURE_NO_WARNINGS
#include "resource.h"
#include <windows.h>
#include <TCHAR.H>
#include <math.h>
#include <stdlib.h>
#include <time.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND		hwnd;
	MSG			msg;
	WNDCLASS	WndClass;
	hInst = hInstance;
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

void Animation(int xPos, int yPos, HDC hdc)
{
	HDC memdc;
	HBITMAP RunBit[10], hBit, oldBit, Mask[10];
	static int count;
	int i;
	count++;
	count = count % 10;
	//흰 배경 생김
	/*for (i = 0; i<10; i++)
		RunBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_R1+i));*/

	//AND 연산 출력으로 배경 제거
	for (i = 0; i < 10; i++)
		Mask[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1 + i));
	memdc = CreateCompatibleDC(hdc);
	hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5_6));
	oldBit = (HBITMAP)SelectObject(memdc, hBit);
	BitBlt(hdc, 0, 0, 819, 614, memdc, 0, 0, SRCAND);
	SelectObject(memdc, Mask[count]);
	BitBlt(hdc, xPos, yPos, 180, 240, memdc, 0, 0, SRCPAINT);
	SelectObject(memdc, oldBit);
	for (i = 0; i < 10; i++)
	{
		//DeleteObject(RunBit[i]);
		DeleteObject(Mask[i]);
	}
	DeleteDC(memdc);
	DeleteObject(hBit);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	PAINTSTRUCT ps;
	static int xPos;



	switch (iMsg)
	{
	case WM_CREATE:
		xPos = -100;
		SetTimer(hwnd, 1, 100, NULL);
		break;
	case WM_TIMER:
		xPos += 10;
		if (xPos > 819) xPos = -100;
		InvalidateRgn(hwnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Animation(xPos, 300, hdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}


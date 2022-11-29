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
	WndClass.hIcon = LoadIcon(NULL, IDI_QUESTION);		//������ ������
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//Ŀ�� ���
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//��� ��
	WndClass.lpszMenuName = NULL;	//�޴� �̸�
	WndClass.lpszClassName = _T("Window Class Name");	//������ Ŭ���� �̸�
	RegisterClass(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("Cobaltbru's First Window"),		//������ Ÿ��Ʋ �̸�
		WS_OVERLAPPEDWINDOW,		//������ ��Ÿ��
		200,	//������ ��ġ X
		300,	//������ ��ġ Y
		1000,	//������ ����
		600,	//������ ����
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);		//������ �⺻ ��� �Լ�
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
	//�� ��� ����
	/*for (i = 0; i<10; i++)
		RunBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_R1+i));*/

	//AND ���� ������� ��� ����
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


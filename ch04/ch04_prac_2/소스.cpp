#define _CRT_SECURE_NO_WARNINGS
#include "resource.h"
#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
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
	WndClass.hCursor = LoadCursor(NULL, IDC_IBEAM);		//커서 모양
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//배경 색
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU4_PRAC_2);	//메뉴 이름
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

double LengthPts(int x1, int y1, int x2, int y2)
{
	return(sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my)
{
	if (LengthPts(x, y, mx, my) < BSIZE) return TRUE;
	else return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	int answer;

	OPENFILENAME OFN;
	TCHAR str[100], lpstrFile[100] = _T("");
	TCHAR filter[] = _T("C++ 소스와 헤더 파일\0 *.cpp;*.h\0Every File(*.*) \0*.*\0");

	static POINT circleList[10];
	static int currentPoint;
	static int counter;
	HDC hdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	int mx, my;

	switch (iMsg)
	{
	case WM_CREATE:
		circleList[0].x = 20;
		circleList[0].y = 20;
		currentPoint = -1;
		counter = 1;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < counter; i++)
		{
			if (currentPoint == i)
			{
				hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Ellipse(hdc, circleList[i].x - 20, circleList[i].y + 20, circleList[i].x + 20, circleList[i].y - 20);
				SelectObject(hdc, oldPen);
				DeleteObject(hPen);
			}
			else
			{
				Ellipse(hdc, circleList[i].x - 20, circleList[i].y + 20, circleList[i].x + 20, circleList[i].y - 20);
			}
			
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EDITCOPY:

			break;
		case ID_EDITPASTE:
			circleList[counter].x = circleList[counter-1].x + 40;
			circleList[counter].y = circleList[counter-1].y + 40;
			counter++;
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case ID_FILENEW:
			MessageBox(hwnd,
				_T("새 파일을 열겠습니까?"),
				_T("새 파일 선택"),
				MB_OKCANCEL);
			break;
		case ID_FILEOPEN:
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFilter = filter;
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 100;
			OFN.lpstrInitialDir = _T(".");
			if (GetOpenFileName(&OFN) != 0)
			{
				_stprintf_s(str, _T("%s 파일을 열겠습니까?"), OFN.lpstrFile);
				MessageBox(hwnd, str, _T("열기 선택"), MB_OK);
			}
			break;
		case ID_EXIT:
			answer = MessageBox(hwnd,
				_T("파일을 저장하고 끝내시겠습니까?"),
				_T("끝내기 선택"),
				MB_YESNOCANCEL);
			if (answer == IDYES || answer == IDNO) PostQuitMessage(0);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		for (int i = 0; i < counter; i++)
		{
			if (InCircle(circleList[i].x, circleList[i].y, mx, my))
			{
				currentPoint = i;
				break;
			}
		}
		
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
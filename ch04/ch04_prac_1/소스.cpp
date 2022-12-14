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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU4_PRAC);	//메뉴 이름
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
	int answer;

	OPENFILENAME OFN;
	TCHAR str[100], lpstrFile[100] = _T("");
	TCHAR filter[] = _T("C++ 소스와 헤더 파일\0 *.cpp;*.h\0Every File(*.*) \0*.*\0");

	HDC hdc;
	TCHAR filter2[] = _T("텍스트 파일\0 *.txt\0Every File(*.*) \0*.*\0");
	OPENFILENAME SFN;
	PAINTSTRUCT ps;
	static TCHAR str2[1000];
	static int count;
	RECT rt = { 0,0,1000,1000 };

	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawText(hdc, str2, _tcslen(str2), &rt, DT_TOP | DT_LEFT);
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_BACK && count > 0) count--;
		else str2[count++] = wParam;
		str2[count] = NULL;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
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
		case ID_FILESAVE:
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hwnd;
			SFN.lpstrFilter = filter2;
			SFN.lpstrFile = lpstrFile;
			SFN.nMaxFile = 256;
			SFN.lpstrInitialDir = _T(".");
			if (GetSaveFileName(&SFN) != 0)
			{
				_stprintf_s(str, _T("%s 파일로 저장하겠습니까?"), SFN.lpstrFile);
				MessageBox(hwnd, str, _T("저장하기 선택"), MB_OK);
				FILE* fp = _wfopen(SFN.lpstrFile, L"w,ccs=UTF-8");
				_ftprintf_s(fp, str2);
				fclose(fp);
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
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
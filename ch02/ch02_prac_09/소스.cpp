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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)	//메시지 처리 함수
{
	HDC				hdc;
	PAINTSTRUCT		ps;
	static TCHAR	str[1000];
	static int		pos_array[4]; // 입력받은 문자열을 정수로 바꿔 저장할 배열
	static int		count;
	static SIZE		size;
	static int		FLAG; // 그릴 도형을 나타내는 플래그
	
	switch (iMsg)
	{
	case WM_CREATE:
		CreateCaret(hwnd, NULL, 5, 15);
		ShowCaret(hwnd);
		count = 0;
		FLAG = -1; 
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetTextExtentPoint(hdc, str, _tcslen(str), &size);
		Rectangle(hdc, 5, 330, 300, 354); // 문자열을 입력받을 장소의 사각형
		TextOut(hdc, 5, 332, str, _tcslen(str));
		SetCaretPos(size.cx + 5, 332);

		if (FLAG == 0) // 원
		{
			Ellipse(hdc, pos_array[0], pos_array[1], pos_array[2], pos_array[3]);
		}
		else if (FLAG == 1) // 선
		{
			MoveToEx(hdc, pos_array[0], pos_array[1],NULL);
			LineTo(hdc, pos_array[2], pos_array[3]);
		}
		else if (FLAG == 2) // 사각형
		{
			Rectangle(hdc, pos_array[0], pos_array[1], pos_array[2], pos_array[3]);
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		if (wParam == VK_BACK && count > 0) count--;
		else if (wParam == VK_RETURN)
		{
			if (_tcsstr(str, _T("Ellipse"))) // 문자열에 ~도형을 나타내는 단어를 검색
			{
				FLAG = 0;
				// _stscanf로 일정한 서식의 문자열 내의 필요한 값을 추출
				_stscanf(str, _T("Ellipse(%d, %d, %d, %d)"), &pos_array[0], &pos_array[1], &pos_array[2], &pos_array[3]);
			}
			else if (_tcsstr(str, _T("Line")))
			{
				FLAG = 1;
				_stscanf(str, _T("Line(%d, %d, %d, %d)"), &pos_array[0], &pos_array[1], &pos_array[2], &pos_array[3]);
			}
			else if (_tcsstr(str, _T("Rectangle")))
			{
				FLAG = 2;
				_stscanf(str, _T("Rectangle(%d, %d, %d, %d)"), &pos_array[0], &pos_array[1], &pos_array[2], &pos_array[3]);
			}


		}
		else str[count++] = wParam;
		str[count] = NULL;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		HideCaret(hwnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}	//처리할 메시지만 case문에 나열
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//나머지는 커널이 처리
}
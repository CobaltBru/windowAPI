#include <windows.h>
#include <TCHAR.H>
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
	const int		MAX = 10;
	HDC				hdc;
	PAINTSTRUCT		ps;
	static TCHAR	str [MAX][100]; //여러줄의 문자열을 담을 공간
	static int		lineCounter[100]; //한줄에 몇개의 문자가 있는지 저장할 공간
	static int		count, yPos;
	int i;
	switch (iMsg)
	{
	case WM_CREATE:
		count = 0;
		yPos = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (i = 0; i <= yPos; i++) // 반복문으로 저장된 모든 문자열을 출력
		{
			TextOut(hdc, 0, i*20, str[i], _tcslen(str[i]));
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (yPos >= MAX) break;
		if (wParam == VK_BACK) // 백스페이스
		{
			if (count > 0)
			{
				count--;
				str[yPos][count] = NULL;
			}
			else if (count == 0) // 현재 줄에 문자가 없다면?
			{
				if (yPos == 0) {} // 첫 행이면 아무것도 하지 않음
				else{			
					yPos--;		//그 외에는 행을 한칸 올리고
					count = lineCounter[yPos];	//저장해두었던 문자열 갯수를 불러옴
					count--;
					str[yPos][count] = NULL; // \0 문자를 지움
				}
			}
		}
		else if (wParam == VK_RETURN && yPos < MAX) // 엔터키
		{
			str[yPos][count] = '\0'; // 널문자로 문장의 끝을 알림
			lineCounter[yPos] = count; // 현재 행의 문자열 길이를 저장
			yPos++;		//행 증가
			count = 0;
		}
		else str[yPos][count++] = wParam; // 문자를 저장한 후 count 를 1늘리고
		if (count >= 99 && yPos< MAX) // 한 행에 99자를 넘기면 개행
		{
			str[yPos][count] = '\0';
			lineCounter[yPos] = count;
			yPos++;
			count = 0;
		}
		InvalidateRgn(hwnd, NULL, TRUE); //즉시 WM_PAINT를 호출
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}	//처리할 메시지만 case문에 나열
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//나머지는 커널이 처리
}

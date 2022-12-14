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
	HDC				hdc;
	PAINTSTRUCT		ps;
	static TCHAR	str[1000][1000];
	static int		count, yPos;
	static SIZE		size;
	static bool		flag;
	switch (iMsg)
	{
	case WM_CREATE:
		CreateCaret(hwnd, NULL, 5, 15);
		ShowCaret(hwnd);
		flag = true;
		yPos = 0;
		count = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if(flag == true)
		{
			GetTextExtentPoint(hdc, str[yPos], _tcslen(str[yPos]), &size);
			Rectangle(hdc, 50, 50, 300, 300);
			for (int i = 0; i <= yPos; i++)
			{
				TextOut(hdc, 50, 50 + i * 20, str[i], _tcslen(str[i]));
			}
			SetCaretPos(size.cx + 50, yPos * 20 + 50);
		}
		else // 글상자를 벗어났다는 뜻이므로 캐럿을 삭제
		{
			HideCaret(hwnd);
			DestroyCaret();
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		if(yPos<=250/20-1) //글상자의 y축 범위
		{
			if (wParam == VK_BACK)
			{
				if (count == 0)
				{
					if (yPos > 0)
					{
						yPos--;
						count = _tcslen(str[yPos]);
					}
				}
				else
				{
					count--;
					str[yPos][count] = NULL;
				}
			}
			else if (wParam == VK_RETURN)
			{
				str[yPos][count] = '\0';
				yPos++;
				count = 0;
			}
			else
			{
				if (size.cx + 5 >= 250) // 글상자의 x축 범위
				{
					str[yPos][count] = '\0';
					yPos++;
					count = 0;
				}
				else
				{
					str[yPos][count++] = wParam;
					str[yPos][count] = NULL;
				}
				
				
			}
		}
		else // 글상자 범위를 벗어나면 flag를 false 해줌
		{
			flag = false;
		}
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
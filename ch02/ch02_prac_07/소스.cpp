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
	RECT			left_rt = { 50, 100, 100, 150 };
	RECT			right_rt = { 150, 100, 200, 150 };
	RECT			up_rt = { 100, 50, 150, 100 };
	RECT			down_rt = { 100, 150, 150, 200 };
	HBRUSH			hBrush, oldBrush;
	static TCHAR	str[10];
	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		//기본 틀
		Rectangle(hdc, 50, 100, 100, 150); // left
		DrawText(hdc, _T("좌측"), _tcslen(_T("좌측")), &left_rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Rectangle(hdc, 150, 100, 200, 150); // right
		DrawText(hdc, _T("우측"), _tcslen(_T("우측")), &right_rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Rectangle(hdc, 100, 50, 150, 100); // up
		DrawText(hdc, _T("상단"), _tcslen(_T("상단")), &up_rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Rectangle(hdc, 100, 150, 150, 200); // down
		DrawText(hdc, _T("하단"), _tcslen(_T("하단")), &down_rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (str[0] == VK_LEFT) // str에 저장되어있는 키 값에 따라 색칠
		{
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, 50, 100, 100, 150); // left
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}
		else if (str[0] == VK_RIGHT)
		{
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, 150, 100, 200, 150); // right
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		}
		else if (str[0] == VK_UP)
		{
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, 100, 50, 150, 100); // up
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		}
		else if (str[0] == VK_DOWN)
		{
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, 100, 150, 150, 200); // down
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		}

		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:		//키가 눌렸을 때
		hdc = GetDC(hwnd);
		str[0] = wParam;	//누른 키의 값을 저장
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_KEYUP:			//키를 땔 때
		hdc = GetDC(hwnd);
		str[0] = NULL;		//눌렀던 키의 값을 삭제
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}	//처리할 메시지만 case문에 나열
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//나머지는 커널이 처리
}
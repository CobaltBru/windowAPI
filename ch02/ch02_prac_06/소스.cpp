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
	POINT			point[10] = { {10,200},{250,80},{500,200},{350,350},{150,350} };
	HPEN			hPen, oldPen; // 펜
	HBRUSH			hBrush, oldBrush; // 면
	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hPen = CreatePen(PS_DASH, 1, RGB(255, 212, 0)); // 펜 생성
		hBrush = CreateSolidBrush(RGB(255, 192, 203)); // 브러쉬 생성
		oldPen = (HPEN)SelectObject(hdc, hPen); // 생성한 펜 지정후 반환된 원래 펜 저장
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush); // 생성한 브러쉬 지정후 반환된 원래 브러쉬 저장
		Ellipse(hdc, 180, 100, 220, 140); // 원(타원) 그리기
		SelectObject(hdc, oldPen); // 저장해두었던 원래 펜 지정
		SelectObject(hdc, oldBrush); // 저장해두었던 원래 브러쉬 지정
		DeleteObject(hPen); // 만들었던 펜 삭제
		DeleteObject(hBrush); // 만들었던 브러쉬 삭제



		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}	//처리할 메시지만 case문에 나열
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//나머지는 커널이 처리
}
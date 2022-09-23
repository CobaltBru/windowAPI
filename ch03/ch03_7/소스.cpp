#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <TCHAR.H>
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
		600,	//������ ����
		400,	//������ ����
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT	ps;
	static int	startX, startY, oldX, oldY;
	static BOOL	Drag;
	int			endX, endY;

	switch (iMsg)
	{
	case WM_CREATE:
		Drag = FALSE;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, oldX, oldY);
		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		startX = oldX = LOWORD(lParam);
		startY = oldY = HIWORD(lParam);
		Drag = TRUE;
		break;
	case WM_LBUTTONUP:
		startX = oldX = 0;
		startY = oldY = 0;
		Drag = FALSE;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (Drag)
		{
			SetROP2(hdc, R2_XORPEN);
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY);
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);
			oldX = endX; oldY = endY;
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
	


}
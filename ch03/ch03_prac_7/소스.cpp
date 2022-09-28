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

double LengthPts(int x1, int y1, int x2, int y2)
{
	return(sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my)
{
	if (LengthPts(x, y, mx, my) < 20) return TRUE;
	else return FALSE;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT	ps;
	static POINT circle;
	static BOOL clicked;
	static int	startX, startY, oldX, oldY;
	static BOOL	Drag;
	static int	endX, endY;
	static int	moving;
	switch (iMsg)
	{
	case WM_CREATE:
		Drag = FALSE;
		circle.x = 20; circle.y = 20;
		moving = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (clicked)
		{
			SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_BRUSH));
			Ellipse(hdc, circle.x - 20, circle.y - 20, circle.x + 20, circle.y + 20);
		}
		else
		{
			Ellipse(hdc, circle.x - 20, circle.y - 20, circle.x + 20, circle.y + 20);
		}
		

		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		startX = oldX = 20;
		startY = oldY = 20;
		if (InCircle(circle.x, circle.y, startX, startY))
		{	//�� ���� Ŭ���������� �۵�
			Drag = TRUE;
			clicked = TRUE;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		startX = oldX = 0;
		startY = oldY = 0;
		if (Drag)
		{	//�� ���� Ŭ���ؼ� ������ �׷ȴٸ� �۵�
			SetTimer(hwnd, 1, 70, NULL);
		}
		Drag = FALSE;
		clicked = FALSE;
		circle.x = 20; circle.y = 20; // �� ����ġ
		moving = 0;
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
	case WM_TIMER:
		if (circle.x >= endX && circle.y >= endY) KillTimer(hwnd, 1);
		moving +=10;
		circle.x = moving + 20; // �����Լ� �������� ��θ� ���
		circle.y = ((double)(endY - 20) / (endX - 20)) * moving + 20;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));

}
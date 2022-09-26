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
	WndClass.hIcon = LoadIcon(NULL, IDI_QUESTION);		//������ ������
	WndClass.hCursor = LoadCursor(NULL, IDC_IBEAM);		//Ŀ�� ���
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
	HDC				hdc;
	PAINTSTRUCT		ps;
	static int		x, y, oldX, oldY; // �Ӹ� ��, ���� ���� ��ǥ
	static RECT		endPoint; // �Ѱ� ��ǥ
	static BOOL		enter; // enterŰ�� �������� ������ �÷�ũ
	static int		way; // ������ ������ �÷���
	HPEN hPen, oldPen;

	switch (iMsg)
	{
	case WM_CREATE:
		endPoint = { 0,0,320,320 };
		enter = false;
		x = 60;
		y = 20;
		oldX = 20;
		oldY = 20;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 0, 0, 320, 320); // �Ѱ踦 ǥ������ �簢��
		if (oldX == x && oldY == y)// �� ���� ��ġ�� �ʰ� ��
		{
			switch (way)
			{
			case 1:
				oldX -= 40;
				break;
			case 2:
				oldX += 40;
				break;
			case 3:
				oldY += 40;
				break;
			case 4:
				oldY -= 40;
				break;
			}
		}
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		Ellipse(hdc, oldX - 20, oldY - 20, oldX + 20, oldY + 20); // �Ķ��� ���� ��

		hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		Ellipse(hdc, x - 20, y - 20, x + 20, y + 20); // ������ �Ӹ� ��
		oldX = x; oldY = y; // ���� ���� �׻� �Ӹ� ���� ����ٴ�
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN: // ���� ���� 3�� ����
		if (wParam == VK_RETURN)
		{
			enter = !enter;
		}
		else if (wParam == VK_RIGHT)
		{
			way = 1;
			SetTimer(hwnd, 1, 70, NULL);
		}
		else if (wParam == VK_LEFT)
		{
			way = 2;
			SetTimer(hwnd, 1, 70, NULL);
		}
		else if (wParam == VK_UP)
		{
			way = 3;
			SetTimer(hwnd, 1, 70, NULL);
		}
		else if (wParam == VK_DOWN)
		{
			way = 4;
			SetTimer(hwnd, 1, 70, NULL);
		}
		break;
	case WM_TIMER:
		if (way == 1) {
			x += 40;
			if (x + 20 > endPoint.right) x -= 40;
			if (!enter)KillTimer(hwnd, 1);
		}
		else if (way == 2)
		{
			x -= 40;
			if (x - 20 < endPoint.left) x += 40;
			if (!enter)KillTimer(hwnd, 1);
		}
		else if (way == 3)
		{
			y -= 40;
			if (y - 20 < endPoint.top) y += 40;
			if (!enter)KillTimer(hwnd, 1);
		}
		else if (way == 4)
		{
			y += 40;
			if (y + 20 > endPoint.bottom) y -= 40;
			if (!enter)KillTimer(hwnd, 1);
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
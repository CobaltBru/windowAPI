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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)	//�޽��� ó�� �Լ�
{
	HDC				hdc;
	PAINTSTRUCT		ps;
	static TCHAR	str[1000];
	static int		pos_array[4]; // �Է¹��� ���ڿ��� ������ �ٲ� ������ �迭
	static int		count;
	static SIZE		size;
	static int		FLAG; // �׸� ������ ��Ÿ���� �÷���
	
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
		Rectangle(hdc, 5, 330, 300, 354); // ���ڿ��� �Է¹��� ����� �簢��
		TextOut(hdc, 5, 332, str, _tcslen(str));
		SetCaretPos(size.cx + 5, 332);

		if (FLAG == 0) // ��
		{
			Ellipse(hdc, pos_array[0], pos_array[1], pos_array[2], pos_array[3]);
		}
		else if (FLAG == 1) // ��
		{
			MoveToEx(hdc, pos_array[0], pos_array[1],NULL);
			LineTo(hdc, pos_array[2], pos_array[3]);
		}
		else if (FLAG == 2) // �簢��
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
			if (_tcsstr(str, _T("Ellipse"))) // ���ڿ��� ~������ ��Ÿ���� �ܾ �˻�
			{
				FLAG = 0;
				// _stscanf�� ������ ������ ���ڿ� ���� �ʿ��� ���� ����
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
	}	//ó���� �޽����� case���� ����
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//�������� Ŀ���� ó��
}
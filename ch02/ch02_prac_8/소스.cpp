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
	static TCHAR	str[1000][1000];
	static int		count, yPos;
	static SIZE		size;

	switch (iMsg)
	{
	case WM_CREATE:
		yPos = 0;
		count = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 50, 50, 300, 300);
		for (int i = 0; i <= yPos; i++)
		{
			TextOut(hdc, 50, 50 + i * 20, str[i], _tcslen(str[i]));
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		if(yPos<=250/20) 
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
				str[yPos][count++] = wParam;
				str[yPos][count] = NULL;
			}
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}	//ó���� �޽����� case���� ����
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//�������� Ŀ���� ó��
}
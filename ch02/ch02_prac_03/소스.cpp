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
	const int		MAX = 10;
	HDC				hdc;
	PAINTSTRUCT		ps;
	static TCHAR	str [MAX][MAX+1];
	static int		lineCounter[MAX];
	static int		count, yPos;
	RECT rt = { 0,0,1000,1000 };
	int i;
	switch (iMsg)
	{
	case WM_CREATE:
		count = 0;
		yPos = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (i = 0; i <= yPos; i++)
		{
			TextOut(hdc, 0, i*20, str[i], _tcslen(str[i]));
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (yPos >= MAX) break;
		if (wParam == VK_BACK)
		{
			if (count > 0)
			{
				count--;
				str[yPos][count] = NULL;
			}
			else if (count == 0)
			{
				if (yPos == 0) {}
				else{
					yPos--;
					count = lineCounter[yPos];
					count--;
					str[yPos][count] = NULL;
				}
			}
		}
		else if (wParam == VK_RETURN && yPos < MAX)
		{
			str[yPos][count] = '\0';
			lineCounter[yPos] = count;
			yPos++;
			count = 0;
		}
		else str[yPos][count++] = wParam; // ���ڸ� ������ �� count �� 1�ø���
		if (count >= MAX && yPos< MAX)
		{
			str[yPos][count] = '\0';
			lineCounter[yPos] = count;
			yPos++;
			count = 0;
		}
		InvalidateRgn(hwnd, NULL, TRUE); //��� WM_PAINT�� ȣ��
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}	//ó���� �޽����� case���� ����
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//�������� Ŀ���� ó��
}

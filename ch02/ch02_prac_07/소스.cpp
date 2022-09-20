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

		//�⺻ Ʋ
		Rectangle(hdc, 50, 100, 100, 150); // left
		DrawText(hdc, _T("����"), _tcslen(_T("����")), &left_rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Rectangle(hdc, 150, 100, 200, 150); // right
		DrawText(hdc, _T("����"), _tcslen(_T("����")), &right_rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Rectangle(hdc, 100, 50, 150, 100); // up
		DrawText(hdc, _T("���"), _tcslen(_T("���")), &up_rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Rectangle(hdc, 100, 150, 150, 200); // down
		DrawText(hdc, _T("�ϴ�"), _tcslen(_T("�ϴ�")), &down_rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (str[0] == VK_LEFT) // str�� ����Ǿ��ִ� Ű ���� ���� ��ĥ
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

	case WM_KEYDOWN:		//Ű�� ������ ��
		hdc = GetDC(hwnd);
		str[0] = wParam;	//���� Ű�� ���� ����
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_KEYUP:			//Ű�� �� ��
		hdc = GetDC(hwnd);
		str[0] = NULL;		//������ Ű�� ���� ����
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}	//ó���� �޽����� case���� ����
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//�������� Ŀ���� ó��
}
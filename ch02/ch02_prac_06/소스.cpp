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
	POINT			point[10] = { {10,200},{250,80},{500,200},{350,350},{150,350} };
	HPEN			hPen, oldPen; // ��
	HBRUSH			hBrush, oldBrush; // ��
	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hPen = CreatePen(PS_DASH, 1, RGB(255, 212, 0)); // �� ����
		hBrush = CreateSolidBrush(RGB(255, 192, 203)); // �귯�� ����
		oldPen = (HPEN)SelectObject(hdc, hPen); // ������ �� ������ ��ȯ�� ���� �� ����
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush); // ������ �귯�� ������ ��ȯ�� ���� �귯�� ����
		Ellipse(hdc, 180, 100, 220, 140); // ��(Ÿ��) �׸���
		SelectObject(hdc, oldPen); // �����صξ��� ���� �� ����
		SelectObject(hdc, oldBrush); // �����صξ��� ���� �귯�� ����
		DeleteObject(hPen); // ������� �� ����
		DeleteObject(hBrush); // ������� �귯�� ����



		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}	//ó���� �޽����� case���� ����
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//�������� Ŀ���� ó��
}
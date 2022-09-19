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
	static TCHAR	str [MAX][100]; //�������� ���ڿ��� ���� ����
	static int		lineCounter[100]; //���ٿ� ��� ���ڰ� �ִ��� ������ ����
	static int		count, yPos;
	int i;
	switch (iMsg)
	{
	case WM_CREATE:
		count = 0;
		yPos = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (i = 0; i <= yPos; i++) // �ݺ������� ����� ��� ���ڿ��� ���
		{
			TextOut(hdc, 0, i*20, str[i], _tcslen(str[i]));
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (yPos >= MAX) break;
		if (wParam == VK_BACK) // �齺���̽�
		{
			if (count > 0)
			{
				count--;
				str[yPos][count] = NULL;
			}
			else if (count == 0) // ���� �ٿ� ���ڰ� ���ٸ�?
			{
				if (yPos == 0) {} // ù ���̸� �ƹ��͵� ���� ����
				else{			
					yPos--;		//�� �ܿ��� ���� ��ĭ �ø���
					count = lineCounter[yPos];	//�����صξ��� ���ڿ� ������ �ҷ���
					count--;
					str[yPos][count] = NULL; // \0 ���ڸ� ����
				}
			}
		}
		else if (wParam == VK_RETURN && yPos < MAX) // ����Ű
		{
			str[yPos][count] = '\0'; // �ι��ڷ� ������ ���� �˸�
			lineCounter[yPos] = count; // ���� ���� ���ڿ� ���̸� ����
			yPos++;		//�� ����
			count = 0;
		}
		else str[yPos][count++] = wParam; // ���ڸ� ������ �� count �� 1�ø���
		if (count >= 99 && yPos< MAX) // �� �࿡ 99�ڸ� �ѱ�� ����
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

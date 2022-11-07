#define _CRT_SECURE_NO_WARNINGS
#include "resource.h"
#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define BSIZE 40

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU4_PRAC_3);	//�޴� �̸�
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
	if (LengthPts(x, y, mx, my) < BSIZE) return TRUE;
	else return FALSE;
}
typedef struct OBJ
{
	char Shape;
	int fx, fy;
	int lx, ly;
	COLORREF penColor = RGB(0, 0, 0);
	COLORREF brushColor = RGB(0, 0, 0);

}OBJ;
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	int answer;

	OPENFILENAME OFN;
	TCHAR str[100], lpstrFile[100] = _T("");
	TCHAR filter[] = _T("C++ �ҽ��� ��� ����\0 *.cpp;*.h\0Every File(*.*) \0*.*\0");

	static OBJ OBJList[100];
	OBJ tmpOBJ;
	static int counter;
	HDC hdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	static int fx, fy;
	static int lx, ly;
	static char currentShape; // L E R

	CHOOSECOLOR COLOR;
	static COLORREF tmp[16], lineColor, brushColor;
	switch (iMsg)
	{
	case WM_CREATE:
		counter = 0;
		currentShape = 'L';
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < counter; i++)
		{
			hBrush = CreateSolidBrush(OBJList[i].brushColor);
			hPen = CreatePen(PS_SOLID, 1, OBJList[i].penColor);
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			oldPen = (HPEN)SelectObject(hdc, hPen);
			switch (OBJList[i].Shape)
			{
			case 'L':
				MoveToEx(hdc, OBJList[i].fx, OBJList[i].fy, NULL);
				LineTo(hdc, OBJList[i].lx, OBJList[i].ly);
				break;
			case 'E':
				Ellipse(hdc, OBJList[i].fx, OBJList[i].fy, OBJList[i].lx, OBJList[i].ly);
				break;
			case 'R':
				Rectangle(hdc, OBJList[i].fx, OBJList[i].fy, OBJList[i].lx, OBJList[i].ly);
				break;
			}
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_LINE:
			currentShape = 'L';
			break;
		case ID_ELLIPSE:
			currentShape = 'E';
			break;
		case ID_RECTANGLE:
			currentShape = 'R';
			break;
		case ID_PENCOLOR:
			for (int i = 0; i < 16; i++)
			{
				tmp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
			}
			memset(&COLOR, 0, sizeof(CHOOSECOLOR));
			COLOR.lStructSize = sizeof(CHOOSECOLOR);
			COLOR.hwndOwner = hwnd;
			COLOR.lpCustColors = tmp;
			COLOR.Flags = CC_FULLOPEN;
			if (ChooseColor(&COLOR) != 0)
			{
				lineColor = COLOR.rgbResult;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			break;
		case ID_FACECOLOR:
			for (int i = 0; i < 16; i++)
			{
				tmp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
			}
			memset(&COLOR, 0, sizeof(CHOOSECOLOR));
			COLOR.lStructSize = sizeof(CHOOSECOLOR);
			COLOR.hwndOwner = hwnd;
			COLOR.lpCustColors = tmp;
			COLOR.Flags = CC_FULLOPEN;
			if (ChooseColor(&COLOR) != 0)
			{
				brushColor = COLOR.rgbResult;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			break;
		case ID_FILENEW:
			MessageBox(hwnd,
				_T("�� ������ ���ڽ��ϱ�?"),
				_T("�� ���� ����"),
				MB_OKCANCEL);
			break;
		case ID_FILEOPEN:
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFilter = filter;
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 100;
			OFN.lpstrInitialDir = _T(".");
			if (GetOpenFileName(&OFN) != 0)
			{
				_stprintf_s(str, _T("%s ������ ���ڽ��ϱ�?"), OFN.lpstrFile);
				MessageBox(hwnd, str, _T("���� ����"), MB_OK);
			}
			break;
		case ID_EXIT:
			answer = MessageBox(hwnd,
				_T("������ �����ϰ� �����ðڽ��ϱ�?"),
				_T("������ ����"),
				MB_YESNOCANCEL);
			if (answer == IDYES || answer == IDNO) PostQuitMessage(0);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		fx = LOWORD(lParam);
		fy = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
	lx = LOWORD(lParam);
	ly = HIWORD(lParam);
	tmpOBJ = { currentShape, fx,fy,lx,ly, lineColor, brushColor };
	OBJList[counter] = tmpOBJ;
	counter++;
	InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
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
	WndClass.hIcon = LoadIcon(NULL, IDI_QUESTION);		//윈도우 아이콘
	WndClass.hCursor = LoadCursor(NULL, IDC_IBEAM);		//커서 모양
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//배경 색
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU4_PRAC_3);	//메뉴 이름
	WndClass.lpszClassName = _T("Window Class Name");	//윈도우 클래스 이름
	RegisterClass(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("Cobaltbru's First Window"),		//윈도우 타이틀 이름
		WS_OVERLAPPEDWINDOW,		//윈도우 스타일
		200,	//윈도우 위치 X
		300,	//윈도우 위치 Y
		600,	//윈도우 가로
		400,	//윈도우 세로
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);		//윈도우 기본 출력 함수
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

BOOL InRectangle(OBJ object, int x, int y)
{
	if (object.fx <= x && object.fy <= y && object.lx >= x && object.ly >= y)
	{
		return true;
	}
	else return false;
}
typedef struct OBJ
{
	char Shape;
	int fx = -1, fy = -1;
	int lx, ly;
	COLORREF penColor = RGB(0, 0, 0);
	COLORREF brushColor = RGB(0, 0, 0);

}OBJ;
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	int answer;

	OPENFILENAME OFN;
	TCHAR str[100], lpstrFile[100] = _T("");
	TCHAR filter[] = _T("C++ 소스와 헤더 파일\0 *.cpp;*.h\0Every File(*.*) \0*.*\0");

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

	static FILE* fp;

	static bool selector;
	static bool currentSelect;
	switch (iMsg)
	{
	case WM_CREATE:
		selector = false;
		currentSelect = -1;
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
		case ID_SELECT:
			if (selector == true) selector = false;
			else selector = true;
			
			break;
		case ID_FILEOPEN:
			fp = fopen("savedata.dat", "rb");
			fread(OBJList, sizeof(OBJ), 100, fp);
			fclose(fp);
			for (int i = 0; i < 100; i++)
			{
				if (OBJList[i].fx == -1 && OBJList[i].fy == -1)
				{
					counter = i;
					break;
				}
			}
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case ID_FILESAVE:
			fp = fopen("savedata.dat", "wb");
			fwrite(OBJList, sizeof(OBJ), 100, fp);
			fclose(fp);
			break;
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
				_T("새 파일을 열겠습니까?"),
				_T("새 파일 선택"),
				MB_OKCANCEL);
			break;
		case ID_EXIT:
			answer = MessageBox(hwnd,
				_T("파일을 저장하고 끝내시겠습니까?"),
				_T("끝내기 선택"),
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
		if (selector == true)
		{
			for (int i = 0; i < counter; i++)
			{
				if (InRectangle(OBJList[i], fx, fy))
				{
					currentSelect = i;
					break;
				}
			}
		}
		else
		{
			lx = LOWORD(lParam);
			ly = HIWORD(lParam);
			tmpOBJ = { currentShape, fx,fy,lx,ly, lineColor, brushColor };
			OBJList[counter] = tmpOBJ;
			counter++;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
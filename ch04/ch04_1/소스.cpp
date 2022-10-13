#define _CRT_SECURE_NO_WARNINGS
#include "resource.h"
#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU4_1);	//메뉴 이름
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

void OutFromFile(TCHAR filename[], HWND hwnd)
{
	FILE*	fPtr;
	HDC		hdc;
	int		line;
	TCHAR	buffer[500];

	line = 0;
	hdc = GetDC(hwnd);

#ifdef _UNICODE
	_tfopen_s(&fPtr, filename, _T("r, ccs = UTF-8"));
#else
	_tfopen_s(&fPtr, filename, _T("r"));
#endif
	while (_fgetts(buffer, 100, fPtr) != NULL)
	{
		if (buffer[_tcslen(buffer) - 1] == _T('\n'))
			buffer[_tcslen(buffer) - 1] = NULL;
		TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
		line++;
	}
	fclose(fPtr);
	ReleaseDC(hwnd, hdc);
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
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC					hdc;
	PAINTSTRUCT			ps;
	int					answer;
	OPENFILENAME		SFN;
	OPENFILENAME		OFN;
	TCHAR				str[100], lpstrFile[100] = _T("");
	TCHAR				filter[] = _T("Every File(*.*) \0*.*\0Text File\0*.txt;*.doc\0");
	CHOOSEFONT			FONT;
	static COLORREF		fColor;
	HFONT				hFont, OldFont;
	static LOGFONT		LogFont;
	CHOOSECOLOR			COLOR;
	static COLORREF		tmp[16], color;
	HBRUSH				hBrush, OldBrush;
	int					i;
	static HMENU		hMenu, hSubMenu;
	int					mx, my;
	static BOOL			Select;
	static BOOL			Copy;
	static int			x, y;
	switch (iMsg)
	{
	case WM_CREATE:
		hMenu = GetMenu(hwnd);
		hSubMenu = GetSubMenu(hMenu, 1);
		EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_EDITPASTE, MF_GRAYED);
		Select = FALSE;
		Copy = FALSE;
		x = 300; y = 300;
		break;
	case WM_PAINT:
		EnableMenuItem(hSubMenu, ID_EDITCOPY,
			Select ? MF_ENABLED : MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_EDITPASTE,
			Copy ? MF_ENABLED : MF_GRAYED);
		hdc = BeginPaint(hwnd, &ps);

		if (Select)
			Rectangle(hdc, x - BSIZE, y - BSIZE, x + BSIZE, y + BSIZE);
		Ellipse(hdc, x - BSIZE, y - BSIZE, x + BSIZE, y + BSIZE);

		hFont = CreateFontIndirect(&LogFont);
		OldFont = (HFONT)SelectObject(hdc, hFont);
		SetTextColor(hdc, fColor);
		TextOut(hdc, 10, 10, _T("HelloWorld"), 10);
		SelectObject(hdc, OldFont);
		DeleteObject(hFont);

		hBrush = CreateSolidBrush(color);
		OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, 10, 20, 200, 200);
		SelectObject(hdc, OldBrush);
		DeleteObject(hBrush);
		EndPaint(hwnd, &ps);


		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EDITCOPY:
			Copy = TRUE;
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case ID_COLORDLG:
			for (i = 0; i < 16; i++)
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
				color = COLOR.rgbResult;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			break;
		case ID_FONTDLG:
			memset(&FONT, 0, sizeof(CHOOSEFONT));
			FONT.lStructSize = sizeof(CHOOSEFONT);
			FONT.hwndOwner = hwnd;
			FONT.lpLogFont = &LogFont;
			FONT.Flags = CF_EFFECTS | CF_SCREENFONTS;
			if (ChooseFont(&FONT) != 0)
			{
				fColor = FONT.rgbColors;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			break;
		case ID_FILESAVE:
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hwnd;
			SFN.lpstrFilter = filter;
			SFN.lpstrFile = lpstrFile;
			SFN.nMaxFile = 256;
			SFN.lpstrInitialDir = _T(".");
			if (GetSaveFileName(&SFN) != 0)
			{
				_stprintf_s(str, _T("%s 파일로 저장하겠습니까?"), SFN.lpstrFile);
				MessageBox(hwnd, str, _T("저장하기 선택"), MB_OK);
			}
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
				_stprintf_s(str, _T("%s 파일을 열겠습니까?"), OFN.lpstrFile);
				MessageBox(hwnd, str, _T("열기 선택"), MB_OK);
				OutFromFile(OFN.lpstrFile, hwnd);
			}
			break;
		case ID_FILENEW:
			MessageBox(hwnd,
				_T("새 파일을 열겠습니까?"),
				_T("새 파일 선택"),
				MB_OKCANCEL);
			break;
		case ID_EXIT:
			answer = MessageBox(hwnd, _T("파일을 저장하고 끝내겠습니까?"),
				_T("끝내기 선택"),
				MB_YESNOCANCEL);
			if (answer == IDYES || answer == IDNO)
				PostQuitMessage(0);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(x, y, mx, my)) Select = TRUE;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
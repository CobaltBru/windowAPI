#include <Windows.h>
#include <tchar.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdline, int nCmdShow) {
    HWND hwnd;
    MSG msg;
    WNDCLASS WndClass;
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = _T("Window Class Name");
    RegisterClass(&WndClass);
    hwnd = CreateWindow(_T("Window Class Name"),
        _T("홍길동의 첫 번째 윈도우"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    static TCHAR str[100];
    static int count;

    switch (iMsg) {
    case WM_CREATE:
        count = 0;
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        TextOut(hdc, 0, 0, str, _tcslen(str));
        EndPaint(hwnd, &ps);
        break;
    case WM_CHAR:
        hdc = GetDC(hwnd);
        //TextOut(hdc, 0, 0, _T("어떤 키를 누를 때 이 문장이 보이나요?"), 22); 아무키나 눌러도 나옴
        str[count++] = wParam;
        str[count] = NULL;
        TextOut(hdc, 0, 0, str, _tcslen(str));
        ReleaseDC(hwnd, hdc);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
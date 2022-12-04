#include "framework.h"
#include <windows.h>
#include <CommCtrl.h>
#include <wchar.h>
#include "resource.h"
#include "MainWindow.h"
#include "Globals.h"

WCHAR szTitle[256];                  // Текст строки заголовка
WCHAR szWindowClass[256];            // имя класса главного окна

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TINELIXIRC));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TINELIXIRC);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINWINDOW), NULL, NULL);

    if (hWnd)
    {
        HMENU hSysMenu = GetSystemMenu(hWnd, FALSE);
        InsertMenu(hSysMenu, 5, MF_BYPOSITION, IDM_ABOUT, TEXT("About..."));
        InsertMenu(hSysMenu, 5, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
        SetWindowText(hWnd, szTitle);
        HWND tabctrl = GetDlgItem(hWnd, IDC_THREADTABS);
        TCITEM tci = { 0 };
        tci.mask = TCIF_TEXT;
        tci.pszText = (LPWSTR)L"Thread";
        TabCtrl_InsertItem(tabctrl, 0, &tci);
        HMENU hMainMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_TINELIXIRC));
        SetMenu(hWnd, hMainMenu);

    }
    else {
        MessageBox(hWnd, TEXT("Something wen\'t wrong..."), TEXT("Error"), MB_ICONERROR | MB_OK);
    }

    return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
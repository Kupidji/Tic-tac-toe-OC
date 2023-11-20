#include "mainScreenDefinitions.h"
#include "iostream";

static string link = "C:\\Users\\kiril\\source\\repos\\WindowsProject1\\config.txt";
static Config cfg;

// Главное окно
HWND hWndMain;
WNDCLASS softwareMainClass;

//int nFromUser = 3;
//cout << "Введите n";
//cin >> nFromUser;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow) {
    loadConfig(cfg, link);

    softwareMainClass =
        NewWindowClass((HBRUSH)CreateSolidBrush(RGB(cfg.color_field.r, cfg.color_field.g, cfg.color_field.b)), LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1)), L"MainWndClass", SoftwareMainProcedure);

    if (!RegisterClassW(&softwareMainClass)) { return -1; }

    MSG softwareMainMessage = { 0 };

    hWndMain =
        CreateWindow(L"MainWndClass", L"First app", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, cfg.width, cfg.height, NULL, NULL, NULL, NULL);

    // Показать и обновить главное окно
    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);

    while (GetMessage(&softwareMainMessage, NULL, NULL, NULL)) {
        TranslateMessage(&softwareMainMessage);
        DispatchMessage(&softwareMainMessage);
    }

    return 0;
}

WNDCLASS NewWindowClass(
    HBRUSH backgrondColor,
    HCURSOR cursor,
    HINSTANCE hInst,
    HICON icon,
    LPCWSTR name,
    WNDPROC procedure
) {
    WNDCLASS newWindowClass = { 0 };

    newWindowClass.hCursor = cursor;
    newWindowClass.hInstance = hInst;
    newWindowClass.hIcon = icon;
    newWindowClass.lpszClassName = name;
    newWindowClass.hbrBackground = backgrondColor;
    newWindowClass.lpfnWndProc = procedure;

    return newWindowClass;
}


LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

    static Rgb linesColor = { cfg.color_line.r, cfg.color_line.g, cfg.color_line.b };
    PAINTSTRUCT ps;
    static int n = cfg.n;
    static int** grid;

    switch (msg) {
    case WM_CREATE: {
        grid = new int* [n];
        for (int i = 0; i < n; i++) {
            grid[i] = new int[n];
            memset(grid[i], 0, sizeof(int) * n);
        }
        break;
    }

    case WM_PAINT: {
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        HBRUSH hBrush = CreateSolidBrush(RGB(linesColor.r, linesColor.g, linesColor.b));
        cfg.color_line = linesColor;
        int gridWidth = clientRect.right / n;
        int gridHeight = clientRect.bottom / n;

        for (int x = 0; x < clientRect.right; x += gridWidth) {
            for (int y = 0; y < clientRect.bottom; y += gridHeight) {
                int row = y / gridHeight;
                int col = x / gridWidth;
                //при n = 3 вылазит за границы массива
                if (row == n) {
                    row = n-1;
                }
                if (col == n) {
                    col = n-1;
                }

                RECT cellRect = { x, y, x + gridWidth, y + gridHeight };
                FrameRect(hdc, &cellRect, hBrush);

                if (grid[row][col] == 1) {
                    MoveToEx(hdc, x, y, NULL);
                    LineTo(hdc, x + gridWidth, y + gridHeight);
                    MoveToEx(hdc, x + gridWidth, y, NULL);
                    LineTo(hdc, x, y + gridHeight);
                }
                else if (grid[row][col] == 2) {
                    Ellipse(hdc, x, y, x + gridWidth, y + gridHeight);
                }
            }
        }
        EndPaint(hWnd, &ps);
        DeleteObject(hBrush);
        break;
    }

    case WM_SIZE: {
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_LBUTTONDOWN: {
        int xPos = GET_X_LPARAM(lp);
        int yPos = GET_Y_LPARAM(lp);

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        int gridWidth = clientRect.right / n;
        int gridHeight = clientRect.bottom / n;

        int row = yPos / gridHeight;
        int col = xPos / gridWidth;

        if (grid[row][col] == 0) {
            grid[row][col] = 1;
            InvalidateRect(hWnd, NULL, TRUE);
        }

        break;
    }

    case WM_RBUTTONDOWN: {
        int xPos = GET_X_LPARAM(lp);
        int yPos = GET_Y_LPARAM(lp);

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        int gridWidth = clientRect.right / n;
        int gridHeight = clientRect.bottom / n;

        int row = yPos / gridHeight;
        int col = xPos / gridWidth;

        if (grid[row][col] == 0) {
            grid[row][col] = 2;
            InvalidateRect(hWnd, NULL, TRUE);
        }

        break;
    }

    case WM_KEYDOWN: {
        if (wp == VK_RETURN) {
            Rgb colorRgb = { getRandomColorr(), getRandomColorr(), getRandomColorr() };
            SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)(HBRUSH)CreateSolidBrush(RGB(colorRgb.r, colorRgb.g, colorRgb.b)));
            cfg.color_field = colorRgb;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wp == VK_ESCAPE) {
            DestroyWindow(hWnd);
        }
        else if ((GetKeyState(VK_CONTROL) & 0x8000) && (wp == 'Q' || wp == 'q')) {
            DestroyWindow(hWnd);
        }
        else if ((GetKeyState(VK_SHIFT) & 0x8000) && (wp == 'C' || wp == 'c')) {
            runNotepad();
        }

        break;
    }

    case WM_MOUSEWHEEL: {
        changeLineColors(linesColor);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_DESTROY: {
        RECT clientRect;
        GetWindowRect(hWnd, &clientRect);
        cfg.n = n;
        cfg.width = clientRect.right - clientRect.left;
        cfg.height = clientRect.bottom - clientRect.top;
        saveConfig(cfg, link);

        for (int i = 0; i < n; i++) {
            delete[] grid[i];
        }
        delete[] grid;

        PostQuitMessage(0);
        return 0; 
    }

    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    
    
    DeleteObject(hWndMain);
    return 0;
}
#include "mainScreenDefinitions.h"
#include <iostream>
#include "StringToLpcwstr.h";


static Config cfg;

auto APP_NAME = L"Крестики-нолики";

// Главное окно
HWND hWndMain;
WNDCLASS softwareMainClass;

//Поля приложения
static int** grid;
static int n;

//для fileMapping
HANDLE hMapFile;
int* pData;
// ID для сообщения broadcast
UINT WM_FIELD_CHANGED;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow) {
    resetData(cfg);
    n = cfg.n;
    WM_FIELD_CHANGED = RegisterWindowMessage(L"WM_FIELD_CHANGED");

    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,   // Идентификатор файла
        NULL,                   // Защита
        PAGE_READWRITE,         // Доступ
        0,                      // Размер файла (0 - динамический)
        sizeof(int) * n * n,    // Размер fileMapping
        L"MyMappingObject");    // Имя fileMapping

    if (hMapFile == NULL) {
        cout << "Не удалось создать File Mapping!" << std::endl;
        return 1;
    }

    pData = (int*)MapViewOfFile(hMapFile, FILE_MAP_WRITE, 0, 0, sizeof(grid) * n * n);
    if (pData == NULL) {
        std::cout << "Не удалось получить указатель на представленную область памяти!" << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    //Инициализация поля, если приложение уже запущено - инициализируем текущий прогресс
    initGrid(grid, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (pData[i * n + j] != 0) {
                grid[i][j] = pData[i * n + j];
            }
        }
    }

    softwareMainClass =
        NewWindowClass((HBRUSH)CreateSolidBrush(RGB(cfg.color_field.r, cfg.color_field.g, cfg.color_field.b)), LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1)), L"MainWndClass", SoftwareMainProcedure);

    if (!RegisterClassW(&softwareMainClass)) { return -1; }

    hWndMain =
        CreateWindow(L"MainWndClass", APP_NAME, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, cfg.width, cfg.height, NULL, NULL, NULL, NULL);

    // Показать и обновить главное окно
    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);
    
    MSG softwareMainMessage = { 0 };

    while (GetMessage(&softwareMainMessage, NULL, NULL, NULL)) {
        TranslateMessage(&softwareMainMessage);
        DispatchMessage(&softwareMainMessage);
    }

    UnmapViewOfFile(pData);
    CloseHandle(hMapFile);

    DestroyWindow(hWndMain);
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
    
    if (msg == WM_FIELD_CHANGED) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                grid[i][j] = pData[i * n + j];
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);
    }

    switch (msg) {

        case WM_CREATE: {
            break;
        }
    
        case WM_PAINT: {
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            HBRUSH hBrush = CreateSolidBrush(RGB(linesColor.r, linesColor.g, linesColor.b));

            int gridWidth = 0;
            int gridHeight = 0;

            getSizeOfGrid(clientRect, gridWidth, gridHeight, n);

            //Отрисовка сетки, крестиков и ноликов
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

                    //Крестики
                    if (grid[row][col] == 1) {
                        drawTic(x, y, hdc, gridWidth, gridHeight);
                    }
                    //Нолики
                    else if (grid[row][col] == 2) {
                        drawTac(x, y, hdc, gridWidth, gridHeight);
                    }
                }
            }
            EndPaint(hWnd, &ps);

            //сохранение цвета линий в конфиг
            cfg.color_line = linesColor;
            setCurrentData(cfg);

            DeleteObject(hBrush);
            break;
        }

        case WM_SIZE: {
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
    
        //Поставить крестик 
        case WM_LBUTTONDOWN: {
            int xPos = GET_X_LPARAM(lp);
            int yPos = GET_Y_LPARAM(lp);
            int gridWidth = 0;
            int gridHeight = 0;
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);

            getSizeOfGrid(clientRect, gridWidth, gridHeight, n);

            int row = yPos / gridHeight;
            int col = xPos / gridWidth;

            if (grid[row][col] == 0) {
                pData[row * n + col] = 1;
                SendMessage(HWND_BROADCAST, WM_FIELD_CHANGED, 0, 0);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        }
    
        //Поставить нолик
        case WM_RBUTTONDOWN: {
            int xPos = GET_X_LPARAM(lp);
            int yPos = GET_Y_LPARAM(lp);
            int gridWidth = 0;
            int gridHeight = 0;
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);

            getSizeOfGrid(clientRect, gridWidth, gridHeight, n);

            int row = yPos / gridHeight;
            int col = xPos / gridWidth;

            if (grid[row][col] == 0) {
                pData[row * n + col] = 2;          
                SendMessage(HWND_BROADCAST, WM_FIELD_CHANGED, 0, 0);
                InvalidateRect(hWnd, NULL, TRUE);
            }

            break;
        }

        case WM_KEYDOWN: {
            if (wp == VK_RETURN) {
                changeBackgroundColor(hWnd, cfg);
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
            saveData(clientRect);

            PostQuitMessage(0);
            return 0; 
        }

        default:
            return DefWindowProc(hWnd, msg, wp, lp);
    }
    
    DeleteObject(hWndMain);
    return 0;
}


void drawTic(int x, int y, HDC hdc, int gridWidth, int gridHeight) {
    MoveToEx(hdc, x, y, NULL);
    LineTo(hdc, x + gridWidth, y + gridHeight);
    MoveToEx(hdc, x + gridWidth, y, NULL);
    LineTo(hdc, x, y + gridHeight);
}

void drawTac(int x, int y, HDC hdc, int gridWidth, int gridHeight) {
    int right = x + gridWidth;
    int bottom = y + gridHeight;
    Ellipse(hdc, x, y, right, bottom);
}

void getSizeOfGrid(RECT clientRect, int& gridWidth, int& gridHeight, int n) {
    gridWidth = clientRect.right / n;
    gridHeight = clientRect.bottom / n;
}
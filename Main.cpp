#include "mainScreenDefinitions.h"

#define STACK_SIZE (64*1024)

static Config cfg;

const wchar_t* APP_NAME = L"Крестики";
auto APP_NAME2 = L"Нолики";

HANDLE gameoverMutex;
HANDLE ticThread;
HANDLE tacThread;
HANDLE animationThread;
int priority;

HANDLE hMapFileProcess;
int* pDataProcess;

// Главное окно
HWND hWndMain;
WNDCLASS softwareMainClass;

//Поля приложения
static int** grid;
static int n;
static int countOfMoves = 0;
static bool backgroundAnimationState = true;

//для fileMapping
HANDLE hMapFile;
int* pData;
// ID для сообщения broadcast
UINT WM_FIELD_CHANGED;

UINT WM_FINISH_ALL_PROCESS;

void createFirst() {
    HINSTANCE hInst = GetModuleHandle(NULL);
    createWindow(hInst, NULL, 0, TRUE, APP_NAME, 100, 100);
}

void createSecond() {
    HINSTANCE hInst2 = GetModuleHandle(NULL);
    createWindow(hInst2, NULL, 0, TRUE, APP_NAME2, 100 + cfg.width, 100);
}

#pragma comment(linker, "/entry:mainCRTStartup")
int main() {
    WM_FIELD_CHANGED = RegisterWindowMessage(L"WM_FIELD_CHANGED");
    WM_FINISH_ALL_PROCESS = RegisterWindowMessage(L"WM_FINISH_ALL_APP");

    
    hMapFileProcess = CreateFileMapping(
        INVALID_HANDLE_VALUE,   // Идентификатор файла
        NULL,                   // Защита
        PAGE_READWRITE,         // Доступ
        0,                      // Размер файла (0 - динамический)
        sizeof(int),    // Размер fileMapping
        L"ProcessMappingObject");    // Имя fileMapping

    if (hMapFileProcess == NULL) {
        cout << "Не удалось создать File Mapping!" << std::endl;
        return 1;
    }

    pDataProcess = (int*)MapViewOfFile(hMapFileProcess, FILE_MAP_WRITE, 0, 0, sizeof(int));
    if (pDataProcess == NULL) {
        std::cout << "Не удалось получить указатель на представленную область памяти!" << std::endl;
        CloseHandle(hMapFileProcess);
        return 1;
    }

    pDataProcess[0]++;

    //Восстанавливаю состояние последнего запуска.
    resetData(cfg);

    if (pDataProcess[0] == 1) {
        if (ticThread == NULL) {
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));
            // CMD command here
            char arg[] = "C:\\Users\\kiril\\source\\repos\\WindowsProject1\\x64\\Debug\\WindowsProject1.exe";
            // Convert char string to required LPWSTR string
            wchar_t text[500];
            mbstowcs(text, arg, strlen(arg) + 1);
            LPWSTR command = text;
            // Run process
            if (CreateProcess(NULL, command, NULL, NULL, 0, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
                DuplicateHandle(pi.hProcess, pi.hThread, pi.hProcess,
                    &ticThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
                createFirst();
                
            }
            
        }
    }
    
    if (pDataProcess[0] == 2) {
        if (tacThread == NULL) {
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));
            // CMD command here
            char arg[] = "C:\\Users\\kiril\\source\\repos\\WindowsProject1\\x64\\Debug\\WindowsProject1.exe";
            // Convert char string to required LPWSTR string
            wchar_t text[500];
            mbstowcs(text, arg, strlen(arg) + 1);
            LPWSTR command = text;
            // Run process
            if (CreateProcess(NULL, command, NULL, NULL, 0, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
                DuplicateHandle(pi.hProcess, pi.hThread, pi.hProcess,
                    &tacThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
                createSecond();
            }
            
        }
    }

    if (pDataProcess[0] > 3) {
        ::MessageBoxW(nullptr, L"Игра уже запущена!", L"Ошибка", MB_OK);
    }

}


int WINAPI createWindow(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow, const wchar_t* appName, int posX, int posY) {
    gameoverMutex = CreateMutex(NULL, NULL, L"Global\MyMutex");

    if (gameoverMutex == NULL) {
        gameoverMutex = OpenMutex(
            MUTEX_ALL_ACCESS,
            FALSE,
            L"Global\MyMutex");
    }

    n = cfg.n;

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
                //если есть число, отличное от нуля -> был произведен ход.
                countOfMoves++;
            }
        }
    }

    softwareMainClass =
        NewWindowClass((HBRUSH)CreateSolidBrush(RGB(cfg.color_field.r, cfg.color_field.g, cfg.color_field.b)), LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1)), appName, SoftwareMainProcedure);
    
    if (!RegisterClassW(&softwareMainClass)) { return -1; }
    hWndMain =
        CreateWindow(appName, appName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, posX, posY, cfg.width, cfg.height, NULL, NULL, NULL, NULL);
    
    // Показать и обновить главное окно
    ShowWindow(hWndMain, SW_SHOW);
    UpdateWindow(hWndMain);

    animationThread = CreateThread(NULL, STACK_SIZE, changeBackgroundAnimation, NULL, 0, NULL);

    MSG softwareMainMessage = { 0 };
    
    while (GetMessage(&softwareMainMessage, NULL, NULL, NULL)) {
        TranslateMessage(&softwareMainMessage);
        DispatchMessage(&softwareMainMessage);
    }

    UnmapViewOfFile(pData);
    CloseHandle(hMapFile);

    UnmapViewOfFile(pDataProcess);
    CloseHandle(hMapFileProcess);

    DeleteObject(ticThread);
    DeleteObject(tacThread);
    DeleteObject(animationThread);
    DeleteObject(gameoverMutex);

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
        countOfMoves++;
        checkFieldToWin(grid, n, countOfMoves, gameoverMutex);
        return 0;
    }

    if (msg == WM_FINISH_ALL_PROCESS) {
        //ExitProcess(0);
        PostQuitMessage(0);
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

    //Поставить крестик или нолик
    case WM_LBUTTONDOWN: {
        if (countOfMoves % 2 == 0 && softwareMainClass.lpszClassName == APP_NAME) {
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
            }
            return 0;
        }
        if (countOfMoves % 2 == 1 && softwareMainClass.lpszClassName == APP_NAME) {
            MessageBox(NULL, _T("Cейчас ход ноликов!"), _T("TickTackToe"),
                MB_OK | MB_SETFOREGROUND);
            return 0;
        }

        if (countOfMoves % 2 == 1 && softwareMainClass.lpszClassName == APP_NAME2) {
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
            }
            return 0;
        }
        if (countOfMoves % 2 == 0 && softwareMainClass.lpszClassName == APP_NAME2) {
            MessageBox(NULL, _T("Cейчас ход крестиков!"), _T("TickTackToe"),
                MB_OK | MB_SETFOREGROUND);
            return 0;
        }

        break;
    }

    case WM_KEYDOWN: {
        if (wp == 0x31) { //1 (самый низкий приоритет)
            SetThreadPriority(animationThread, THREAD_PRIORITY_IDLE);
            wstring c = L"Приоритет теперь:  " + to_wstring(GetThreadPriority(animationThread));
            MessageBox(NULL, c.c_str(), _T("TickTackToe"),
                MB_OK | MB_SETFOREGROUND);

        }

        else if (wp == 0x32) { //2 (самый высокий приоритет)
            SetThreadPriority(animationThread, THREAD_PRIORITY_TIME_CRITICAL);
            wstring c = L"Приоритет теперь:  " + to_wstring(GetThreadPriority(animationThread));
            MessageBox(NULL, c.c_str(), _T("TickTackToe"),
                MB_OK | MB_SETFOREGROUND);
        }

        else if (wp == 0x33) { //3 (нормальный приоритет)
            SetThreadPriority(animationThread, THREAD_PRIORITY_NORMAL);
            wstring c = L"Приоритет теперь: " + to_wstring(GetThreadPriority(animationThread));
            MessageBox(NULL, c.c_str(), _T("TickTackToe"),
                MB_OK | MB_SETFOREGROUND);
        }

        else if (wp == VK_SPACE) { // для остановки/возобновления градиента
            backgroundAnimationState = !backgroundAnimationState;
            if (!backgroundAnimationState) 
                SuspendThread(animationThread);
            else 
                ResumeThread(animationThread);
        }

        else if (wp == VK_RETURN) {
            changeBackgroundColor(cfg);
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
        SendMessage(HWND_BROADCAST, WM_FINISH_ALL_PROCESS, 0, 0);
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

//поток для заднего фона
DWORD WINAPI changeBackgroundAnimation(LPVOID) {
    while (true) {
        if (backgroundAnimationState) {
            changeBackgroundColor(cfg);
            SetClassLongPtr(hWndMain, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(cfg.color_field.r, cfg.color_field.g, cfg.color_field.b)));
        }
        InvalidateRect(hWndMain, NULL, TRUE);
        
        Sleep(50);
    }
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

void changeBackgroundColor(Config& cfg) {
    getRgbColor(cfg.color_field, 5);
}

void getRgbColor(Rgb& backgroundColor, int delta) {
    if (delta > 0) {
        if (backgroundColor.r >= 255 && backgroundColor.g < 255 && backgroundColor.b <= 0)
            backgroundColor.g += delta;
        else if (backgroundColor.r > 0 && backgroundColor.g >= 255 && backgroundColor.b <= 0)
            backgroundColor.r -= delta;
        else if (backgroundColor.r <= 0 && backgroundColor.g >= 255 && backgroundColor.b < 255)
            backgroundColor.b += delta;
        else if (backgroundColor.r <= 0 && backgroundColor.g > 0 && backgroundColor.b >= 255)
            backgroundColor.g -= delta;
        else if (backgroundColor.r < 255 && backgroundColor.g <= 0 && backgroundColor.b >= 255)
            backgroundColor.r += delta;
        else if (backgroundColor.r >= 255 && backgroundColor.g <= 0 && backgroundColor.b > 0)
            backgroundColor.b -= delta;
    }
    else {
        if (backgroundColor.r >= 255 && backgroundColor.g > 0 && backgroundColor.b <= 0)
            backgroundColor.g += delta;
        else if (backgroundColor.r < 255 && backgroundColor.g >= 255 && backgroundColor.b <= 0)
            backgroundColor.r -= delta;
        else if (backgroundColor.r <= 0 && backgroundColor.g >= 255 && backgroundColor.b > 0)
            backgroundColor.b += delta;
        else if (backgroundColor.r <= 0 && backgroundColor.g < 255 && backgroundColor.b >= 255)
            backgroundColor.g -= delta;
        else if (backgroundColor.r > 0 && backgroundColor.g <= 0 && backgroundColor.b >= 255)
            backgroundColor.r += delta;
        else if (backgroundColor.r >= 255 && backgroundColor.g <= 0 && backgroundColor.b < 255)
            backgroundColor.b -= delta;
    }
    if (backgroundColor.r < 0)
        backgroundColor.r = 255;
    else if (backgroundColor.r > 255)
        backgroundColor.r = 0;

    if (backgroundColor.b < 0)
        backgroundColor.r = 255;
    else if (backgroundColor.r > 255)
        backgroundColor.r = 0;

    if (backgroundColor.g < 0)
        backgroundColor.r = 255;
    else if (backgroundColor.r > 255)
        backgroundColor.r = 0;
}

void changeLineColors(Rgb& rgb) {
    if (rgb.r == 255) {
        rgb.r = 0;
    }
    else {
        rgb.r++;
    }

    if (rgb.g == 255) {
        rgb.g = 0;
    }
    else {
        rgb.g += 5;
    }

    if (rgb.b >= 255) {
        rgb.b = 0;
    }
    else {
        rgb.b += 10;
    }
}
#include "MainViewmodel.h"

#include "mainScreenUsecases.h";
#include <tchar.h>

CONST string LINK = "C:\\Users\\kiril\\source\\repos\\WindowsProject1\\config.txt";
Config currentConfig;

void initGrid(int**& grid, int n) {
	grid = new int* [n];
	for (int i = 0; i < n; i++) {
		grid[i] = new int[n];
		memset(grid[i], 0, sizeof(int) * n);
	}
}

void deleteGrid(int**& grid, int n) {
	for (int i = 0; i < n; i++) {
		delete[] grid[i];
	}
	delete[] grid;
}

void resetData(Config &cfg) {
	loadDataBytTxtExecute(cfg, LINK);
	//loadDataFileMappingExecute(cfg, LINK);
	//loadDataWinApiExecute(cfg, LINK);

	currentConfig = cfg;
}

void setCurrentData(Config cfg) {
	currentConfig = cfg;
}

void saveData(RECT clientRect) {
	currentConfig.width = clientRect.right - clientRect.left;
	currentConfig.height = clientRect.bottom - clientRect.top;

	saveDataBytTxtExecute(currentConfig, LINK);
	//saveDataFileMapping(currentConfig, LINK);
	//saveDataWinApiExecute(currentConfig, LINK);
}

void runNotepad() {
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&sInfo, sizeof(STARTUPINFO));
	CreateProcess(_T("C:\\Windows\\Notepad.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
}

void checkFieldToWin(int** array, int n, int countOfMoves, HANDLE gameoverMutex) {
    if (checkDiagonal(array, 1, n) || checkLine(array, 1, n)) {
        if (WaitForSingleObject(gameoverMutex, 0) == WAIT_TIMEOUT) {
            PostQuitMessage(0);
            return;
        }

        MessageBox(NULL, _T("Игра закончилась победой крестиков!"), _T("TickTackToe"),
            MB_OK | MB_SETFOREGROUND);

        PostQuitMessage(0);
    }

    if (checkDiagonal(array, 2, n) || checkLine(array, 2, n)) {
        if (WaitForSingleObject(gameoverMutex, 0) == WAIT_TIMEOUT) {
            PostQuitMessage(0);
            return;
        }

        MessageBox(NULL, _T("Игра закончилась победой ноликов!"), _T("TickTackToe"),
            MB_OK | MB_SETFOREGROUND);

        PostQuitMessage(0);
    }

    if (countOfMoves >= (n * n)) {
        if (WaitForSingleObject(gameoverMutex, 0) == WAIT_TIMEOUT) {
            PostQuitMessage(0);
            return;
        }

        MessageBox(NULL, _T("Игра закончилась ничьёй!"), _T("TickTackToe"),
            MB_OK | MB_SETFOREGROUND);

        PostQuitMessage(0);
    }

}
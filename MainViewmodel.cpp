#include "MainViewmodel.h"

#include "mainScreenUsecases.h";
#include "GetRandomColor.h";
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
	currentConfig = cfg;
}

void setCurrentData(Config cfg) {
	currentConfig = cfg;
}

void saveData(RECT clientRect) {
	currentConfig.width = clientRect.right - clientRect.left;
	currentConfig.height = clientRect.bottom - clientRect.top;
	saveDataBytTxtExecute(currentConfig, LINK);
}

void changeBackgroundColor(HWND hWnd, Config &cfg) {
	Rgb colorRgb = { getRandomColor(), getRandomColor(), getRandomColor() };
	SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)(HBRUSH)CreateSolidBrush(RGB(colorRgb.r, colorRgb.g, colorRgb.b)));
	cfg.color_field = colorRgb;
	currentConfig.color_field = colorRgb;;
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

void runNotepad() {
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&sInfo, sizeof(STARTUPINFO));
	CreateProcess(_T("C:\\Windows\\Notepad.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
}
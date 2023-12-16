#pragma once
#include "framework.h"
#include "Resource.h"
#include <windowsx.h>
#include <stdio.h>
#include "GetRandomColor.h";
#include <iostream>
#include "mainScreenUsecases.h"
#include "MainViewmodel.h"

WNDCLASS NewWindowClass(
	HBRUSH backgrondColor,
	HCURSOR cursor,
	HINSTANCE hInst,
	HICON icon,
	LPCWSTR name,
	WNDPROC procedure
);
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

int WINAPI createWindow(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow, const wchar_t* appName, int posX, int posY);
DWORD WINAPI changeBackgroundAnimation(LPVOID);

void drawTic(int x, int y, HDC hdc, int gridWidth, int gridHeight); 
void drawTac(int x, int y, HDC hdc, int gridWidth, int gridHeight);
void getSizeOfGrid(RECT clientRect, int& gridWidth, int& gridHeight, int n);
void changeBackgroundColor(Config& cfg);
void getRgbColor(Rgb& backgroundColor, int delta);
void changeLineColors(Rgb& rgb);
void checkFieldToWin(int** array, int n, int countOfMoves, HANDLE gameoverMutex);
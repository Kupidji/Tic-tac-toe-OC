#pragma once
#include "framework.h"
#include "Resource.h"
#include <windowsx.h>
#include <stdio.h>
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

void drawTic(int x, int y, HDC hdc, int gridWidth, int gridHeight); 
void drawTac(int x, int y, HDC hdc, int gridWidth, int gridHeight);
void getSizeOfGrid(RECT clientRect, int& gridWidth, int& gridHeight, int n);
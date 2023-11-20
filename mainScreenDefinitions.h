#pragma once
#include "framework.h"
#include "Resource.h"
#include <windowsx.h>
#include <stdio.h>
#include "mainScreenUsecases.h"
#include "ConfigStorageRepository.h"
#include "getRandomColor.h";

WNDCLASS NewWindowClass(
	HBRUSH backgrondColor,
	HCURSOR cursor,
	HINSTANCE hInst,
	HICON icon,
	LPCWSTR name,
	WNDPROC procedure
);
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

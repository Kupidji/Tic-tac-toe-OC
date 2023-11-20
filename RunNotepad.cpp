#include <windows.h>;
#include <tchar.h>
#include "mainScreenUsecases.h";

void runNotepad(void) {
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&sInfo, sizeof(STARTUPINFO));
	CreateProcess(_T("C:\\Windows\\Notepad.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
}
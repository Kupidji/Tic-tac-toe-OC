#include "SaveDataWinApi.h";

#include "windows.h";
#include "StringToLpcwstr.h";

void saveDataWinApiExecute(Config cfg, string link) {
	wstring temp = wstring(link.begin(), link.end());
	LPCWSTR wideString = temp.c_str();
	saveConfigWinApi(cfg, wideString);
}
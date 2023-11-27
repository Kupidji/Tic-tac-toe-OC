#include "LoadDataWinApi.h";

#include "windows.h";
#include "StringToLpcwstr.h";

void loadDataWinApiExecute(Config& cfg, string link) {	
	wstring temp = wstring(link.begin(), link.end());
	LPCWSTR wideString = temp.c_str();
	loadConfigWinApi(cfg, wideString);
}
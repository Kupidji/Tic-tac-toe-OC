#include "StringToLpcwstr.h";

LPCWSTR stringToLpcwstr(string str) {
	wstring temp = wstring(str.begin(), str.end());
	LPCWSTR wideString = temp.c_str();
	return wideString;
}
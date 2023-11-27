#pragma once

#include "Config.h";
#include <string>
#include "windows.h"

using namespace std;

void saveConfigTxt(Config cfg, string link);
void loadConfigTxt(Config& cfg, string link);

void saveConfigMapping(Config cfg, string link);
void loadConfigMapping(Config& cfg, string link);

void saveConfigWinApi(Config cfg, LPCWSTR link);
void loadConfigWinApi(Config& cfg, LPCWSTR link);
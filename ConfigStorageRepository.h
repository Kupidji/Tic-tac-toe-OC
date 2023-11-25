#pragma once

#include "Config.h";
#include <string>

using namespace std;

void saveConfigTxt(Config cfg, string link);
void loadConfigTxt(Config& cfg, string link);
void loadConfigMapping(Config& cfg, string link);
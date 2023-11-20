#pragma once

#include "Config.h";
#include <string>

using namespace std;

void saveConfig(Config cfg, string link);
void loadConfig(Config& cfg, string link);
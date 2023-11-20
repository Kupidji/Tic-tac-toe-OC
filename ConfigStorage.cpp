#include "ConfigStorageRepository.h"

#include <iostream>
#include <fstream>
#include "Config.h"
#include <windows.h>
#include <sstream> 
#include <string>
#include <io.h>

using namespace std;

void saveConfig(Config cfg, string link) {
	std::ofstream fout(link);
	fout << "n = " << cfg.n << "\n";
	fout << "width = " << cfg.width << "\n";
	fout << "height = " << cfg.height << "\n";
	fout << "color_line = " << cfg.color_line.r << " " << cfg.color_line.g << " " << cfg.color_line.b << "\n";
	fout << "color_field = " << cfg.color_field.r << " " << cfg.color_field.g << " " << cfg.color_field.b << "\n";
	fout.close();
}

bool FileExists(const char* fname) {
	return _access(fname, 0) != -1;
}

void loadConfig(Config& cfg, string link)
{
	std::ifstream fin(link);
	if (FileExists(link.c_str()))
	{
		std::string line;
		while (std::getline(fin, line)) {
			std::istringstream sin(line.substr(line.find("=") + 1));
			if (line.find("n ") != -1)
				sin >> cfg.n;
			else if (line.find("width") != -1)
				sin >> cfg.width;
			else if (line.find("height") != -1)
				sin >> cfg.height;
			else if (line.find("color_line") != -1)
			{
				sin >> cfg.color_line.r;
				sin >> cfg.color_line.g;
				sin >> cfg.color_line.b;
			}
			else if (line.find("color_field") != -1)
			{
				sin >> cfg.color_field.r;
				sin >> cfg.color_field.g;
				sin >> cfg.color_field.b;
			}
		}
		fin.close();
	}
	else {
		saveConfig(cfg, link);
	}

}
#include "ConfigStorageRepository.h"

#include <iostream>
#include <fstream>
#include "Config.h"
#include <windows.h>
#include <sstream> 
#include <string>
#include <io.h>

using namespace std;

const string N = "N = ";
const string WIDTH = "WIDTH = ";
const string HEIGHT = "HEIGHT = ";
const string LINE_COLOR = "LINE_COLOR = ";
const string FIELD_COLOR = "FIELD_COLOR = ";

void saveConfigTxt(Config cfg, string link) {
	std::ofstream fout(link);
	fout << N << cfg.n << "\n";
	fout << WIDTH << cfg.width << "\n";
	fout << HEIGHT << cfg.height << "\n";
	fout << LINE_COLOR << cfg.color_line.r << " " << cfg.color_line.g << " " << cfg.color_line.b << "\n";
	fout << FIELD_COLOR << cfg.color_field.r << " " << cfg.color_field.g << " " << cfg.color_field.b << "\n";
	fout.close();
}

bool fileExists(const char* fname) {
	return _access(fname, 0) != -1;
}

void loadConfigTxt(Config& cfg, string link) {
	std::ifstream fin(link);
	if (fileExists(link.c_str())) {
		std::string line;
		while (std::getline(fin, line)) {
			std::istringstream sin(line.substr(line.find("=") + 1));
			if (line.find(N) != -1)
				sin >> cfg.n;
			else if (line.find(WIDTH) != -1)
				sin >> cfg.width;
			else if (line.find(HEIGHT) != -1)
				sin >> cfg.height;
			else if (line.find(LINE_COLOR) != -1) {
				sin >> cfg.color_line.r;
				sin >> cfg.color_line.g;
				sin >> cfg.color_line.b;
			}
			else if (line.find(FIELD_COLOR) != -1) {
				sin >> cfg.color_field.r;
				sin >> cfg.color_field.g;
				sin >> cfg.color_field.b;
			}
		}
		fin.close();
	}
	else {
		saveConfigTxt(cfg, link);
	}

}

void loadConfigMapping(Config& cfg, string link) {
    if (fileExists(link.c_str())) {
        HANDLE hFile = CreateFileA(link.c_str(), FILE_GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
            if (hMapping != NULL) {
                LPVOID pData = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
                if (pData != NULL) {
                    std::stringstream ss(static_cast<const char*>(pData));
                    std::string line;
                    while (std::getline(ss, line)) {
                        std::istringstream sin(line.substr(line.find("=") + 1));
                        if (line.find(N) != -1)
                            sin >> cfg.n;
                        else if (line.find(WIDTH) != -1)
                            sin >> cfg.width;
                        else if (line.find(HEIGHT) != -1)
                            sin >> cfg.height;
                        else if (line.find(LINE_COLOR) != -1) {
                            sin >> cfg.color_line.r;
                            sin >> cfg.color_line.g;
                            sin >> cfg.color_line.b;
                        }
                        else if (line.find(FIELD_COLOR) != -1) {
                            sin >> cfg.color_field.r;
                            sin >> cfg.color_field.g;
                            sin >> cfg.color_field.b;
                        }
                    }
                    UnmapViewOfFile(pData);
                }
                CloseHandle(hMapping);
            }
            CloseHandle(hFile);
        }
    }
    else {
        saveConfigTxt(cfg, link);
    }
}
#include "Rgb.h";
#include "Config.h";
#include "windows.h";

void initGrid(int**& grid, int n);
void deleteGrid(int**& grid, int n);
void resetData(Config& cfg);
void setCurrentData(Config cfg);
void saveData(RECT clientRect);
void changeBackgroundColor(HWND hWnd, Config& cfg);
void changeLineColors(Rgb& rgb);
void runNotepad();
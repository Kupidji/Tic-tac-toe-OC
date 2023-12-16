#include "CheckLine.h";

bool checkLine(int** array, int symb, int n) {
    bool cols = true;
    bool rows = true;
    for (int column = 0; column < n; column++) {
        cols = true;
        rows = true;
        for (int row = 0; row < n; row++) {
            cols &= (array[column][row] == symb);
            rows &= (array[row][column] == symb);
        }

        if (cols || rows) return true;
    }

    return false;
}
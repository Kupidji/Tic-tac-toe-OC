#include "CheckDiagonal.h";

bool checkDiagonal(int** array, int symb, int n) {
    bool toright = true;
    bool toleft = true;
    for (int i = 0; i < n; i++) {
        toright &= (array[i][i] == symb);
        toleft &= (array[n - i - 1][i] == symb);
    }
    if (toright || toleft) return true;

    return false;
}
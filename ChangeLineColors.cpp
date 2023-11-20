#include "mainScreenUsecases.h";

void changeLineColors(Rgb &rgb) {
	if (rgb.r == 255) {
		rgb.r = 0;
	}
	else {
		rgb.r++;
	}

	if (rgb.g == 255) {
		rgb.g = 0;
	}
	else {
		rgb.g += 5;
	}

	if (rgb.b >= 255) {
		rgb.b = 0;
	}
	else {
		rgb.b += 10;
	}
}
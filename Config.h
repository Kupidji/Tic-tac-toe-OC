#pragma once

#include "Rgb.h";

//struct Config {
//	int n;
//	int	width;
//	int	height;
//	Rgb	color_line;
//	Rgb	color_field;
//};

struct Config {
	int n = 3;
	int	width = 320;
	int	height = 240;
	Rgb	color_line = { 255,	0,	0 };
	Rgb	color_field = { 46,	  61,  209 };
};
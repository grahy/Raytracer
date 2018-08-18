#pragma once
#include<Windows.h>
#include"color.h"
#include<fstream>
#include"texture.h"

void generate_color_PPM(const char *filename, const int &w, const int &h, color **mat);


bool getBmpData(const char *filepath, color **texc);

texture2d readBmp(const char *filepath);
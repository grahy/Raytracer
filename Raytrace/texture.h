#pragma once
#include"color.h"
class texture2d {
public:
	texture2d(color **texc, int w, int h);
	texture2d(const texture2d &t);
	~texture2d();
	color **_texc;                    
	int _width, _height;
};

class textcoord {
public:
	textcoord(int u, int v);
	textcoord(const textcoord &tc);
	textcoord operator*(float n)const;
	textcoord operator+(const textcoord &tc);
	bool operator!=(const textcoord &tc);
	int _u, _v;
};

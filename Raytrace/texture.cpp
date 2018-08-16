#include "texture.h"
#include<fstream>

texture2d::texture2d(color **texc, int w, int h) {
	_width = w;
	_height = h;
	if (texc) {
		_texc = texc;
	}else{
		_texc = NULL;
	}
}
texture2d::texture2d(const texture2d &t) {
	_width = t._width;
	_height = t._height;
	_texc = new color*[_height];
	for (int i = 0; i < _height; i++) {
		_texc[i] = new color[_width];
	}
	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
			_texc[i][j] = t._texc[i][j];
		}
	}
}

texture2d::~texture2d() {
	if (_texc) {
		for (int i = 0; i < _height; i++) {
			delete[]_texc[i];
		}
		delete[]_texc;
	}
	_texc = NULL;
}

textcoord::textcoord(int u, int v)
{
	_u = u;
	_v = v;
}

textcoord::textcoord(const textcoord &tc)
	:_u(tc._u), _v(tc._v)
{

}

textcoord textcoord::operator*(float n)const {
	return textcoord(_u*n, _v*n);
}

textcoord textcoord::operator+(const textcoord &tc) {
	return textcoord(_u + tc._u, _v + tc._v);
}

bool textcoord::operator!=(const textcoord &tc) {
	if (_u != tc._u)return false;
	if (_v != tc._v)return false;
	return true;
}
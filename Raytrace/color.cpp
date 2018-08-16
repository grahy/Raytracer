#include"color.h"



color::color()
	:_r(0), _g(0), _b(0)
{

}

color::color(float n)
	: _r(n), _g(n), _b(n)
{

}

color::color(float _r, float g, float b)
	: _r(_r), _g(g), _b(b)
{

}

color::color(const color &c)
	: _r(c._r), _g(c._g), _b(c._b)
{

}

color color::operator+(const color &c) {
	return color(_r + c._r, _g + c._g, _b + c._b);
}
color& color::operator=(const color &c) {
	_r = c._r;
	_g = c._g;
	_b = c._b;
	return *this;
}
color color::operator*(float n) const{
	return color(_r*n, _g*n, _b*n);
}
bool color::operator!=(const color &c) {
	if (_r != c._r)return false;
	if (_g != c._g)return false;
	if (_b != c._b)return false;
	return true;
}
std::ostream& operator<<(std::ostream &os, const color &c) {
	os << c._r << " " << c._g << " " << c._b;
	return os;
}
#include"vec3f.h"

vec3f::vec3f() :_x(0), _y(0), _z(0) {}
vec3f::vec3f(float x, float y, float z) : _x(x), _y(y), _z(z) {}
vec3f::vec3f(const vec3f &v) : _x(v._x), _y(v._y), _z(v._z) {}
vec3f::vec3f(float n) : _x(n), _y(n), _z(n) {}

vec3f::~vec3f() {}

vec3f vec3f::operator+(const vec3f &v) const {
	return vec3f(_x + v._x, _y + v._y, _z + v._z);
}

vec3f vec3f::operator-(const vec3f &v) const{
	return vec3f(_x - v._x, _y - v._y, _z - v._z);
}

vec3f operator-(const vec3f &v) {
	return vec3f(-v._x, -v._y, -v._z);
}

vec3f vec3f::operator*(float num) const {
	return vec3f(_x*num, _y*num, _z*num);
}

vec3f vec3f::operator*(const vec3f &v)const {
	return vec3f(_x*v._x,_y*v._y, _z*v._z);
}

vec3f vec3f::operator/(float num) const {
	return vec3f(_x / num, _y / num, _z / num);
}

vec3f& vec3f::operator=(const vec3f &v) {
	_x = v._x;
	_y = v._y;
	_z = v._z;
	return *this;
}

vec3f& vec3f::operator+=(const vec3f &v) {
	_x += v._x;
	_y += v._y;
	_z += v._z;
	return *this;
}

bool vec3f::operator>(float n) {
	if (_x + _y + _z > n) {
		return true;
	}
	return false;
}

bool vec3f::operator!=(const vec3f &v) const {
	if (_x != v._x)return false;
	if (_y != v._y)return false;
	if (_z != v._z)return false;
	return true;
}

float dot(const vec3f &v1, const vec3f &v2) {
	return (v1._x*v2._x + v1._y*v2._y + v1._z*v2._z);
}

vec3f cross(const vec3f &v1, const vec3f &v2) {
	return vec3f(v1._y*v2._z - v2._y*v1._z, -(v1._x*v2._z - v2._x*v1._z), v1._x*v2._y - v2._x*v1._y);
}

std::ostream& operator<<(std::ostream &os, const vec3f &v) {
	os << "[" << v._x << "," << v._y << "," << v._z << "]";
	return os;
}

vec3f vec3f::normalize() const {
	float t = sqrt(1.0f / (_x*_x + _y * _y + _z * _z));
	return vec3f(t*_x, t*_y, t*_z);
}

float vec3f::squared_length() {
	return (_x*_x + _y * _y + _z * _z);
}

float vec3f::length() const {
	return sqrt(_x*_x + _y * _y + _z * _z);
}


//vertex

vertex::vertex(const vec3f &pos, const textcoord &tc)
	:_pos(pos), _tc(tc)
{
}

vertex::vertex(const vec3f &pos)
	: _pos(pos), _tc(textcoord(0, 0))
{

}

vertex::vertex(const vertex &vertex)
	: _pos(vertex._pos), _tc(vertex._tc)
{
	
}

bool vertex::operator!=(const vertex &v) {
	if (_pos != v._pos)return false;
	if (_tc != v._tc)return false;
	return true;
}

vertex::~vertex() {

}
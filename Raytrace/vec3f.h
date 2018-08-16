#pragma once
#include<math.h>
#include<ostream>
#include"color.h"
#include"texture.h"
class vec3f {
public:
	float _x, _y, _z;

	vec3f();
	vec3f(float x, float y, float z);
	vec3f(const vec3f &v);
	vec3f(float n);

	~vec3f();

	vec3f operator+(const vec3f &v)const;

	vec3f operator-(const vec3f &v)const;

	friend vec3f operator-(const vec3f &v);

	vec3f operator*(float num)const;

	vec3f operator*(const vec3f &v)const;

	vec3f operator/(float num)const;

	vec3f& operator=(const vec3f &v);

	vec3f& operator+=(const vec3f &v);

	bool operator>(float n);

	bool operator!=(const vec3f &v)const;

	friend float dot(const vec3f &v1, const vec3f &v2);

	friend vec3f cross(const vec3f &v1, const vec3f &v2);

	friend std::ostream& operator<<(std::ostream &os, const vec3f &v);

	vec3f normalize()const;

	float squared_length();

	float length() const;
};

class vertex
{
public:
	vertex(const vec3f &pos, const textcoord &tc);
	vertex(const vec3f &pos);
	vertex(const vertex &vertex);
	~vertex();

	bool operator!=(const vertex &v);

	vec3f _pos;
	textcoord _tc;
	//edge e;
};
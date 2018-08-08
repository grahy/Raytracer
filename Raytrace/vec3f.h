#pragma once
#include<math.h>
#include<ostream>
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

	bool operator!=(const vec3f &v)const;

	friend float dot(const vec3f &v1, const vec3f &v2);

	friend vec3f cross(const vec3f &v1, const vec3f &v2);

	friend std::ostream& operator<<(std::ostream &os, const vec3f &v);

	vec3f normalize()const;

	float squared_length();

	float length() const;
};
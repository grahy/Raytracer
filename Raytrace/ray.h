#pragma once
#include"vec3f.h"
class Ray {
public:
	Ray(const vec3f &rayori, const vec3f &raydir) :ori_(rayori), dir_(raydir) {}
	~Ray() {}
	vec3f ori_, dir_;
};
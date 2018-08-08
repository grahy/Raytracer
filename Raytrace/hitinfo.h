#pragma once
#include"vec3f.h"

class Hitinfo {
public:
	Hitinfo(const float &d = 1e8, const vec3f &p = 0, const vec3f &n = 0)
		:distance_(d), phit_(p), normal_(n) {}
	~Hitinfo() {}
	float distance_;
	vec3f phit_;
	vec3f normal_;
};

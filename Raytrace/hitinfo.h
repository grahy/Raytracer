#pragma once
#include"vec3f.h"

class Hitinfo {
public:
	Hitinfo(const float &d = 1e8, const vec3f &p = 0, const vec3f &n = 0,const color &texcol=0)
		:_distance(d), _phit(p), _normal(n), _texcol(texcol){}
	~Hitinfo() {}
	float _distance;
	vec3f _phit;
	vec3f _normal;
	color _texcol;
};

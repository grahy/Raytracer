#pragma once
#include"vec3f.h"
#include"texture.h"

class material
{
public:
	material(const vec3f &sc,
		const float &refl = 0,
		const float &transp = 0,
		const vec3f &ec = 0,
		const float &nt = 1.0f,
		const texture2d *texture=NULL);
	material(const material &mat);
	~material();

	bool operator!=(const material &mat)const;

	float _transparency, _reflection;
	vec3f _emissionColor, _surfaceColor;
	float _nt;
	const texture2d* _texture;
};


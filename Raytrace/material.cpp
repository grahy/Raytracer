#include "material.h"



material::material(const vec3f &sc,
	const float &refl,
	const float &transp,
	const vec3f &ec,
	const float &nt,
	const texture2d *texture) 
	:_surfaceColor(sc), _emissionColor(ec),
	_transparency(transp), _reflection(refl), _nt(nt)
{
	_texture = texture;
}
material::material(const material &mat) 
	: _surfaceColor(mat._surfaceColor), _emissionColor(mat._emissionColor),
	_transparency(mat._transparency), _reflection(mat._reflection), _nt(mat._nt)
{
	_texture = mat._texture;
}

material::~material()
{
	
}

bool material::operator!=(const material &mat)const {
	if ((_surfaceColor != mat._surfaceColor)  ||
		(_transparency != mat._transparency)  ||
		(_reflection != mat._reflection)      ||
		(_emissionColor != mat._emissionColor ||
		(_nt != mat._nt))					  ||						
		(_texture!=mat._texture)				) {
		return false;
	}
	return true;
}
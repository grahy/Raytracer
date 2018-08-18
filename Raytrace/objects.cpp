#include "objects.h"
#include<iostream>

objects* getObjData(const objects *obj) {
	objects* temp = NULL;
	switch ((*obj).getObjectType())
	{
	case Objtype::SPHERE:
		temp = (Sphere*)obj;
	case Objtype::TRIANGLE:
		temp = (Triangle*)obj;
	default:
		break;
	}
	return temp;
}

objects::objects(const material &mat) 
	:_mat(mat)
{

}

bool objects::operator!=(const objects *obj)const {
	if (this->getObjectType() != obj->getObjectType()) {
		return false;
	}
	const Triangle* temp_t1 = NULL;
	const Triangle* temp_t2 = NULL;
	const Sphere* temp_s1 = NULL;
	const Sphere* temp_s2 = NULL;
	switch (getObjectType())
	{
	case Objtype::SPHERE:
		temp_s1 = (Sphere*)getObjData(this);
		temp_s2 = (Sphere*)getObjData(obj);
		if ((*temp_s1) != (*temp_s2)) {
			return false;
		}
		return true;
	case Objtype::TRIANGLE:
		temp_t1 = (Triangle*)getObjData(this);
		temp_t2 = (Triangle*)getObjData(obj);
		if ((*temp_t1) != (*temp_t2)) {
			return false;
		}
		return true;
	default:
		break;
	}
}

vec3f objects::surfaceColor()const {
	return _mat._surfaceColor;
}
vec3f objects::emissionColor()const {
	return _mat._emissionColor;
}
float objects::transparency()const {
	return _mat._transparency;
}
float objects::reflection()const {
	return _mat._reflection;
}
float objects::nt()const {
	return _mat._nt;
}

Sphere::Sphere(
	const vec3f &c,
	const float &_r,
	const vec3f &sc,
	const float &refl,
	const float &transp,
	const vec3f &ec,
	const float &nt) :
	center_(c), radius_(_r), radius2_(_r * _r), objects(material(sc, refl, transp, ec, nt))
{

}

Sphere::~Sphere() {

}

bool Sphere::intersect(const Ray &ray,Hitinfo &hitinfo, float tmin, float tmax) const{
	vec3f v = ray.ori_ - center_;
	float a = dot(ray.dir_, ray.dir_);
	float b = 2 * dot(v, ray.dir_);
	float c = dot(v, v) - radius2_;
	float discriminant = b * b - 4 * a*c;
	if (discriminant>0) {
		float temp = (-b - sqrtf(discriminant)) / a * 0.5f;
		if (temp < tmax && temp > tmin)
		{
			hitinfo._distance = temp;
			hitinfo._phit = ray.ori_ + ray.dir_*temp;
			hitinfo._normal = (hitinfo._phit - center_).normalize();
			return true;
		}
		//否则就计算远离射线源的点
		temp = (-b + sqrtf(discriminant)) / a * 0.5f;
		if (temp < tmax && temp > tmin)
		{
			hitinfo._distance = temp;
			hitinfo._phit = ray.ori_ + ray.dir_*temp;
			hitinfo._normal = (hitinfo._phit - center_).normalize();
			return true;
		}
	}
	return false;
}

int Sphere::getObjectType()const {
	return Objtype::SPHERE;
}

bool Sphere::operator!=(const Sphere &sp) const {
	if ((center_ != sp.center_) ||
		(radius_ != sp.radius_) ||
		_mat!=sp._mat)			{
		return false;
	}
	return true;
}

Triangle::Triangle(const vertex &v1,
	const vertex &v2,
	const vertex &v3,
	const vec3f &sc,
	float refl,
	float transp,
	float nt,
	const vec3f &ec,
	const texture2d *texture)
	:_a(v1), _b(v2), _c(v3), surfaceNormal_(cross(_b._pos - _a._pos, _c._pos - _a._pos).normalize()),objects(material(sc,refl,transp,ec,nt,texture))
{
	
}
Triangle::~Triangle() {

}

color Get_texture_color(const vertex &v0, const vertex &v1, const vertex &v2, const Hitinfo &hitinfo, const texture2d *texture) {
	vec3f na = cross(v2._pos - v1._pos, hitinfo._phit - v1._pos);
	vec3f nb = cross(v0._pos - v2._pos, hitinfo._phit - v2._pos);
	vec3f nc = cross(v1._pos - v0._pos, hitinfo._phit - v0._pos);
	vec3f n = cross((v1._pos - v0._pos), v2._pos - v0._pos);
	float n2 = dot(n, n);
	float a = dot(n, na) / n2;
	float b = dot(n, nb) / n2;
	float c = dot(n, nc) / n2;
	textcoord ta = v0._tc*a + v1._tc*b + v2._tc*c;
	
	if (ta._u >= texture->_width ||
		ta._v >= texture->_height||
		ta._u<0||
		ta._v<0) {
		std::cout << ta._u << " " << ta._v << std::endl;
		ta._u = 0;
		ta._v = 0;
		std::cout << "wrong";
	}
	return texture->_texc[texture->_height - ta._v - 1][ta._u];
}

bool Triangle::intersect(const Ray &ray, Hitinfo &hitinfo, float tmin, float tmax) const {
	float a = _a._pos._x - _b._pos._x;
	float b = _a._pos._y - _b._pos._y;
	float c = _a._pos._z - _b._pos._z;
	//
	float d = _a._pos._x - _c._pos._x;
	float e = _a._pos._y - _c._pos._y;
	float f = _a._pos._z - _c._pos._z;
	//
	float g = ray.dir_._x;
	float h = ray.dir_._y;
	float i = ray.dir_._z;
	//
	float j = _a._pos._x - ray.ori_._x;
	float k = _a._pos._y - ray.ori_._y;
	float _l = _a._pos._z - ray.ori_._z;
	//
	float emf = e * i - h * f;
	float amb = a * k - j * b;
	float jml = j * c - a * _l;
	float bmc = b * _l - k * c;
	float dmg = d * h - e * g;
	float gmi = g * f - d * i;
	//
	float M = a * emf + b * gmi + c * dmg;

	if (M == 0) {
		return false;
	}
	float t = -(f*amb + e * jml + d * bmc) / M;
	if ( (t<tmin) || (t>tmax)) {
		return false;
	}
	float f2 = (i*amb + h * jml + g * bmc) / M;
	if ((f2 < 0) || (f2>1)) {
		return false;
	}
	float f1 = (j * emf + k * gmi + _l * dmg) / M;
	if ((f1 < 0) || (f1>1 - f2)) {
		return false;
	}
	hitinfo._normal = surfaceNormal_;
	hitinfo._phit = ray.ori_ + ray.dir_*t;
	hitinfo._distance = t;
	if (_mat._texture) {
		hitinfo._texcol = Get_texture_color(_a, _b, _c, hitinfo, _mat._texture);
	}
	return true;
}

int Triangle::getObjectType()const {
	return Objtype::TRIANGLE;
}

bool Triangle::operator!=(const Triangle &t)const {
	if ((_a._pos != t._a._pos)	||
		(_b._pos != t._b._pos)	||
		(_c._pos != t._c._pos)	||
		_mat!=t._mat)	{
		return false;
	}
	return true;
}
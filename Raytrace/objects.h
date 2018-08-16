#pragma once
#include"ray.h"
#include"hitinfo.h"
#include"material.h"
#include<vector>

enum Objtype {
	SPHERE,
	TRIANGLE
};



class objects
{
public:
	virtual bool intersect(const Ray &ray, Hitinfo &hitinfo,float tmin,float tmax)const = 0;
	virtual vec3f surfaceColor()const;
	virtual vec3f emissionColor()const;
	virtual float transparency()const;
	virtual float reflection()const;
	virtual float nt()const;
	bool operator!=(const objects *obj)const;
	virtual int getObjectType()const = 0;
protected:
	objects(const material &mat);
	material _mat;
	
};

class Sphere :public objects{
public:
	Sphere(
		const vec3f &c,
		const float &_r,
		const vec3f &sc,
		const float &refl = 0,
		const float &transp = 0,
		const vec3f &ec = 0,
		const float &nt = 1.0f);
	~Sphere();

	bool intersect(const Ray &ray, Hitinfo &hitinfo, float tmin, float tmax) const override;

	bool operator!=(const Sphere &_s)const;
	int getObjectType()const override;

	vec3f center_;
	float radius_, radius2_;
};

class Triangle :public objects {
public:
	Triangle(const vertex &v1,
		const vertex &v2,
		const vertex &v3,
		const vec3f &surfacecolor,
		float reflection = 0,
		float transparency = 0,
		float nt = 1.0f,
		const vec3f &emissioncolor = vec3f(0),
		const texture2d *texture = NULL);

	~Triangle();
	bool intersect(const Ray &ray, Hitinfo &hitinfo, float tmin, float tmax) const override;
	bool operator!=(const Triangle &t)const;
	int getObjectType()const override;

	vertex _a, _b, _c;                              
	vec3f surfaceNormal_;
};

objects* getObjData(const objects *obj);
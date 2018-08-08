#pragma once
#include"math.h"
#include"vec3f.h"
#include"ray.h"
#include"hitinfo.h"
#include<iostream>

class Sphere {
public:
	Sphere(
		const vec3f &c,
		const float &r,
		const vec3f &sc,
		const float &refl = 0,
		const float &transp = 0,
		const vec3f &ec = 0) :
		center_(c), radius_(r), radius2_(r * r), surfaceColor_(sc), emissionColor_(ec),
		transparency_(transp), reflection_(refl)
	{}
	~Sphere() {}

	//    bool intersect(const Ray &ray,Hitinfo &hitinfo)const{
	//        Vec3f rc=center_-ray.ori_;
	//        float p=rc.dot(ray.dir_);
	//        float dis2=rc.dot(rc)-p*p;
	//        float tds;
	//        if(dis2>radius2_ || (p<0))return false;
	//        if(rc.dot(rc)<radius2_){
	//            tds = sqrt(radius2_ - dis2) + p;
	//            //std::cout<<"inside "<<dis2<<" "<<p<<" "<<radius2_<<" "<< rc.dot(rc)<<" "<<tds<<'\n';
	//        }else{
	//            tds = p - sqrt(radius2_ - dis2);
	//            //std::cout<<"outside "<<dis2<<" "<<p<<" "<<radius2_<<" "<< rc.dot(rc)<<" "<<tds<<'\n';
	//        }
	//        hitinfo.distance_=tds;
	//        hitinfo.phit_=ray.ori_+ray.dir_*tds;
	//        hitinfo.normal_=(hitinfo.phit_-center_).normalize();
	//        return true;
	//    }

	bool intersect(const Ray &ray, Hitinfo &hitinfo) const {
		vec3f v = ray.ori_ - center_;
		float a = dot(ray.dir_, ray.dir_);
		float b = 2 * dot(v, ray.dir_);
		float c = dot(v, v) - radius2_;
		float discriminant = b * b - 4 * a*c;
		float t_max = 1e8, t_min = 0;
		if (discriminant>0) {
			float temp = (-b - sqrtf(discriminant)) / a * 0.5f;
			if (temp < t_max && temp > t_min)
			{
				hitinfo.distance_ = temp;
				hitinfo.phit_ = ray.ori_ + ray.dir_*temp;
				hitinfo.normal_ = (hitinfo.phit_ - center_).normalize();
				return true;
			}
			//否则就计算远离射线源的点
			temp = (-b + sqrtf(discriminant)) / a * 0.5f;
			if (temp < t_max && temp > t_min)
			{
				hitinfo.distance_ = temp;
				hitinfo.phit_ = ray.ori_ + ray.dir_*temp;
				hitinfo.normal_ = (hitinfo.phit_ - center_).normalize();
				return true;
			}
		}
		return false;
	}

	bool operator!=(const Sphere &sp) {
		if ((center_ != sp.center_) ||
			(radius_ != sp.radius_) ||
			(surfaceColor_ != sp.surfaceColor_) ||
			(transparency_ != sp.transparency_) ||
			(reflection_ != sp.reflection_) ||
			(emissionColor_ != sp.emissionColor_)) {
			return false;
		}
		return true;
	}

	vec3f center_;
	float radius_, radius2_;
	float transparency_, reflection_;
	vec3f emissionColor_, surfaceColor_;
};
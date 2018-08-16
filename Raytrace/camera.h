#pragma once
#include"vec3f.h"
#include"ray.h"
#include"random_arr.h"
#include"color.h"
#include"hitinfo.h"
#include<vector>
#include"objects.h"

enum Camera_model {
	PERPSECTIVE,
	ORTHORGRAPHICS
};

class camera
{
public:
	camera(const vec3f &eye, const vec3f &lookat, const vec3f &vup, float zn, float zf, float vtheta);						//perspective
	camera(const vec3f &eye, const vec3f &lookat, const vec3f &vup, float zf);                                              //orthorgraphic
	~camera();
	void useSample(float sample_num);
	void useDOF(float lens, float focal_distance);
	void process_img(std::vector<objects*> &obj, int MAX_DEPTH, color** img, int w, int h);

private:
	void init(float zf, float zn = 0, float sample_num = 1, float lens = 0, float focal_distance = 1);
	void render_perspective(std::vector<objects*> &obj, int MAX_DEPTH, color** img, int w, int h)const;
	void render_orthorgraphic(std::vector<objects*> &obj, int MAX_DEPTH, color** img, int w, int h)const;
	vec3f _e, _u, _v, _w;
	float _zn, _zf, _ration, _vtheta, _window_w, _window_h;
	Camera_model _model;
	int _sample_num;
	float _lens,_focal_distance;
};


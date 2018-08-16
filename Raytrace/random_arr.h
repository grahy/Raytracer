#pragma once

struct point {
	float x, y;
};

class random_arr
{
public:
	random_arr(int sample_num = 1, float lens = 0);
	random_arr(const random_arr &_r);
	~random_arr();
	void shuffle_s();
	void shuffle_l();
	point *_r, *_s, *_l;                   //_r为像素采样所用 _s为阴影射线采样所用 _l为lens采样.
	int n2;
	float lens_width;
};


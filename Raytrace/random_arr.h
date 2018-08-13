#pragma once

struct point {
	float x, y;
};

class random_arr
{
public:
	random_arr(int sample_num);
	~random_arr();
	void shuffle_s();
	void shuffle_l();
	point *r, *s, *l;                   //r为像素采样所用 s为阴影射线采样所用 l为lens采样.
	int n2;
};


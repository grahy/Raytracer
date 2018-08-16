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
	point *_r, *_s, *_l;                   //_rΪ���ز������� _sΪ��Ӱ���߲������� _lΪlens����.
	int n2;
	float lens_width;
};


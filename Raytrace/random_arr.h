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
	point *r, *s, *l;                   //rΪ���ز������� sΪ��Ӱ���߲������� lΪlens����.
	int n2;
};


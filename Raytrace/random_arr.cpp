#include "random_arr.h"
#include<time.h>
#include<math.h>
#include<random>
#include<iostream>
#include"vec3f.h"


random_arr::random_arr(int sample_num,float lens)
	:n2(sample_num*sample_num),lens_width(lens)
{
	if (sample_num < 1) {
		std::cout << "sample_num is a invalid value and the sample_num init with number(1)" << std::endl;
		sample_num = 1;
	}
	float n2 = pow(sample_num, 2);
	srand(unsigned(time(NULL)));
	_r = new point[n2];
	_s = new point[n2];
	_l = new point[n2];
	if (sample_num == 1) {
		_r[0].x = _s[0].x = 0;
		_r[0].y = _s[0].y = 0;
	}
	else {
		float half_sample = float(sample_num) / 2.0f;
		
		for (int i = 0; i < n2; i++) {
			_r[i].x =  rand() % 99 / double(100) + i % (sample_num - 1) - half_sample;
			_r[i].y =  rand() % 99 / double(100) + i % (sample_num - 1) - half_sample;
			_s[i].x = rand() % 9 / double(10) - 0.5f;
			_s[i].y = rand() % 9 / double(10) - 0.5f;
			std::cout << _s[i].x << " " << _s[i].y << std::endl;
		}
	}
	if (lens == 0) {
		for (int i = 0; i < n2; i++) {
			_l[i].y = 0;
			_l[i].x = 0;
		}
	}
	else {
		float half_lens = float(lens) / 2.0f;
		int range = lens * 100;
		for (int i = 0; i < n2; i++) {
			_l[i].y = rand() % range / 100.0f - half_lens;
			_l[i].x = rand() % range / 100.0f - half_lens;
		}
	}
	
}

random_arr::random_arr(const random_arr &ra) {
	delete[]_r;
	delete[]_l;
	delete[]_s;
	_r = new point[ra.n2];
	_s = new point[ra.n2];
	_l = new point[ra.n2];
	for (int i = 0; i < ra.n2; i++) {
		_r[i] = ra._r[i];
		_s[i] = ra._s[i];
		_l[i] = ra._l[i];
	}
	lens_width = ra.lens_width;
	n2 = ra.n2;
}

random_arr::~random_arr()
{
	delete[]_r;
	delete[]_s;
	delete[]_l;
}


void random_arr::shuffle_s() {                         //重新排列数组_s 达到伪随机效果
	srand(unsigned(time(NULL)));
	for (int i = n2 - 1; i > 0; i--) {
		int j = rand() % i;
		point temp = _s[i];
		_s[i] = _s[j];
		_s[j] = temp;
	}
}

void random_arr::shuffle_l() {                         //重新排列数组_l 达到伪随机效果
	srand(unsigned(time(NULL)));
	for (int i = n2 - 1; i > 0; i--) {
		int j = rand() % i;
		point temp = _l[i];
		_l[i] = _l[j];
		_l[j] = temp;
	}
}
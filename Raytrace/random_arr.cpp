#include "random_arr.h"
#include<time.h>
#include<math.h>
#include<random>


random_arr::random_arr(int sample_num,float lens)
	:n2(sample_num*sample_num),lens_width(lens)
{
	float n2 = pow(sample_num, 2);
	srand(unsigned(time(NULL)));
	r = new point[n2];
	s = new point[n2];
	l = new point[n2];
	int range = lens * 100;
	float half_sample = float(sample_num) / 2.0f;
	float half_lens = float(lens) / 2.0f;
	for (int i = 0; i < n2; i++) {
		r[i].x = s[i].x = rand() % 99 / double(100) + i % (sample_num - 1) - half_sample;
		r[i].y = s[i].y = rand() % 99 / double(100) + i % (sample_num - 1) - half_sample;
	}
	for (int i = 0; i < n2; i++) {
		l[i].y = rand() % range / 100.0f - half_lens;
		l[i].x = rand() % range / 100.0f - half_lens;
	}
}

random_arr::~random_arr()
{
	delete[]r;
	delete[]s;
	delete[]l;
}


void random_arr::shuffle_s() {                         //重新排列数组s 达到伪随机效果
	srand(unsigned(time(NULL)));
	for (int i = n2 - 1; i > 0; i--) {
		int j = rand() % i;
		point temp = s[i];
		s[i] = s[j];
		s[j] = temp;
	}
}

void random_arr::shuffle_l() {                         //重新排列数组l 达到伪随机效果
	srand(unsigned(time(NULL)));
	for (int i = n2 - 1; i > 0; i--) {
		int j = rand() % i;
		point temp = l[i];
		l[i] = l[j];
		l[j] = temp;
	}
}
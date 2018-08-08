#pragma once
#include"vec3f.h"

class matrix44 {
public:
	matrix44();
	~matrix44();

	void set_identity();
	matrix44 set_transformation();
	matrix44 set_inverse();


	void set_translate(float nx, float ny, float nz);
	void set_rotate(float theta, const vec3f &axis);
	void set_rotate_g(float theta, const vec3f &axis);
	void set_normal_rotate(float theta, const vec3f &axis);
	void set_scale(float sx, float sy, float sz);
	void set_shearing_x(float dy, float dz);
	void set_shearing_y(float dx, float dz);
	void set_shearing_z(float dx, float dy);


	void set_camera(const vec3f &eye, const vec3f &lookat, const vec3f &vup);   //first step

	void set_orthorgraphic_offCenter(const vec3f &left_corner, const vec3f &right_corner);   //sceond
	void set_perspective_offCenter(const vec3f &left_bottom_corner, const vec3f &right_top_corner);
	void set_orthorgraphic(float w, float aspect, float n, float f);
	void set_perspective(float aspect, float theta, float n, float f);
	void set_inverse_perspective(float aspect, float theta, float n, float f, float z);

	void set_viewport(float nx, float ny);                                         //thrid

	matrix44 operator*(const matrix44 &m);
	matrix44& operator=(const matrix44 &m);

	vec3f operator*(const vec3f *v) const;
	vec3f operator*(const vec3f &v) const;

	friend std::ostream& operator<<(std::ostream &os, const matrix44 &m) {
		for (int i = 0; i < 4; i++) {
			os << "|" << " ";
			for (int j = 0; j < 4; j++) {
				os << m.mat[i][j] << " ";
			}
			os << "|" << "\n";
		}
		return os;
	}

	float mat[4][4];
};
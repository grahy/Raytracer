#include"matrix44.h"

const float PI = 3.1415926f;

void set_smallest_magnitude_to_1(vec3f &v) {
	float x = v._x;
	float y = v._y;
	float z = v._z;
	if (x < 0)x = -x;
	if (y < 0)y = -y;
	if (z < 0)z = -z;
	float *temp;
	float t = (x > y) ? y : x;
	t = (t > z) ? z : t;
	if (t == x) {
		v._x = 1;
	}
	else if (t == y) {
		v._y = 1;
	}
	else {
		v._z = 1;
	}
}


matrix44::matrix44() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
}

matrix44::~matrix44() {

}

void matrix44::set_identity() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = (i == j) ? 1 : 0;
		}
	}
}

matrix44 matrix44::set_transformation() {
	matrix44 temp = *this;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp.mat[j][i] = mat[i][j];
		}
	}
	return temp;
}

matrix44 matrix44::set_inverse() {
	matrix44 temp;
	temp.set_identity();
	return temp * this->set_transformation();
}

void matrix44::set_rotate(float theta, const vec3f& unit_axis) {
	set_identity();
	float cosT = cos(PI*theta / 180.0f);
	float sinT = sin(PI*theta / 180.0f);
	float u = (unit_axis._x > 0) ? unit_axis._x : -unit_axis._x;
	float v = (unit_axis._y > 0) ? unit_axis._y : -unit_axis._y;
	float w = (unit_axis._z > 0) ? unit_axis._z : -unit_axis._z;
	float u2 = unit_axis._x*unit_axis._x;
	float v2 = unit_axis._y*unit_axis._y;
	float w2 = unit_axis._z*unit_axis._z;

	mat[0][0] = u2 + (1 - u2)*cosT;
	mat[0][1] = u * v*(1 - cosT) - w * sinT;
	mat[0][2] = u * w*(1 - cosT) + v * sinT;
	mat[1][0] = u * v*(1 - cosT) + w * sinT;
	mat[1][1] = v2 + (1 - v2)*cosT;
	mat[1][2] = v * 2 * (1 - cosT) - u * sinT;
	mat[2][0] = u * w*(1 - cosT) - v * sinT;
	mat[2][1] = v * w*(1 - cosT) + u * sinT;
	mat[2][2] = w2 + (1 - w2)*cosT;
}

void matrix44::set_rotate_g(float theta, const vec3f &axis) {
	vec3f w = axis.normalize();
	vec3f t = w;
	set_smallest_magnitude_to_1(t);
	vec3f u = cross(t, w).normalize();
	vec3f v = cross(w, u);

	//Ruvw                                               // transform xyz coordinary to uvw coordinary
	matrix44 Ruvw;
	Ruvw.set_identity();
	Ruvw.mat[0][0] = u._x; Ruvw.mat[0][1] = u._y; Ruvw.mat[0][2] = u._z;
	Ruvw.mat[1][0] = v._x; Ruvw.mat[1][1] = v._y; Ruvw.mat[1][2] = v._z;
	Ruvw.mat[2][0] = w._x; Ruvw.mat[2][1] = w._y; Ruvw.mat[2][2] = w._z;

	//Rz                                                 // rotate by z(w) axis
	float cosT = cos(PI*theta / 180);
	float sinT = sin(PI*theta / 180);
	matrix44 Rz;
	Rz.set_identity();
	Rz.mat[0][0] = cosT; Rz.mat[0][1] = -sinT;
	Rz.mat[1][0] = sinT; Rz.mat[1][1] = cosT;

	//Ruvw(T)                                            // transform uvw coordinary to xyz coordinary
	matrix44 Ruvw_t = Ruvw.set_transformation();

	matrix44 temp = Rz * Ruvw;
	*this = Ruvw_t * temp;
}

void matrix44::set_normal_rotate(float theta, const vec3f &axis) {
	matrix44 rotate;
	rotate.set_rotate_g(theta, axis);
	*this = rotate.set_inverse().set_transformation();
}

void matrix44::set_scale(float sx, float sy, float sz) {
	set_identity();
	mat[0][0] = sx;
	mat[1][1] = sy;
	mat[2][2] = sz;
}

void matrix44::set_translate(float nx, float ny, float nz) {
	set_identity();
	mat[0][3] = nx;
	mat[1][3] = ny;
	mat[2][3] = nz;
}


void matrix44::set_shearing_x(float dy, float dz) {
	set_identity();
	mat[0][1] = dy;
	mat[0][2] = dz;
}
void matrix44::set_shearing_y(float dx, float dz) {
	set_identity();
	mat[0][0] = dx;
	mat[0][2] = dz;
}
void matrix44::set_shearing_z(float dx, float dy) {
	set_identity();
	mat[0][0] = dx;
	mat[0][1] = dy;
}

void matrix44::set_viewport(float nx, float ny) {
	set_identity();
	mat[0][0] = nx / 2.0f;
	mat[0][3] = (nx - 1) / 2.0f;
	mat[1][1] = ny / 2.0f;
	mat[1][3] = (ny - 1) / 2.0f;
}

void matrix44::set_camera(const vec3f &eye, const vec3f &lookat, const vec3f &vup) {
	vec3f w = -lookat.normalize();
	vec3f u = cross(vup, w).normalize();
	vec3f v = cross(w, u);

	matrix44 Translate;
	Translate.set_translate(-eye._x, -eye._y, -eye._z);

	matrix44 Ruvw;
	Ruvw.set_identity();
	Ruvw.mat[0][0] = u._x; Ruvw.mat[0][1] = u._y; Ruvw.mat[0][2] = u._z;
	Ruvw.mat[1][0] = v._x; Ruvw.mat[1][1] = v._y; Ruvw.mat[1][2] = v._z;
	Ruvw.mat[2][0] = w._x; Ruvw.mat[2][1] = w._y; Ruvw.mat[2][2] = w._z;

	*this = Ruvw * Translate;
}

void matrix44::set_orthorgraphic_offCenter(const vec3f &left_corner, const vec3f &right_corner) {
	set_identity();
	float rml = right_corner._x - left_corner._x;
	float ral = right_corner._x + left_corner._x;
	float tmb = right_corner._y - left_corner._y;
	float tab = right_corner._y + left_corner._y;
	float nmf = left_corner._z - right_corner._z;
	float naf = left_corner._z + right_corner._z;
	mat[0][0] = 2.0f / rml;
	mat[1][1] = 2.0f / tmb;
	mat[2][2] = 2.0f / nmf;
	mat[0][3] = -ral / rml;
	mat[1][3] = -tab / tmb;
	mat[2][3] = -naf / nmf;
}

void matrix44::set_perspective_offCenter(const vec3f &left_bottom_corner, const vec3f &right_top_corner) {
	matrix44 othor;
	othor.set_orthorgraphic_offCenter(left_bottom_corner, right_top_corner);
	matrix44 p;
	p.set_identity();
	p.mat[0][0] = left_bottom_corner._z;
	p.mat[1][1] = left_bottom_corner._z;
	p.mat[2][2] = left_bottom_corner._z + right_top_corner._z;
	p.mat[2][3] = -(left_bottom_corner._z*right_top_corner._z);
	p.mat[3][2] = 1;
	p.mat[3][3] = 0;
	*this = othor * p;
}

void matrix44::set_orthorgraphic(float w, float aspect, float n, float f) {
	set_identity();
	float h = w / aspect;
	mat[0][0] = 2.0f / w;
	mat[1][1] = 2.0f / h;
	mat[2][2] = -1.0f / (f - n);
	mat[2][3] = n / (f - n);
}
void matrix44::set_perspective(float aspect, float theta, float n, float f) {
	matrix44 orthor;
	float tanHalfT = tan(PI*theta / 180 / 2);
	float h = -2 * n *tanHalfT;
	float w = h * aspect;
	orthor.set_orthorgraphic(w, aspect, n, f);
	matrix44 p;
	p.set_identity();
	p.mat[0][0] = n;
	p.mat[1][1] = n;
	p.mat[2][2] = n + f;
	p.mat[2][3] = -(n + f);
	p.mat[3][2] = 1;
	p.mat[3][3] = 0;
	*this = orthor * p;
}

void matrix44::set_inverse_perspective(float aspect, float theta, float n, float f, float z) {
	matrix44 orthor;
	float tanHalfT = tan(PI*theta / 180 / 2);
	float h = -2 * n *tanHalfT;
	float w = h * aspect;
	orthor.set_orthorgraphic(w, aspect, n, f);
	matrix44 p;
	p.set_identity();
	p.mat[0][0] = f;
	p.mat[1][1] = f;
	p.mat[2][2] = 0;
	p.mat[2][3] = f * n;
	p.mat[3][2] = -1;
	p.mat[3][3] = n + f;
	*this = p * orthor.set_inverse();
}

matrix44 matrix44::operator*(const matrix44 &m) {
	matrix44 n;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			n.mat[i][j] = mat[i][0] * m.mat[0][j] + mat[i][1] * m.mat[1][j] + mat[i][2] * m.mat[2][j] + mat[i][3] * m.mat[3][j];
		}
	}
	return n;
}

vec3f matrix44::operator*(const vec3f &v) const {
	vec3f nv;
	nv._x = mat[0][0] * v._x + mat[0][1] * v._y + mat[0][2] * v._z + mat[0][3];
	nv._y = mat[1][0] * v._x + mat[1][1] * v._y + mat[1][2] * v._z + mat[1][3];
	nv._z = mat[2][0] * v._x + mat[2][1] * v._y + mat[2][2] * v._z + mat[2][3];
	float w = mat[3][0] * v._x + mat[3][1] * v._y + mat[3][2] * v._z + mat[3][3];
	nv = nv / w;
	return nv;
}

vec3f matrix44::operator*(const vec3f *v) const {
	vec3f nv;
	nv._x = mat[0][0] * v->_x + mat[0][1] * v->_y + mat[0][2] * v->_z + mat[0][3];
	nv._y = mat[1][0] * v->_x + mat[1][1] * v->_y + mat[1][2] * v->_z + mat[1][3];
	nv._z = mat[2][0] * v->_x + mat[2][1] * v->_y + mat[2][2] * v->_z + mat[2][3];
	float w = mat[3][0] * v->_x + mat[3][1] * v->_y + mat[3][2] * v->_z + mat[3][3];
	nv = nv / w;
	return nv;
}

matrix44& matrix44::operator=(const matrix44 &m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = m.mat[i][j];
		}
	}
	return *this;
}
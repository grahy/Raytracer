#include "camera.h"
#include"ioimages.h"

float mix(const float &a, const float &b, const float &mix)
{
	return b * mix + a * (1 - mix);
}

bool refract(const Ray &ray, const vec3f &normal, float nt, vec3f &refrac_dir) {
	float ior = 1.0f / nt;                                   //默认空气中
	float cosi = -dot(normal, ray.dir_);
	float dn = dot(ray.dir_, normal);
	float k = 1 - ior * ior*(1 - cosi * cosi);
	float j = 1 - (ior*(1 - pow(dn, 2)));
	vec3f i = (ray.dir_ - normal *dn)*ior;
	if (j < 0) {
	return false;
	}
	refrac_dir = (i - normal *sqrt(j)).normalize();
	return true;
}

vec3f lambertin(const Hitinfo &hitinfo, const vec3f &light_dir, const vec3f &transmision, const objects *light, const objects *object) {
	float num = max(float(0), dot(hitinfo._normal, light_dir));
	vec3f texcol(hitinfo._texcol._r, hitinfo._texcol._g, hitinfo._texcol._b);
	texcol = texcol / 255.0f;
	return (object->surfaceColor() + texcol) * transmision * num * light->emissionColor();
}

vec3f blinn_phong(const Hitinfo &hitinfo, const Ray &ray,const vec3f &light_dir, const vec3f &transmision, const objects *light, const objects *object, float p) {
	vec3f h = -(ray.dir_ + light_dir).normalize();
	vec3f texcol(hitinfo._texcol._r, hitinfo._texcol._g, hitinfo._texcol._b);
	texcol = texcol / 255.0f;
	return (object->surfaceColor() + texcol)*transmision*(max(0, dot(hitinfo._normal, light_dir)) + pow(max(0, dot(hitinfo._normal, h)), p));
}

vec3f camera::trace(const Ray &ray, std::vector<objects*> &obj, const random_arr &random, int depth, const int &MAX_DEPTH, float tmin, float tmax){
	objects *object = NULL;
	Hitinfo hitinfo, temphit;
	float dis = INFINITY;
	for (unsigned int i = 0; i<obj.size(); i++) {
		if (obj[i]->intersect(ray, temphit, tmin, tmax)) {
			if (dis>temphit._distance) {
				dis = temphit._distance;
				hitinfo = temphit;
				object = obj[i];
			}
		}
	}
	if (!object) {
		return vec3f(1);
	}
	vec3f surfaceColor;
	float bias = 1e-2;
	bool inside = false;
	if (dot(ray.dir_, hitinfo._normal)>0)hitinfo._normal = -hitinfo._normal, inside = true;
	if ((object->transparency()>0 || object->reflection()>0) && (depth<MAX_DEPTH)) {
		float r0 = pow((object->nt() - 1),2) / pow((object->nt() + 1), 2);
		float facingratio = -dot(ray.dir_, hitinfo._normal);
		float fresneleffect = r0 + (1 - r0)*pow(1 - facingratio, 3);
		vec3f reflec_dir = (ray.dir_ - hitinfo._normal * 2 * dot(ray.dir_, hitinfo._normal)).normalize();

		vec3f reflection = trace(Ray(hitinfo._phit + hitinfo._normal*bias, reflec_dir), obj,random, depth + 1, MAX_DEPTH);
		vec3f refraction = 0;
		if (object->transparency()>0) {
			float eta = (inside) ? object->nt() : (1.0f / object->nt());
			float cosi = -dot(hitinfo._normal, ray.dir_);
			float k = 1 - eta * eta*(1 - cosi * cosi);
			vec3f refrac_dir = (ray.dir_*eta + hitinfo._normal*(eta*cosi - sqrt(k))).normalize();
			refraction = trace(Ray(hitinfo._phit + hitinfo._normal * bias, refrac_dir), obj, random, depth + 1, MAX_DEPTH);
		}
		vec3f texcol(hitinfo._texcol._r, hitinfo._texcol._g, hitinfo._texcol._b);
		texcol = texcol / 255.0f;
		surfaceColor = (
			reflection * fresneleffect +
			refraction * (1 - fresneleffect) * object->transparency()) * (object->surfaceColor() + texcol) ;
	}
	else {
		for (unsigned int i = 0; i<obj.size(); i++) {
			vec3f tempColor;
			if (obj[i]->emissionColor()>0) {
				vec3f transmision(1);
				vec3f lightDirection;
				vec3f light_center;
				if (obj[i]->getObjectType() == Objtype::SPHERE) {
					Sphere *_s = (Sphere*)obj[i];
					light_center = _s->center_;
					lightDirection = (light_center - hitinfo._phit).normalize();
				}else {
					continue;
				}
				if (_useSR) {
					tempColor = 0;
					for (int N = 0; N < random.n2; N++) {
						vec3f vup(0, 1, 0);
						vec3f w = lightDirection;
						vec3f u = cross(vup, w).normalize();
						vec3f v = cross(u, w);
						lightDirection = (light_center + u * random._s[N].x + v * random._s[N].y - hitinfo._phit).normalize();
						for (unsigned int j = 0; j<obj.size(); j++) {
							if (i != j && (obj[i] != object)) {
								Hitinfo hit2;
								if (obj[j]->intersect(Ray(hitinfo._phit + hitinfo._normal*bias, lightDirection), hit2, tmin, tmax)) {
									transmision = vec3f(0);
									break;
								}
							}
						}
						tempColor = tempColor + lambertin(hitinfo, lightDirection, transmision, obj[i], object);
						//tempColor = tempColor + blinn_phong(hitinfo, ray, lightDirection, transmision, obj[i], object, 100);
					}
					tempColor = tempColor / float(random.n2);
					surfaceColor += tempColor;
				}
				else {
					for (unsigned int j = 0; j<obj.size(); j++) {
						if (i != j && (obj[i] != object)) {
							Hitinfo hit2;
							if (obj[j]->intersect(Ray(hitinfo._phit + hitinfo._normal*bias, lightDirection), hit2, tmin, tmax)) {
								transmision = vec3f(0);
								break;
							}
						}
					}
					surfaceColor += lambertin(hitinfo, lightDirection, transmision, obj[i], object);
				}
			}
		}
	}
	return surfaceColor + object->emissionColor();
}

camera::camera(const vec3f &eye, const vec3f &lookat, const vec3f &vup, float zn, float zf, float vtheta) 
	:_e(eye),_w((eye - lookat).normalize()),_u(cross(vup,_w).normalize()),_v(cross(_w,_u)),_vtheta(vtheta)
{
	init(zf, zn);
	_model = Camera_model::PERPSECTIVE;
}
camera::camera(const vec3f &eye, const vec3f &lookat, const vec3f &vup, float zf) 
	: _e(eye), _w((eye - lookat).normalize()), _u(cross(vup, _w).normalize()), _v(cross(_w, _u)), _zn(0), _zf(zf)
{
	init(zf);
	_model = Camera_model::ORTHOGRAPHIC;
}
camera::~camera() {

}

void camera::init(float zf, float zn, float sample_num, float lens, float focal_distance) {
	_zf = zf;
	_zn = zn;
	_sample_num = sample_num;
	_lens = lens;
	_focal_distance = focal_distance;
	_useSR = false;
}

void camera::useAntialising(float sample_num) {
	_sample_num = sample_num;
}
void camera::useSoftShadow() {
	_useSR = true;
}

void camera::useDOF(float lens, float focal_distance) {
	if (lens <= 0.0099) {
		std::cout << "lens is too small,reset it to zero" << std::endl;
		_lens = 0;
	}
	else {
		_lens = lens;
		_focal_distance = focal_distance;
	}
}

void camera::render_perspective(std::vector<objects*> &obj, int MAX_DEPTH,color** img, int w, int h) {
	float tanh = tanf(_vtheta / 360 * 3.14f);
	float ratio = float(w) / float(h);
	float height = tanh * 2;
	float width = height * ratio;
	float invheight = 1.0f / h;
	float invwidth = 1.0f / w;
	float halfwidth = width / 2.0f;
	float halfheight = height / 2.0f;
	random_arr random(_sample_num, _lens);
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			vec3f reslut;
			random.shuffle_s();                                 //重置随机数的浮动值
			random.shuffle_l();
			for (int N = 0; N < random.n2; N++) {
				if (_lens <= 0) {                                                                      //是否有使用DOF
					float dx = (x + random._r[N].x)*invwidth*width - halfwidth;
					float dy = (y + random._r[N].y)*invheight*height - halfheight;
					vec3f rayori(_e);
					vec3f raydir = ( _u * dx + _v * dy - _w * _zn).normalize();
					reslut += trace(Ray(rayori, raydir), obj, random, 0, MAX_DEPTH, 0, _zf);
				}
				else {
					vec3f img_center(_e + _u * ((x + 0.5f)*invwidth*width - halfwidth) + _v * ((y + 0.5f)*invheight*height - halfheight) - _w * _zn);                                 //像素中心点
					Ray _r(_e, (img_center - _e).normalize());
					vec3f P = _e + _r.dir_ * (_focal_distance);                                                     //P为焦点
					vec3f rayori(_e + _u * random._l[N].x + _v * random._l[N].y);                                       //lens 的坐标
					vec3f raydir = (P - rayori).normalize();
					reslut += trace(Ray(rayori, raydir), obj, random, 0, MAX_DEPTH, 0, _zf);
				}
			}
			reslut = reslut / float(random.n2);
			reslut = reslut * 255;
			img[h - y - 1][x] = color(reslut._x, reslut._y, reslut._z);
		}
		std::cout << "%" << float(y) / float(h) * 100 << std::endl;
	}
}
void camera::render_orthographic(std::vector<objects*> &obj, int MAX_DEPTH, color** img, int w, int h) {
	//float invHeight = 1.0f / float(h);
	//float mx = float(w) / 2.0f;
	//float my = float(h) / 2.0f;
	//for (int y = 0; y < h; y++) {
	//	for (int x = 0; x < w; x++) {
	//		vec3f reslut;
	//		for (int p = 0; p < _sample_num; p++) {
	//			for (int q = 0; q < _sample_num; q++) {
	//				srand(unsigned(time(NULL)));
	//				float randomx = (rand() % 99 / double(100) + p) / float(sample_num);
	//				float randomy = (rand() % 99 / double(100) + q) / float(sample_num);
	//				float dx = invHeight * (x - mx + randomx);                                                    //将height从[0,h]放缩到[0,1] 同理width将放缩到[0,ratio]
	//				float dy = invHeight * (y - my + randomy);
	//				reslut = reslut + trace(Ray(vec3f(dx, dy, 0), vec3f(dx, dy, -1).normalize()), spheres, 0, MAX_DEPTH);
	//			}
	//		}
	//		reslut = reslut / pow(sample_num, 2);
	//		reslut = reslut * 255;
	//		image[h - y - 1][x] = color(reslut._x, reslut._y, reslut._z);
	//	}

	//}
}

void camera::process_img(std::vector<objects*> &obj, int MAX_DEPTH, int w, int h,const char *filepath) {
	color **img = new color*[h];
	for (int i = 0; i < h; i++) {
		img[i] = new color[w];
	}
	if (_model == Camera_model::PERPSECTIVE) {
		render_perspective(obj, MAX_DEPTH, img, w, h);
		generate_color_PPM(filepath, w, h, img);
	}
	else {
		render_orthographic(obj, MAX_DEPTH, img, w, h);
		generate_color_PPM(filepath, w, h, img);
	}
}
#include"vec3f.h"
#include"sphere.h"
#include"hitinfo.h"
#include<vector>
#include<time.h>
#include"ioimages.h"

int count = 0;

const float INFINITY_ = 1e8;

float mix(const float &a, const float &b, const float &mix)
{
	return b * mix + a * (1 - mix);
}

float max(float a, float b) {
	return a > b ? a : b;
}

bool refract(const Ray &ray, Hitinfo &hitinfo, float nt,bool inside,vec3f &refrac_dir) {
	float ior = 1.0f / nt;                                   //默认空气中
	if (inside) {
		ior = 1 / ior;
	}
	float cosi = -dot(hitinfo.normal_, ray.dir_);
	float dn = dot(ray.dir_, hitinfo.normal_);
	float k = 1 - ior * ior*(1 - cosi * cosi);
	float j = 1 - (ior*(1 - pow(dn, 2)));
	vec3f i = (ray.dir_ - hitinfo.normal_ *dn)*ior;
	if (j < 0) {
		//refrac_dir = i.normalize();
		return false;
	}
	refrac_dir = (i - hitinfo.normal_ *sqrt(j)).normalize();
	return true;
}

vec3f lambertin(const Hitinfo &hitinfo, const vec3f &light_dir, const vec3f &transmision, const Sphere *light, const Sphere *sphere) {
	float num = max(float(0), dot(hitinfo.normal_, light_dir));
	return sphere->surfaceColor_ * transmision * num * light->emissionColor_;
}

vec3f blinn_phong(const Hitinfo &hitinfo,const Ray &ray ,const vec3f &light_dir, const vec3f &transmision, const Sphere *light,const Sphere *sphere,float shiness) {
	vec3f h = (light_dir + ray.dir_).normalize();
	float num1 = max(float(0), dot(hitinfo.normal_, light_dir));
	float num2 = pow(max(float(0), dot(hitinfo.normal_, h)), shiness);
	return sphere->surfaceColor_ * transmision * num1 * light->emissionColor_ + light->emissionColor_*transmision*num2*sphere->reflection_;
}

vec3f ambient(const Hitinfo &hitinfo, const Ray &ray, const vec3f &light_dir, const vec3f &transmision, const Sphere *light, const Sphere *sphere, float shiness) {
	vec3f h = (light_dir + ray.dir_).normalize();
	float num1 = max(float(0), dot(hitinfo.normal_, light_dir));
	float num2 = pow(max(float(0), dot(hitinfo.normal_, h)), shiness);
	return sphere->surfaceColor_ * transmision * num1 * light->emissionColor_ + light->emissionColor_*transmision*num2*sphere->reflection_;
}


vec3f trace(const Ray &ray, std::vector<Sphere> &spheres, int depth,const int &MAX_DEPTH) {
	Sphere *sphere = NULL; Hitinfo hitinfo, temphit;
	float dis = INFINITY_;
	for (unsigned int i = 0; i<spheres.size(); i++) {
		if (spheres[i].intersect(ray, temphit)) {
			if (dis>temphit.distance_) {
				dis = temphit.distance_;
				hitinfo = temphit;
				sphere = &spheres[i];
			}
		}
	}
	if (!sphere) {
		return vec3f(0.6f);
	}
	vec3f surfaceColor = 0;
	float bias = 1e-2;
	bool inside = false;
	if (dot(ray.dir_, hitinfo.normal_) > 0)inside = true, hitinfo.normal_ = -hitinfo.normal_;
	if ((sphere->transparency_>0 || sphere->reflection_>0) && (depth<MAX_DEPTH)) {
		vec3f reflec_dir = (ray.dir_ - hitinfo.normal_ * 2 * dot(ray.dir_, hitinfo.normal_)).normalize();
		vec3f reflection = trace(Ray(hitinfo.phit_ + hitinfo.normal_*bias, reflec_dir), spheres, depth + 1, MAX_DEPTH);
		vec3f refraction;
		vec3f refrac_dir;
		vec3f k;
		float c;
		if (!inside) {
			refract(ray, hitinfo, sphere->nt_, inside, refrac_dir);
			c = -dot(ray.dir_, hitinfo.normal_);
			k = 1;
			refraction = trace(Ray(hitinfo.phit_ + hitinfo.normal_*bias, refrac_dir), spheres, depth + 1, MAX_DEPTH);
		}
		else {
			k._x = exp(-sphere->surfaceColor_._x*hitinfo.distance_);
			k._y = exp(-sphere->surfaceColor_._y*hitinfo.distance_);
			k._z = exp(-sphere->surfaceColor_._z*hitinfo.distance_);
			if (refract(ray, hitinfo, sphere->nt_, inside, refrac_dir)) {
				c = -dot(refrac_dir, hitinfo.normal_);
				refraction = trace(Ray(hitinfo.phit_ + hitinfo.normal_*bias, refrac_dir), spheres, depth + 1, MAX_DEPTH);
			}
			else {
				surfaceColor = k * reflection;
				return surfaceColor + sphere->emissionColor_;
			}
		}
		float r0 = pow(sphere->nt_ - 1, 2) / pow(sphere->nt_ + 1, 2);
		float r = r0 + (1 - r0)*pow(1 - c, 5);
		surfaceColor = k * (reflection*r + refraction * (1 - r)*sphere->transparency_);
		return surfaceColor + sphere->emissionColor_;
	}
	else {
		for (unsigned int i = 0; i<spheres.size(); i++) {
			if (spheres[i].emissionColor_._x>0) {
				vec3f transmision = 1;
				vec3f lightDirection = spheres[i].center_ - hitinfo.phit_;
				lightDirection.normalize();
				for (unsigned int j = 0; j<spheres.size(); j++) {
					if (i != j && spheres[j] != *sphere) {
						Hitinfo hit2;
						if (spheres[j].intersect(Ray(hitinfo.phit_ + hitinfo.normal_*bias, lightDirection), hit2)) {
							transmision = 0;
							break;
						}
					}
				}
				//surfaceColor += lambertin(hitinfo, lightDirection, transmision, &spheres[i], sphere);
				surfaceColor += blinn_phong(hitinfo, ray, lightDirection, transmision, &spheres[i], sphere,100);
			}
		}
	}
	return surfaceColor + sphere->emissionColor_;
}

void m_render_perspective(int w, int h, float vtheta,std::vector<Sphere> &spheres, float sample_num,int MAX_DEPTH, color **image) {
	float tanh = tanf(vtheta / 360 * 3.14f);
	float ratio = float(w) / float(h);
	float height = tanh * 2;
	float width = height * ratio;
	float invheight = 1.0f / h;
	float invwidth = 1.0f / w;
	float halfwidth = width / 2.0f;
	float halfheight = height / 2.0f;
	srand(unsigned(time(NULL)));
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			vec3f reslut;
			for (int p = 0; p < sample_num; p++) {
				for (int q = 0; q < sample_num; q++) {
					float randomx = (rand() % 99 / double(100) + p) / float(sample_num);
					float randomy = (rand() % 99 / double(100) + q) / float(sample_num);
					float dx = (x + randomx)*invwidth*width - halfwidth;
					float dy = (y + randomy)*invheight*height - halfheight;
					reslut = reslut + trace(Ray(vec3f(0), vec3f(dx, dy, -1)), spheres, 0, MAX_DEPTH);
				}
			}
			reslut = reslut / pow(sample_num, 2);
			reslut = reslut * 255;
			image[h - y - 1][x] = color(reslut._x, reslut._y, reslut._z);
		}
	}
}

void m_render_orthorgraphics(int w, int h, std::vector<Sphere> &spheres, float sample_num,int MAX_DEPTH, color **image) {
	float invHeight = 1.0f / float(h);
	float mx = float(w) / 2.0f;
	float my = float(h) / 2.0f;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			vec3f reslut;
			for (int p = 0; p < sample_num; p++) {
				for (int q = 0; q < sample_num; q++) {
					srand(unsigned(time(NULL)));
					float randomx = (rand() % 99 / double(100) + p) / float(sample_num);
					float randomy = (rand() % 99 / double(100) + q) / float(sample_num);
					float dx = invHeight * (x - mx + randomx);                                                    //将height从[0,h]放缩到[0,1] 同理width将放缩到[0,ratio]
					float dy = invHeight * (y - my + randomy);
					reslut = reslut + trace(Ray(vec3f(dx, dy, 0), vec3f(dx, dy, -1)), spheres, 0, MAX_DEPTH);
				}
			}
			reslut = reslut / pow(sample_num, 2);
			reslut = reslut * 255;
			image[h - y - 1][x] = color(reslut._x, reslut._y, reslut._z);
		}
	}
}

void demo_sphere_world(int w, int h, int MAX_DEPTH) {
	std::vector<Sphere> spheres;
	/*spheres.push_back(Sphere(vec3f(0,0,0),20,vec3f(0,0,0),1,0));
    spheres.push_back(Sphere(vec3f(0,10,-20),100,vec3f(0.4f,0.6f,0.1f),0,0));
	spheres.push_back(Sphere(vec3f(5,0,-10),2,vec3f(0.6f,0.1f,0.3f),0.6f,0.4f));*/
	/* position, radius, surface color, reflectivity, transparency, emission color*/
	//spheres.push_back(Sphere(vec3f( 0.0, 0, -10), 3, vec3f(0.20, 0.20, 0.20), 0.4, 0.6,vec3f(0.7f,0.2f,0.2f)));
	//spheres.push_back(Sphere(vec3f(0.0, -104, -20), 100, vec3f(0.6f, 0.1f, 0.1f), 0.9, 0.1f,vec3f(0)));
	spheres.push_back(Sphere(vec3f(5.0, -1, -15), 2, vec3f(0.10, 0.46, 0.46), 0.6f, 0.4f, vec3f(0), 1.0f));
	spheres.push_back(Sphere(vec3f(5.0, 0, -25), 3, vec3f(0.65, 0.77, 0.97), 0.6f, 0.4f, vec3f(0), 1.33f));
	spheres.push_back(Sphere(vec3f(-5.5, 0, -15), 3, vec3f(0.65, 0.5f, 0.1f), 0.2f, 0.8f, vec3f(0), 1.6f));
	// light
	spheres.push_back(Sphere(vec3f(0.0, 10, -30), 3, vec3f(0.6f, 0.00, 0.00), 1, 0.0, vec3f(1)));
	spheres.push_back(Sphere(vec3f(0, 30, 0), 2, vec3f(0.6f, 0.00, 0.00), 0, 0, vec3f(1)));

	color **mat = new color*[h];
	for (unsigned i = 0; i < h; i++) {
		mat[i] = new color[w];
	}

	m_render_perspective(w, h, 60, spheres, 16, 5, mat);
	//m_render_orthorgraphics(w, h, spheres, 5, mat);

	generate_color_PPM("D:/github/Raytracer/Raytrace/images/sphere_world_pers_2.ppm", w, h, mat);
	//generate_color_PPM("D:/github/Raytracer/Raytrace/images/sphere_world_ortho_1.ppm", w, h, mat);
}


int main() {
	int w = 640;
	int h = 480;
	demo_sphere_world(w, h, 10);
	
	system("pause");
	return 0;
}
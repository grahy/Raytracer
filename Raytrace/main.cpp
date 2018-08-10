#include"vec3f.h"
#include"sphere.h"
#include"hitinfo.h"
#include<vector>
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
		return vec3f(3);
	}
	vec3f surfaceColor = 0;
	float bias = 1e-2;
	bool inside = false;
	if (dot(ray.dir_, hitinfo.normal_)>0)hitinfo.normal_ = -hitinfo.normal_, inside = true;
	if ((sphere->transparency_>0 || sphere->reflection_>0) && (depth<MAX_DEPTH)) {
		float facingratio = -dot(ray.dir_, hitinfo.normal_);
		float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
		vec3f reflec_dir = ray.dir_ - hitinfo.normal_ * 2 * dot(ray.dir_, hitinfo.normal_);
		reflec_dir.normalize();

		vec3f reflection = trace(Ray(hitinfo.phit_ + hitinfo.normal_*bias, reflec_dir), spheres, depth + 1, MAX_DEPTH);
		vec3f refraction = 0;
		if (sphere->transparency_>0) {
			float ior = 1.1f; float eta = (inside) ? ior : (1 / ior);
			float cosi = -dot(hitinfo.normal_, ray.dir_);
			float k = 1 - eta * eta*(1 - cosi * cosi);
			vec3f refrac_dir = ray.dir_*eta + hitinfo.normal_*(eta*cosi - sqrt(k));
			refrac_dir.normalize();
			refraction = trace(Ray(hitinfo.phit_ - hitinfo.normal_ * bias, refrac_dir), spheres, depth + 1,MAX_DEPTH);
		}
		surfaceColor = (
			reflection * fresneleffect +
			refraction * (1 - fresneleffect) * sphere->transparency_) * sphere->surfaceColor_;
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
				float num = max(float(0), dot(hitinfo.normal_,lightDirection));
				surfaceColor += sphere->surfaceColor_ * transmision *
					num * spheres[i].emissionColor_;
			}
		}
	}
	return surfaceColor + sphere->emissionColor_;
}

void m_render_perspective(int w, int h, float vtheta,std::vector<Sphere> &spheres,int MAX_DEPTH, color **image) {
	float tanh = tanf(vtheta / 360 * 3.14f);
	float ratio = float(w) / float(h);
	float height = tanh * 2;
	float width = height * ratio;
	float invheight = 1.0f / h;
	float invwidth = 1.0f / w;
	float halfwidth = width / 2.0f;
	float halfheight = height / 2.0f;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			float dx = (x + 0.5f)*invwidth*width - halfwidth;
			float dy = (y + 0.5f)*invheight*height - halfheight;
			vec3f target(dx, dy, -1);
			vec3f reslut = trace(Ray(vec3f(0), target.normalize()), spheres, 0, MAX_DEPTH);
			reslut = reslut * 70;
			image[h - y - 1][x] = color(reslut._x, reslut._y, reslut._z);
		}
	}
}

void m_render_orthorgraphics(int w, int h, std::vector<Sphere> &spheres, int MAX_DEPTH, color **image) {
	float invHeight = 1.0f / float(h);
	float mx = (float)w / 2.0f;
	float my = float(h) / 2.0f;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			float dx = invHeight * (x - mx + 0.5f);                                                    //将height从[0,h]放缩到[0,1] 同理width将放缩到[0,ratio]
			float dy = invHeight * (y - my + 0.5f);
			vec3f reslut = trace(Ray(vec3f(dx,dy,0), vec3f(dx,dy,-1)), spheres, 0, MAX_DEPTH);
			reslut = reslut * 70;
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
	spheres.push_back(Sphere(vec3f( 0.0, 0, -10), 3, vec3f(0.20, 0.20, 0.20), 0.4, 0.6,vec3f(0.7f,0.2f,0.2f)));
	spheres.push_back(Sphere(vec3f(0.0, -104, -20), 100, vec3f(0.6f, 0.1f, 0.1f), 1, 0));
	spheres.push_back(Sphere(vec3f( 5.0,     -1, -15),     2, vec3f(0.90, 0.76, 0.46), 0.6f, 0.4f));
	spheres.push_back(Sphere(vec3f( 5.0,      0, -25),     3, vec3f(0.65, 0.77, 0.97), 1, 0.4f));
	spheres.push_back(Sphere(vec3f(-5.5,      0, -15),     3, vec3f(0.90, 0.90, 0.90), 1, 0.4f));
	// light
	spheres.push_back(Sphere(vec3f(0.0, 10, -30), 3, vec3f(0.00, 0.00, 0.00), 0, 0.0, vec3f(3)));

	color **mat = new color*[h];
	for (unsigned i = 0; i < h; i++) {
		mat[i] = new color[w];
	}

	//m_render_perspective(w, h, 60, spheres, 5, mat);
	m_render_orthorgraphics(w, h, spheres, 5, mat);

	//generate_color_PPM("D:/github/Raytracer/Raytrace/images/sphere_world_pers_1.ppm", w, h, mat);
	generate_color_PPM("D:/github/Raytracer/Raytrace/images/sphere_world_ortho_1.ppm", w, h, mat);
}


int main() {
	int w = 640;
	int h = 480;
	demo_sphere_world(w, h, 5);
	system("pause");
	return 0;
}
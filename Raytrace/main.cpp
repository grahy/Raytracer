
#include"objects.h"
#include<vector>
#include"ioimages.h"
#include"camera.h"


void demo_sphere_world(int w, int h, int MAX_DEPTH) {
	texture2d woodtexture(readBmp("D:/images/woodframe.bmp"));
	std::vector<objects*> obj;
	//gruond
	obj.push_back(&Triangle(vertex(vec3f(-50, -10, 10), textcoord(0, 0)),
		vertex(vec3f(50, -10, 10), textcoord(woodtexture._width - 1, 0)),
		vertex(vec3f(-50, -10, -50), textcoord(0, woodtexture._height - 1)),
		vec3f(0), 0, 0, 1, vec3f(0), &woodtexture));
	obj.push_back(&Triangle(vertex(vec3f(50, -10, 10), textcoord(woodtexture._width - 1, 0)),
		vertex(vec3f(50, -10, -50), textcoord(woodtexture._width - 1, woodtexture._height - 1)),
		vertex(vec3f(-50, -10, -50), textcoord(0, woodtexture._height - 1)),
		vec3f(0), 0, 0, 1, vec3f(0), &woodtexture));

	//frontwall
	obj.push_back(&Triangle(vertex(vec3f(-50, -10, -50), textcoord(0, 0)),
		vertex(vec3f(50, -10, -50), textcoord(woodtexture._width - 1, 0)),
		vertex(vec3f(-50, 60, -50), textcoord(0, woodtexture._height - 1)),
		vec3f(0), 0, 0, 1, vec3f(0), &woodtexture));
	obj.push_back(&Triangle(vertex(vec3f(50, -10, -50), textcoord(woodtexture._width - 1, 0)),
		vertex(vec3f(50, 60, -50), textcoord(woodtexture._width - 1, woodtexture._height - 1)),
		vertex(vec3f(-50, 60, -50), textcoord(0, woodtexture._height - 1)),
		vec3f(0), 0, 0, 1, vec3f(0), &woodtexture));

	//
	//obj.push_back(&Triangle(vec3f(-600, 0, -400), vec3f(600, 0, -400), vec3f(600, 0, 10), vec3f(0.2f, 0.1f, 0.6f), 1, 0, 1));
	//obj.push_back(&Sphere(vec3f(0,0,0),20,vec3f(0,0,0),1,0));
    //obj.push_back(&Sphere(vec3f(0,10,-20),100,vec3f(0.4f,0.6f,0.1f),0,0));
	obj.push_back(&Sphere(vec3f(5,0,-10),2,vec3f(0,0,0.8f),0,1));
	// position, radius, surface color, reflectivity, transparency, emission color
	obj.push_back(&Sphere(vec3f( -3.0, 0, -10), 3, vec3f(0.8f, 0 ,0.00), 1, 0,vec3f(0),1.33f));
	//obj.push_back(&Sphere(vec3f(0.0, -104, -20), 100, vec3f(0.6f, 0.6f, 0.5f), 0, 0,vec3f(0)));
	//obj.push_back(&Sphere(vec3f(5.0, -1, -15), 2, vec3f(0.10, 0.46, 0.46), 0.6f, 0.4f, vec3f(0), 1.0f));
	obj.push_back(&Sphere(vec3f(5.0, 0, -25), 3, vec3f(0, 0.8f, 0), 1, 0, vec3f(0), 1.33f));
	//obj.push_back(&Sphere(vec3f(-5.5, 0, -15), 3, vec3f(0.45, 0.3f, 0.2f), 0.1f, 0.9f, vec3f(0), 1.6f));
	// light
	//obj.push_back(&Sphere(vec3f(0.0, 10, -30), 3, vec3f(0), 0, 1, vec3f(1)));
	obj.push_back(&Sphere(vec3f(0, 0, 20), 2, vec3f(0), 0, 0, vec3f(1)));

	color **img = new color*[h];
	for (int i = 0; i < h; i++) {
		img[i] = new color[w];
	}

	camera cam(vec3f(0, 3, 3), vec3f(0, 0, -1), vec3f(0, 1, 0), -1, 100, 60);
	cam.useSample(4);
	cam.process_img(obj, 5, img, w, h);

	//m_render_perspective(w, h, 60, obj, 2, 5, img);
	//m_render_perspective(w, h, 60, obj, 4, 0.06f, 15, 5, mat);
	//m_render_orthorgraphics(w, h, spheres, 5, mat);

	generate_color_PPM("D:/github/Raytracer/Raytrace/images/room_2.ppm", w, h, img);
	//generate_color_PPM("D:/github/Raytracer/Raytrace/images/sphere_world_ortho_1.ppm", w, h, mat);
}


int main() {
	int w = 640;
	int h = 480;
	demo_sphere_world(w, h, 5);
	/*std::vector<objects*> obj;
	obj.push_back(&Triangle(vec3f(-500, -10, 10), vec3f(500, -10, 10), vec3f(-500, -10, -500), vec3f(0.3f, 0.6f, 0.8f), 1, 0, 1));
	obj.push_back(&Sphere(vec3f(5, 0, -10), 2, vec3f(0.4, 0.1f, 0.4f), 0.6f, 0.4f));

	auto temp = getObjData(obj[0]);
	std::cout << temp->surfaceColor() << std::endl;*/

	system("pause");
	return 0;
}
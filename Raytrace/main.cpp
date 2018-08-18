
#include"objects.h"
#include<vector>
#include"ioimages.h"
#include"camera.h"


void demo_room(int w, int h, int MAX_DEPTH) {
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

	

	camera cam(vec3f(0, 3, 3), vec3f(0, 3, -3), vec3f(0, 1, 0), 1, INFINITY, 60);
	cam.useAntialising(2);
	cam.process_img(obj, 5, w, h, "D:/github/Raytracer/Raytrace/images/room3.ppm");
}

void demo_sphere_world(int w, int h, int MAX_DEPTH) {
	texture2d texture(readBmp("D:/images/checkboard.bmp"));
	std::vector<objects*> obj;
	//ground
	obj.push_back(&Triangle(vertex(vec3f(-100, -30, 10),textcoord(0,0)),
		vertex(vec3f(100, -30, 10), textcoord(texture._width-1, 0)),
		vertex(vec3f(-100, -30, -110), textcoord(0, texture._height-1)),
		vec3f(0.4f, 0, 0.6f), 0, 0, 1, vec3f(0),&texture));
	obj.push_back(&Triangle(vertex(vec3f(100, -30, 10), textcoord(texture._width - 1, 0)),
		vertex(vec3f(100, -30, -110), textcoord(texture._width - 1, texture._height-1)),
		vertex(vec3f(-100, -30, -110), textcoord(0, texture._height - 1)),
		vec3f(0.4f,0,0.6f), 0, 0, 1, vec3f(0), &texture));
	//left
	obj.push_back(&Triangle(vertex(vec3f(-100, -30, 10)),
		vertex(vec3f(-100, -30, -110)),
		vertex(vec3f(-100, 80, 10)),
		vec3f(0.6f, 0.6f, 0), 0, 0, 1, vec3f(0)));
	obj.push_back(&Triangle(vertex(vec3f(-100, -30, -110)),
		vertex(vec3f(-100, 80, -110)),
		vertex(vec3f(-100, 80, 10)),
		vec3f(0.6f, 0.6f, 0), 0, 0, 1, vec3f(0)));
	//right
	obj.push_back(&Triangle(vertex(vec3f(100, -30, 10)),
		vertex(vec3f(100, -30, -110)),
		vertex(vec3f(100, 80, 10)),
		vec3f(0.6f, 0.6f, 0), 0, 0, 1, vec3f(0)));
	obj.push_back(&Triangle(vertex(vec3f(100, -30, -110)),
		vertex(vec3f(100, 80, -110)),
		vertex(vec3f(100, 80, 10)),
		vec3f(0.6f, 0.6f, 0), 0, 0, 1, vec3f(0)));
	//front
	obj.push_back(&Triangle(vertex(vec3f(-100, -30, -110)),
		vertex(vec3f(100, -30, -110)),
		vertex(vec3f(-100, 80, -110)),
		vec3f(0.4f, 0, 0.6f), 0, 0, 1, vec3f(0)));
	obj.push_back(&Triangle(vertex(vec3f(100, -30, -110)),
		vertex(vec3f(100, 80, -110)),
		vertex(vec3f(-100, 80, -110)),
		vec3f(0.4f, 0, 0.6f), 0, 0, 1, vec3f(0)));
	//floor
	obj.push_back(&Triangle(vertex(vec3f(-100, 80, 10)),
		vertex(vec3f(100, 80, 10)),
		vertex(vec3f(-100, 80, -110)),
		vec3f(0.8f, 0.8f, 0.8f), 0, 0, 1, vec3f(0)));
	obj.push_back(&Triangle(vertex(vec3f(100, 80, 10)),
		vertex(vec3f(100, 80, -110)),
		vertex(vec3f(-100, 80, -110)),
		vec3f(0.8f, 0.8f, 0.8f), 0, 0, 1, vec3f(0)));

	obj.push_back(&Sphere(vec3f(15, -5, -20), 4, vec3f(0.8f, 0.4f, 0), 0.4f, 0.6f, vec3f(0.8f, 0.4f, 0), 1));
	obj.push_back(&Sphere(vec3f(-10, -10, -10), 5, vec3f(0.6f, 0.8f, 0.2f), 1, 0, vec3f(0), 1.1f));
	obj.push_back(&Sphere(vec3f(-30, 5, -5), 6, vec3f(0.6f, 0.4f, 0), 1, 0, vec3f(0.6f, 0.4f, 0), 1.6f));
	obj.push_back(&Sphere(vec3f(15, -15, -15), 8, vec3f(0, 0, 0.2f), 0, 1, vec3f(0), 1.33f));

	//light
	obj.push_back(&Sphere(vec3f(0, 75, -60), 1, vec3f(0, 0.4f, 0.8f), 1, 0, vec3f(1), 1.1f));
	obj.push_back(&Sphere(vec3f(70, 40, -30), 1, vec3f(0.7f, 0.4f, 0.2f), 1, 0, vec3f(0.7f, 0.4f, 0.2f), 1.1f));
	obj.push_back(&Sphere(vec3f(-70, 40, -30), 1, vec3f(0.1f, 0.8f, 0.3f), 1, 0, vec3f(0.1f, 0.8f, 0.3f),1.2f));
	obj.push_back(&Sphere(vec3f(4, -6, -3), 3, vec3f(0.6f, 0.4f, 0), 1, 0, vec3f(0.6f, 0.4f, 0), 1.33f));

	camera cam(vec3f(0, 0, 40), vec3f(0, 0, -10), vec3f(0, 1, 0), 1, INFINITE, 60);
	cam.useAntialising(4);
	cam.useSoftShadow();
	//cam.useDOF(0.6f, 30);
	cam.process_img(obj, 5, w, h,"D:/github/Raytracer/Raytrace/images/sphere_world_with_shadowRay3.ppm");
}


int main() {
	int w = 640;
	int h = 480;
	//demo_room(w, h, 5);
	demo_sphere_world(w, h, 10);

	system("pause");
	return 0;
}
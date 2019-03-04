#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cfloat>
#include <math.h>

#include "ray.h"
#include "vec3.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include "PNG_IMAGE.h"
#include "tool.h"


//采样
vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.00000001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
			//这一步会导致浅色强折射材质染上深色材质
			//第一步采样碰撞时，就会乘自己的颜色，越深越不受后续采样影响
			//当向其他地方发射线采样时，color函数同样会乘其他物体材质的颜色
		}
		else {
			return vec3(0, 0, 0);
		}

	}
	else { //天空盒
		vec3 unit_direction = r.direction().unit_vector(); //到[-1, 1]之间
		float t = 0.5 * (unit_direction.y() + 1.0); //到[0, 1]之间
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}

}

int main() {


	int ns = 100;//取样次数

	unsigned char* data = new unsigned char[200 * 100 * 4];
	PNG_IMAGE png(data, 200, 100, 4);

	hitable* list[5];
	list[0] = new sphere(vec3(0, 0, -1), 0.5F, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100.0F, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5F, new metal(vec3(0.8, 0.6, 0.2)));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5F, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45F, new dielectric(1.5));
	hitable *world = new hitable_list(list, 5);


	vec3 lookfrom(3, 3, 2);
	vec3 lookat(0, 0, -1);
	float dist_to_focus = (lookfrom - lookat).length();//相机与目标点距离
	float aperture = 2.0;
	camera cam(lookfrom, lookat, vec3(0,1,0), 20, png.weight()/png.height(), aperture, dist_to_focus);

	for (int row = 0; row < png.height(); row++) {
		for (int col = 0; col < png.weight(); col++) {
			vec3 colo(0, 0, 0);

			//多次取样
			for (int s = 0; s < ns; s++) {
				float u = float(col + rand_uniform()) / float(png.weight());
				float v = float(png.height() - row - 1 + rand_uniform()) / float(png.height());
				ray r = cam.get_ray(u, v);
				colo += color(r, world, 0);
			}
			colo /= ns;
			colo = vec3(sqrt(colo[0]), sqrt(colo[1]), sqrt(colo[2]));

			png.getNum(row, col, 0) = unsigned char(255.99 * colo[0]);
			png.getNum(row, col, 1) = unsigned char(255.99 * colo[1]);
			png.getNum(row, col, 2) = unsigned char(255.99 * colo[2]);
			png.getNum(row, col, 3) = 255;

		}
	}

	png.write("fov lookat blur.png");
	delete world;
	for (int i = 0; i < sizeof(list) / sizeof(list[0]); i++)
		delete list[i];
	return 0;
}
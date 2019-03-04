#pragma once

#include "ray.h"

#include <math.h>

#include "tool.h"

vec3 random_in_unit_disk() {
	vec3 p = 2.0f * vec3(rand_uniform(), rand_uniform(), 0) - vec3(1, 1, 0);
	p = p.unit_vector() * rand_uniform();
	return p;
}

class camera {
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist) {
		lens_radius = aperture / 2;

		float theta = vfov * M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;

		origin = lookfrom;//ԭ��
		w = (lookfrom - lookat).unit_vector();
		u = cross(vup, w).unit_vector();
		v = cross(w, u);

		lower_left_corner = origin - half_width *focus_dist* u - half_height *focus_dist* v - focus_dist*w;
		horizontal = 2*half_width*u*focus_dist;//ˮƽ����
		vertical = vertical = 2*half_height*v*focus_dist;//��ֱ�߶�
		
	}
	ray get_ray(float s, float t) {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin+offset, lower_left_corner + s * horizontal + t * vertical - origin-offset);
	}

	vec3 lower_left_corner; //���½ǣ���������ϵ��
	vec3 horizontal;//ˮƽ����
	vec3 vertical;//��ֱ�߶�
	vec3 origin;//ԭ��
	vec3 u, v, w;
	float lens_radius;

};
#pragma once

#include "hitable.h"
#include "material.h"

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r, material* mat = new lambertian(vec3(0.5, 0.5, 0.5))) : center(cen), radius(r), mat_ptr(mat) {};
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

	vec3 center;
	float radius;
	material* mat_ptr;

	virtual ~sphere() {
		delete mat_ptr;
	}
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center; // A-C
	float a = dot(r.direction(), r.direction()); // a = dot(B, B)
	float b = 2.0 * dot(oc, r.direction()); // b = 2 * dot((A-C), B)
	float c = dot(oc, oc) - radius * radius; // c = dot((A-C), (A-C)) - R^2
	float discriminant = b * b - 4 * a * c; // b^2 - 4*a*c > 0
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / (a*2.0); //近的点
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius; //法向量单位化了

			rec.mat_ptr = this->mat_ptr;
			return true;
		}
		//不折射就不触碰远点(背面看不到，注释掉下面)
		//temp = (-b + sqrt(discriminant)) / (a*2.0); //远的点
		//if (temp < t_max && temp > t_min) {
		//	rec.t = temp;
		//	rec.p = r.point_at_parameter(rec.t);
		//	rec.normal = (rec.p - center) / radius;

		//	rec.mat_ptr = this->mat_ptr;
		//	return true;
		//}
	}
	return false;
}
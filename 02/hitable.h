#pragma once

#include "ray.h"

class material;

struct  hit_record {
	float t;//射线t参数
	vec3 p;//相交点
	vec3 normal;//法向量
	material *mat_ptr;
};

class hitable {
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};
#pragma once

#include "hitable.h"

class hitable_list : public hitable {
public:
	hitable_list() {}
	hitable_list(hitable **l, int n) { list = l; list_size = n; }
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

	hitable **list;
	int list_size;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (int i = 0; i < list_size; i++) { //对列表中所有可碰撞物体进行碰撞，找到碰撞距离最近的
		if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t; //最后碰撞物体的距离 
			//假如第一个采样为5，那么最近采样距离为5，为第二次采样的最大距离
			//当第二次采样时发现采样距离为10，会导致不再采样范围内，从而不采样(碰撞，hit返回false)
			//这样可以检测深度
			rec = temp_rec; //最后碰撞的记录
		}
	}
	return hit_anything;
}
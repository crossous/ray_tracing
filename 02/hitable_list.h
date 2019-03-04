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
	for (int i = 0; i < list_size; i++) { //���б������п���ײ���������ײ���ҵ���ײ���������
		if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t; //�����ײ����ľ��� 
			//�����һ������Ϊ5����ô�����������Ϊ5��Ϊ�ڶ��β�����������
			//���ڶ��β���ʱ���ֲ�������Ϊ10���ᵼ�²��ٲ�����Χ�ڣ��Ӷ�������(��ײ��hit����false)
			//�������Լ�����
			rec = temp_rec; //�����ײ�ļ�¼
		}
	}
	return hit_anything;
}
#pragma once
#include "ray.h"
#include "hitable.h"

#include "tool.h"



vec3 random_in_unit_sphere() {


	vec3 p;
	//do {
	//	p = 2.0 * vec3(u_rand(rand_engine), u_rand(rand_engine), u_rand(rand_engine)) - vec3(1, 1, 1);
	//} while (p.squared_length() >= 1.0);
	p = (2.0 * vec3(rand_uniform(), rand_uniform(), rand_uniform()) - vec3(1, 1, 1)).unit_vector() * rand_uniform();
	return p;
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	//v所在看做来时方向,n为与v(向量!)夹角大于90°的法向量，ni/nt为n外/n内(来时方向看做外)，refracted是折射方向
	vec3 uv = v.unit_vector();
	float dt = dot(uv, n.unit_vector());
	//非向量夹角看做theta外，互余角为90-theta外，夹角为90°+(90°-theta外)
	//dot单位向量得到夹角的cos值，根据诱导公式，dt等同于-cos(theta外)
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	//1 - dt^2 = sin^2(theta外)，ni/nt=sin(theta内)/sin(theta外)，结果是cos^2(theta内)
	if (discriminant > 0) {//出射角为0或不存在，既入射角为垂直射入或数值错误，才可能返回false
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		//分别为出射向量的横向和纵向向量，加在一起组成向量
		return true;
	}
	else {
		return false;
	}
}

float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	//r0 = ((ni-nt)/(ni+nt))^2
	//   = [((ni-nt)/nt) / ((ni+nt)/nt)]^2
	//   = ([(ni/nt)-1] / [(ni/nt)+1])^2
	//   = ([1-(ni/nt)] / [(ni/nt)+1])^2
	r0 = r0 * r0;
	//R(theta_i) = (1-r0)(1-cos(theta_i))^5
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
	lambertian(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();//新的采样目标
		scattered = ray(rec.p, target - rec.p);// 新的采样射线，兰伯特会随机打线
		attenuation = albedo; //衰减 = 折射率
		return true;
	}
	vec3 albedo; //可看做颜色，假如颜色为0,0,0 那么外界直接衰减为无
};

class metal : public material {
public:
	metal(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(r_in.direction().unit_vector(), rec.normal);
		scattered = ray(rec.p, reflected);//金属不会随机打线，入射出射公式严格
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);//出射线与法向量的夹角在正负90间？？？包括折射？？？
	}

	vec3 albedo;
	float fuzz;
};

//模糊效果金属
class sp_metal : public material {
public:
	sp_metal(const vec3& a, float f) : albedo(a) {
		fuzz = f < 1 ? f : 1;
	}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(r_in.direction().unit_vector(), rec.normal);
		scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());//金属不会随机打线，入射出射公式严格
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);//出射线与法向量的夹角在正负90间？？？包括折射？？？
	}

	vec3 albedo;
	float fuzz;
};

class dielectric : public material {
public:
	dielectric(float ri) : ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
		vec3 outward_normal; //出射法向量
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;

		float reflect_prob;
		float cosine;

		if (dot(r_in.direction(), rec.normal) > 0) { //向量夹角小于90°，既从内部射出
			outward_normal = -rec.normal;//都转换为法向量与射线向量夹角大于90°的情况
			ni_over_nt = ref_idx;
			//cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else { //向量夹角大于等于90°，外部射入
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {//能折射
			reflect_prob = schlick(cosine, ref_idx);
		}
		else {
			reflect_prob = 1.0;
		}
		if (rand_uniform() < reflect_prob) {
			scattered = ray(rec.p, reflected);
		}
		else {
			scattered = ray(rec.p, refracted);
		}
		return true;
	}
	float ref_idx;
};
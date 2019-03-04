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
	//v���ڿ�����ʱ����,nΪ��v(����!)�нǴ���90��ķ�������ni/ntΪn��/n��(��ʱ��������)��refracted�����䷽��
	vec3 uv = v.unit_vector();
	float dt = dot(uv, n.unit_vector());
	//�������нǿ���theta�⣬�����Ϊ90-theta�⣬�н�Ϊ90��+(90��-theta��)
	//dot��λ�����õ��нǵ�cosֵ�������յ���ʽ��dt��ͬ��-cos(theta��)
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	//1 - dt^2 = sin^2(theta��)��ni/nt=sin(theta��)/sin(theta��)�������cos^2(theta��)
	if (discriminant > 0) {//�����Ϊ0�򲻴��ڣ��������Ϊ��ֱ�������ֵ���󣬲ſ��ܷ���false
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		//�ֱ�Ϊ���������ĺ������������������һ���������
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
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();//�µĲ���Ŀ��
		scattered = ray(rec.p, target - rec.p);// �µĲ������ߣ������ػ��������
		attenuation = albedo; //˥�� = ������
		return true;
	}
	vec3 albedo; //�ɿ�����ɫ��������ɫΪ0,0,0 ��ô���ֱ��˥��Ϊ��
};

class metal : public material {
public:
	metal(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(r_in.direction().unit_vector(), rec.normal);
		scattered = ray(rec.p, reflected);//��������������ߣ�������乫ʽ�ϸ�
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);//�������뷨�����ļн�������90�䣿�����������䣿����
	}

	vec3 albedo;
	float fuzz;
};

//ģ��Ч������
class sp_metal : public material {
public:
	sp_metal(const vec3& a, float f) : albedo(a) {
		fuzz = f < 1 ? f : 1;
	}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(r_in.direction().unit_vector(), rec.normal);
		scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());//��������������ߣ�������乫ʽ�ϸ�
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);//�������뷨�����ļн�������90�䣿�����������䣿����
	}

	vec3 albedo;
	float fuzz;
};

class dielectric : public material {
public:
	dielectric(float ri) : ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
		vec3 outward_normal; //���䷨����
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;

		float reflect_prob;
		float cosine;

		if (dot(r_in.direction(), rec.normal) > 0) { //�����н�С��90�㣬�ȴ��ڲ����
			outward_normal = -rec.normal;//��ת��Ϊ�����������������нǴ���90������
			ni_over_nt = ref_idx;
			//cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else { //�����нǴ��ڵ���90�㣬�ⲿ����
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {//������
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
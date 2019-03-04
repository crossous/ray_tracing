#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>

class vec3 {
public:
	friend std::istream& operator>>(std::istream &is, vec3 &vec);
	friend std::ostream& operator<<(std::ostream &os, const vec3& vec);
	friend inline float dot(const vec3 &v1, const vec3 &v2);
	friend vec3 cross(const vec3 &v1, const vec3 &v2);
	friend inline vec3 reflect(const vec3 &v1, const vec3 &v2);
	friend inline vec3 operator*(const float t, const vec3 &v1);

	vec3() {};
	vec3(float e0, float e1, float e2) {
		e[0] = e0; e[1] = e1; e[2] = e2;
	}
	vec3(const vec3& v2) {
		e[0] = v2.e[0]; e[1] = v2.e[1]; e[2] = v2.e[2];
	}

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const vec3& operator+() const { return *this; }
	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	inline vec3& operator+=(const vec3 &v2) {
		e[0] += v2.e[0]; e[1] += v2.e[1]; e[2] += v2.e[2];
		return *this;
	}
	inline vec3& operator-=(const vec3 &v2) {
		e[0] -= v2.e[0]; e[1] -= v2.e[1]; e[2] -= v2.e[2];
		return *this;
	}
	inline vec3& operator*=(const vec3 &v2) {
		e[0] *= v2.e[0]; e[1] *= v2.e[1]; e[2] *= v2.e[2];
		return *this;
	}
	inline vec3& operator/=(const vec3 &v2) {
		e[0] /= v2.e[0]; e[1] /= v2.e[1]; e[2] /= v2.e[2];
		return *this;
	}
	inline vec3& operator*=(const float t) {
		e[0] *= t; e[1] *= t; e[2] *= t;
		return *this;
	}
	inline vec3& operator/=(const float t) {
		e[0] /= t; e[1] /= t; e[2] /= t;
		return *this;
	}
	inline vec3 operator*(const vec3 &v2) const {
		return vec3(e[0] * v2.e[0], e[1] * v2.e[1], e[2] * v2.e[2]);
	}
	inline vec3 operator/(const vec3 &v2) const {
		return vec3(e[0] / v2.e[0], e[1] / v2.e[1], e[2] / v2.e[2]);
	}
	inline vec3 operator+(const vec3 &v2) const {
		return vec3(e[0] + v2.e[0], e[1] + v2.e[1], e[2] + v2.e[2]);
	}
	inline vec3 operator-(const vec3 &v2) const {
		return vec3(e[0] - v2.e[0], e[1] - v2.e[1], e[2] - v2.e[2]);
	}
	inline vec3 operator*(const float t) const {
		return vec3(e[0] * t, e[1] * t, e[2] * t);
	}
	inline vec3 operator/(const float t) const {
		return vec3(e[0] / t, e[1] / t, e[2] / t);
	}


	inline float length() const {
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline float squared_length() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	inline void make_unit_vector() {
		float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
		e[0] *= k; e[1] *= k; e[2] *= k;
	}
	inline vec3 unit_vector() const{
		return *this / this->length();
	}
private:
	float e[3];
};

inline std::istream& operator>>(std::istream &is, vec3 &vec) {
	is >> vec.e[0] >> vec.e[1] >> vec.e[2];
	return is;
}
inline std::ostream& operator<<(std::ostream &os, const vec3& vec) {
	os << vec.e[0] << " " << vec.e[1] << " " << vec.e[2];
	return os;
}

inline float dot(const vec3 &v1, const vec3 &v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
	return vec3(
		v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
		v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
		v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]
	);
}

inline vec3 reflect(const vec3 &v, const vec3 &n) {
	return v - 2 * dot(v, n.unit_vector()) * n.unit_vector();
}

inline vec3 operator*(const float t, const vec3 &v1) {
	return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}
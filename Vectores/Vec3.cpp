#pragma once
#include <math.h>
#include <iostream>


class Vec3 {
public:
	Vec3() = default;
	Vec3(float x, float y, float z = 0) : x(x), y(y), z(z) {
		
	}
	
	
	friend std::ostream& operator<<(std::ostream& os, const Vec3& dt);
	friend Vec3 operator+(const Vec3& v1, const Vec3& v2);
	friend Vec3 operator-(const Vec3& v1, const Vec3& v2);

	Vec3 operator+=(const Vec3& v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	}
	inline float GetMag() const {
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 Normalize() const {
		float mag = GetMag();
		return Vec3(
			x / mag,
			y / mag,
			z / mag
		);
	}

	float distance_to(const Vec3& v) const {
		Vec3 vectDist = *this - v;
		return vectDist.GetMag();
	}

	inline float dot_product(const Vec3& v) const {
		//return this->GetMag()* v.GetMag()* cos(theta);
		return (this->x * v.x) + (this->y * v.y) + (this->z * v.z);
	}

	float angle_between(const Vec3& v) const {
		return acos(dot_product(v) / (this->GetMag() * v.GetMag()));
	}

	Vec3 cross_product(const Vec3& v) const {
		float rx = this->y * v.z - this->z * v.y;
		float ry = this->x * v.z - this->z * v.x;
		float rz = this->x * v.y - this->y * v.x;
		return Vec3(rx, -ry, rz);
	}

private:
	float x;
	float y;
	float z;
};


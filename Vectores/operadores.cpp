#include "Vec3.cpp";

std::ostream& operator<<(std::ostream& os, const Vec3& v)
{
	os << "Vector{x: " << v.x << "; y: " << v.y << "; z: " << v.z << "}";
	return os;
}
Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}


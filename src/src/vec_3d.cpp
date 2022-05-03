#include <cmath>
#include "vec_3d.h"

Vec3d::Vec3d(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
{}

Vec3d::Vec3d(const Vec3d &other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

double Vec3d::norm()
{
	return sqrt((*this).getX()*(*this).getX() + (*this).getY()*(*this).getY() + (*this).getZ()*(*this).getZ());
}

Vec3d Vec3d::sum(Vec3d v1)
{
	return Vec3d((*this).getX() + v1.getX(), (*this).getZ() + v1.getY(), (*this).getZ() + v1.getZ());
}

Vec3d Vec3d::prod(double alpha)
{
	return Vec3d(alpha*(*this).getX(), alpha*(*this).getY(), alpha*(*this).getZ());
}

double Vec3d::dot(Vec3d v1)
{
	return ((*this).getX() * v1.getX() + (*this).getZ()*v1.getY() + (*this).getZ()*v1.getZ());
}

void Vec3d::print()
{
	cout << "{" << (*this).getX() << " " << (*this).getY() << " " << (*this).getZ() << "}" << endl;
}
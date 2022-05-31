#ifndef VEC_3D_H
#define VEC_3D_H

#include "pvm_misc.h"

class Vec3d
{
public:
	Vec3d(double _x = 0.0, double _y = 0.0, double _z = 0.0);
	Vec3d(const Vec3d &other);
	virtual ~Vec3d(){}

	Vec3d sum(Vec3d v1);
	Vec3d prod(double alpha);
	double dot(Vec3d v1);
	double norm();
	
	double getX() { return this->x; }
	double getY() { return this->y; }
	double getZ() { return this->z; }

	void setX(double _x) { this->x = _x; }
	void setY(double _y) { this->y = _y; }
	void setZ(double _z) { this->z = _z; }
	void print();

private:
	double x;
	double y;
	double z;
};

#endif // !VEC_3D_H
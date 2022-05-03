#ifndef DIPOLAR_SUM_H
#define DIPOLAR_SUM_H

#include <pvm_misc.h>
#include <vec_3d.h>

class DipolarSum
{
public:
	DipolarSum(double resolution, 
			   double external_field, 
			   double pore_sus, 
			   double matrix_sus, 
			   int dim_x, 
			   int dim_y, 
			   int dim_z, 
			   double* map, 
			   double* field);
private:
	double resolution;
	double external_field;
	double pore_sus;
	double matrix_sus;
	int dim_x;
	int dim_y;
	int dim_z;
	double* map;
	double* field;
	double sus_contrast;
	double m_factor;

	void analysis();
	double dipsum(Vec3d &_ref);
};

#endif // !DIPOLAR_SUM_H
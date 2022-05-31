#ifndef FIELD_GRADIENT_H
#define FIELD_GRADIENT_H

#include <vector>
#include "pvm_misc.h"
#include "vec_3d.h"

using namespace std;

class FieldGradient
{
public:
	FieldGradient(double resolution, 
				  int dim_x, 
				  int dim_y, 
				  int dim_z, 
				  uint8_pvm* map, 
				  double* field, 
				  double* gradient);

	virtual ~FieldGradient(){}

	void run(bool _periodicBC = false);
private:
	double resolution;
	int dim_x;
	int dim_y;
	int dim_z;
	uint8_pvm* map;
	double* field;
	double* grads;
	
	void gradient_periodic();
	void gradient_volume();
	double derivative_3pt_centered(int x, int y, int z, double ds);
	double derivative_3pt_centered_border(int x, int y, int z, double ds);	
	double derivative_3pt_tail_border(int x, int y, int z, double ds);
	void print_point(int x, int y, int z);
};

#endif // !FIELD_GRADIENT_H
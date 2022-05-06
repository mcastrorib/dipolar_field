#include <iostream>
#include "dipolar_sum_callers.h"

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void call_dipolar_sum(
	double resolution, bool periodic_flag, double external_field, double pore_sus, double matrix_sus, 
	int dim_x, int dim_y, int dim_z, uint8_pvm* map, double* field)
{
	DipolarSum dpsum(resolution, external_field, pore_sus, matrix_sus, dim_x, dim_y, dim_z, map, field);
	dpsum.run(periodic_flag);
}

void call_field_gradient(
	double resolution, bool periodic_flag, int dim_x, int dim_y, int dim_z, 
	uint8_pvm* map, double* field, double* gradient)
{
	FieldGradient fgrad(resolution, dim_x, dim_y, dim_z, map, field, gradient);
	fgrad.run(periodic_flag);	
}

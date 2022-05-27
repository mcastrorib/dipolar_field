#ifndef DIPOLAR_SUM_CALLS_H
#define DIPOLAR_SUM_CALLS_H

#ifdef __linux__
	#define DIPOLAR_SUM_CALLERS_API
#else
	#define DIPOLAR_SUM_CALLERS_API __declspec(dllexport)
#endif

#include "pvm_misc.h"
#include "dipolar_sum.h"
#include "field_gradient.h"

extern "C" DIPOLAR_SUM_CALLERS_API void call_dipolar_sum(
	double resolution, bool periodic_flag, double external_field, double pore_sus, double matrix_sus, 
	int dim_x, int dim_y, int dim_z, uint8_pvm* map, double* field, uint8_pvm accmode);

extern "C" DIPOLAR_SUM_CALLERS_API void call_field_gradient(
	double resolution, bool periodic_flag, int dim_x, int dim_y, int dim_z, 
	uint8_pvm* map, double* field, double* gradient);

extern "C" DIPOLAR_SUM_CALLERS_API void call_omp(double* v1, double* v2, int size);

#endif
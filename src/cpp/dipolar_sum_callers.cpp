#include <iostream>
#include "dipolar_sum_callers.h"

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void call_dipolar_sum(
	double resolution, bool periodic_flag, double external_field, double pore_sus, double matrix_sus, 
	int dim_x, int dim_y, int dim_z, uint8_pvm* map, double* field, uint8_pvm accmode)
{
	DipolarSum dpsum(resolution, external_field, pore_sus, matrix_sus, dim_x, dim_y, dim_z, map, field);
	dpsum.run(periodic_flag, accmode);
}

void call_field_gradient(
	double resolution, bool periodic_flag, int dim_x, int dim_y, int dim_z, 
	uint8_pvm* map, double* field, double* gradient)
{
	FieldGradient fgrad(resolution, dim_x, dim_y, dim_z, map, field, gradient);
	fgrad.run(periodic_flag);	
}

void call_omp(
	double* v1, double *v2, int size)
{
	const int num_cpu_threads = PVM_GLOBAL_NUM_THREADS;

	#pragma omp parallel num_threads(num_cpu_threads)
	{			
		int64_pvm xjump;
		int start_x, end_x;
		const int tid = omp_get_thread_num();
		get_multi_thread_loop_limits(tid, num_cpu_threads, size, start_x, end_x);

		cout << "hello from thread " << tid << endl;	
		for (int x = start_x; x < end_x; x++)
		{
			int count; for(int c = 0; c < 1000; c++) count++;
			v1[x] += v2[x];
		}
	}
}
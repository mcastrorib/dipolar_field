#include "dipolar_sum_cuda_callers.h"

void dipolar_sum_analysis_periodic_cuda()
{
	cout << "periodic analysis" << endl;
	m_kernel<<<32,1>>>();
}

void dipolar_sum_analysis_volume_cuda()
{
	cout << "volume analysis" << endl;
	m_kernel<<<32,1>>>();
}
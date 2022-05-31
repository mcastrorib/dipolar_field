#include "dipolar_sum_cuda.h"

// Kernels
__global__ void m_kernel(void)
{
	int idx = threadIdx.x + blockIdx.x * blockDim.x;
	printf("Hello from thread %d \n", idx);
}

// Wrappers
void dipolar_sum_analysis_periodic_cuda()
{
	printf("periodic analysis\n");
	m_kernel<<<32,1>>>();
	cudaDeviceSynchronize();
	cudaDeviceReset();
}

void dipolar_sum_analysis_volume_cuda()
{
	printf("volume analysis");
	m_kernel<<<32,1>>>();
	cudaDeviceSynchronize();
	cudaDeviceReset();
}
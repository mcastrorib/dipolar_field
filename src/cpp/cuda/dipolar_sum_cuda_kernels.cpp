#include "dipolar_sum_cuda_kernels.h"

__global__ void m_kernel(void)
{
	int idx = threadIdx.x + blockIdx.x * blockDim.x;
	printf("Hello from thread %d", idx);
}
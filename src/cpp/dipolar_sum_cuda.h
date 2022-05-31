#ifndef DIPOLAR_SUM_CUDA_H
#define DIPOLAR_SUM_CUDA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <cuda.h>

#define HANDLE_ERROR(status) { cudaEvalStatus((status),__FILE__,__LINE__); }

static inline void cudaEvalStatus(cudaError_t status, const char * filename, unsigned int line){
    if (status != cudaSuccess){
        printf("CUDA error in %s (line %i): %s\n",filename,line,cudaGetErrorString(status));
        cudaDeviceReset();
        exit(0);
    }
}

// kernels
__global__ void m_kernel(void);

// wrappers
#ifdef __cplusplus // nvcc defaults .cu files to c++
extern "C" {
#endif
    void dipolar_sum_analysis_periodic_cuda();
    void dipolar_sum_analysis_volume_cuda();
#ifdef __cplusplus
}
#endif

#endif // !DIPOLAR_SUM_CUDA_H
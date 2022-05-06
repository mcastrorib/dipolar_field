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
	int dim_x, int dim_y, int dim_z, uint8_pvm* map, double* field);

extern "C" DIPOLAR_SUM_CALLERS_API void call_field_gradient(
	double resolution, bool periodic_flag, int dim_x, int dim_y, int dim_z, 
	uint8_pvm* map, double* field, double* gradient);

// extern "C" DIPOLAR_SUM_CALLERS_API int call_make_random_grain_rock_model(
// 	int num_families, double* families_data, int dim, int64_pvm resolution, double pixel_size,
// 	float* grains_data, uint8_pvm* rock_model, int ignore_rock_model, int num_threads, uint64_pvm* states);


// extern "C" DIPOLAR_SUM_CALLERS_API int call_make_rock_model_with_circular_or_spherical_pores(
// 	int rock_dim, int num_families, double* families_data, double rock_pixel_size, 
// 	int64_pvm rock_num_res, uint64_pvm s0, uint64_pvm s1, double* elements_data, uint8_pvm* rock_model);


// extern "C" DIPOLAR_SUM_CALLERS_API void call_make_menger_sponge(int rock_dim, 
// 	int num_levels, int* sponge_levels, uint8_pvm* rock_model, const int repeats);


// extern "C" DIPOLAR_SUM_CALLERS_API void call_make_uGC_model(
// 	int rock_dim, int64_pvm pixel_res,double rsm, double rbg, double model_size,
// 	double grid_size, uint8_pvm* out_binary_rock, int64_pvm& micro_pixels, int64_pvm& macro_pixels);


// extern "C" DIPOLAR_SUM_CALLERS_API void call_fill_3d_rock_with_synthetic_grains(
// 	int model_resolution, float voxel_dist, uint8_pvm fill_value, int num_grains, 
// 	float* g_radii, float* g_cx, float* g_cy, float* g_cz, uint8_pvm* model_data);


#endif
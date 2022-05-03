#include <iostream>
#include "dipolar_sum_callers.h"

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void call_naive_python_call(int dim)
{
	std::cout << "hey python, I'm alive!" << endl;
}

// int call_make_random_grain_rock_model(
// 	int num_families, double* families_data, int dim, int64_pvm resolution, double pixel_size,
// 	float* grains_data, uint8_pvm* rock_model, int ignore_rock_model, int num_threads, uint64_pvm* states)
// {
// 	if (ignore_rock_model) {
// 		rock_model = nullptr;
// 	}

// 	vector<SyntheticGrainFamilyInfo> families(num_families);

// 	int fi5;
// 	for (int fi = 0; fi < num_families; fi++)
// 	{
// 		fi5 = 5 * fi;
// 		families[fi].min_r = families_data[fi5];
// 		families[fi].max_r = families_data[fi5 + 1];
// 		families[fi].min_overlapp_dist = families_data[fi5 + 2];
// 		families[fi].max_overlapp_dist = families_data[fi5 + 3];
// 		families[fi].chance = families_data[fi5 + 4];
// 	}
// 	int num_grains = 0;
// 	if (dim == 2)
//  	{
// 		vector<Synthetic2DGrain> grains;
// 		num_grains = make_2D_random_grain_rock_model(families, resolution,
// 			pixel_size, grains, rock_model, states[0], states[1]);

// 		int ig4;
// 		for (int ig = 0; ig < grains.size(); ig++)
// 		{
// 			ig4 = ig * 4;
// 			grains_data[ig4 + 0] = (float)grains[ig].family;
// 			grains_data[ig4 + 1] = (float)grains[ig].radius;
// 			grains_data[ig4 + 2] = (float)grains[ig].xc;
// 			grains_data[ig4 + 3] = (float)grains[ig].yc;
// 		}
// 	}
// 	else if (dim == 3)
// 	{
// 		vector<Synthetic3DGrain> grains;

// 		num_grains = multi_core_make_3D_random_grain_rock_model(families, 
// 			resolution, pixel_size, grains, rock_model, num_threads, states);

// 		int ig5;
// 		for (int ig = 0; ig < grains.size(); ig++)
// 		{
// 			ig5 = ig * 5;
// 			grains_data[ig5 + 0] = (float)grains[ig].family;
// 			grains_data[ig5 + 1] = (float)grains[ig].radius;
// 			grains_data[ig5 + 2] = (float)grains[ig].xc;
// 			grains_data[ig5 + 3] = (float)grains[ig].yc;
// 			grains_data[ig5 + 4] = (float)grains[ig].zc;
// 		}
// 	}

// 	return num_grains;
// }

// ///////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////////////////////

// int call_make_rock_model_with_circular_or_spherical_pores(
// 	int rock_dim, int num_families, double* families_data, double rock_pixel_size, 
// 	int64_pvm rock_num_res, uint64_pvm s0, uint64_pvm s1, double* elements_data, uint8_pvm* rock_model)
// {
// 	bool result;
// 	int i8, num_elements;
// 	vector<RoundPoreFamily> families(num_families);
// 	for (int i = 0; i < num_families; i++)
// 	{
// 		i8 = i * 8;
// 		families[i].min_radius = families_data[i8];
// 		families[i].max_radius = families_data[i8 + 1];
// 		families[i].req_porosity = families_data[i8 + 2];
// 		families[i].use_log_dist = (bool)families_data[i8 + 3];
// 		families[i].overlapping_mode = (int)families_data[i8 + 4];
// 		families[i].no_same_family_overlapping = (bool)families_data[i8 + 5];
// 		families[i].min_overlapped_distance = families_data[i8 + 6];
// 		families[i].max_overlapped_distance = families_data[i8 + 7];
// 	}

// 	if (rock_dim == 3)
// 	{
// 		vector<SyntheticSphereInfo> spheres;
// 		result = make_3D_rock_model_with_spherical_pores(families,
// 			rock_pixel_size, rock_num_res, s0, s1, spheres, rock_model);

// 		int i6;
// 		for (int i = 0; i < spheres.size(); i++)
// 		{
// 			i6 = i * 6;
// 			elements_data[i6 + 0] = spheres[i].family;
// 			elements_data[i6 + 1] = spheres[i].radius;
// 			elements_data[i6 + 2] = spheres[i].porosity;
// 			elements_data[i6 + 3] = spheres[i].xc;
// 			elements_data[i6 + 4] = spheres[i].yc;
// 			elements_data[i6 + 5] = spheres[i].zc;
// 		}
// 		num_elements = (int)spheres.size();
// 	}
// 	else
// 	{
// 		vector<SyntheticCircleInfo> circles;
// 		result = make_2D_rock_model_with_circular_pores(families, 
// 			rock_pixel_size, rock_num_res, s0, s1, circles, rock_model);

// 		int i5;
// 		for (int i = 0; i < circles.size(); i++)
// 		{
// 			i5 = i * 5;
// 			elements_data[i5 + 0] = circles[i].family;
// 			elements_data[i5 + 1] = circles[i].radius;
// 			elements_data[i5 + 2] = circles[i].porosity;
// 			elements_data[i5 + 3] = circles[i].xc;
// 			elements_data[i5 + 4] = circles[i].yc;
// 		}
// 		num_elements = (int)circles.size();
// 	}

// 	if (result) { return num_elements; }
// 	else { return -1; }
// }

// ///////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////////////////////

// void call_make_menger_sponge(int rock_dim, int num_levels, 
// 	int* sponge_levels, uint8_pvm* rock_model, const int repeats)
// {
// 	vector<int> levels(sponge_levels, sponge_levels + num_levels);
// 	if (rock_dim == 3) { make_3D_menger_sponge(levels, rock_model, repeats); }
// 	else { make_2D_menger_sponge(levels, rock_model, repeats); }
// }

// ///////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////////////////////

// void call_make_uGC_model(
// 	int rock_dim, int64_pvm pixel_res,double rsm, double rbg, double model_size,
// 	double grid_size, uint8_pvm* out_binary_rock, int64_pvm& micro_pixels, int64_pvm& macro_pixels)
// {
// 	if (rock_dim == 3)
// 	{
// 		make_uGC_3D_model(pixel_res, rsm, rbg, model_size, 
// 			grid_size, out_binary_rock, micro_pixels, macro_pixels);
// 	}
// 	else
// 	{
// 		make_uGC_2D_model(pixel_res, rsm, rbg, model_size, 
// 			grid_size, out_binary_rock, micro_pixels, macro_pixels);
// 	}
// }

// ///////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////////////////////

// void call_fill_3d_rock_with_synthetic_grains(
// 	int model_resolution, float voxel_dist, uint8_pvm fill_value, int num_grains, 
// 	float* g_radii, float* g_cx, float* g_cy, float* g_cz, uint8_pvm* model_data)
// {
// 	fill_3d_rock_with_synthetic_grains(
// 		model_resolution, voxel_dist, fill_value, 
// 		num_grains, g_radii, g_cx, g_cy, g_cz, model_data
// 	);
// }
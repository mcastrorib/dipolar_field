#include <cmath>
#include "dipolar_sum.h"

DipolarSum::DipolarSum(double resolution, 
					   double external_field, 
					   double pore_sus, 
					   double matrix_sus, 
					   int dim_x, 
					   int dim_y, 
					   int dim_z, 
					   uint8_pvm* map, 
					   double* field) : resolution(resolution),
						   				external_field(external_field),
						   				pore_sus(pore_sus),
						   				matrix_sus(matrix_sus),
						   				dim_x(dim_x),
						   				dim_y(dim_y),
						   				dim_z(dim_z),
						   				map(map),
						   				field(field)
{
	this->sus_contrast = this->matrix_sus - this->pore_sus;
	this->m_pore = ((4.0/3.0) * M_PI *  pow(0.5 * this->resolution, 3.0) * this->sus_contrast * this->external_field);
	this->m_grain = ((8.0/3.0) * M_PI *  pow(0.5 * this->resolution, 3.0) * this->matrix_sus * this->external_field);
}

void DipolarSum::run(bool _periodicBC, uint8_pvm _accmode)
{
	if(_accmode == ACC_GPU)
	{
		if(_periodicBC) 
			dipolar_sum_analysis_periodic_cuda();
		else
			dipolar_sum_analysis_volume_cuda();
	}
	else 
	{
		if(_periodicBC)	
			(*this).analysis_periodic(_accmode);
		else 
			(*this).analysis_volume(_accmode);
	}
}

void DipolarSum::analysis_volume(uint8_pvm _accmode)
{
	int64_pvm vol = this->dim_x * this->dim_y * this->dim_z;
	ProgressBar pBar((double) vol);
	int working_threads = PVM_GLOBAL_NUM_THREADS;
	if(_accmode == ACC_NONE) working_threads = 1;

	const int num_cpu_threads = working_threads;
	#pragma omp parallel num_threads(num_cpu_threads) shared(vol, pBar)
	{			
		int start_idx, end_idx;
		const int tid = omp_get_thread_num();
		get_multi_thread_loop_limits(tid, num_cpu_threads, vol, start_idx, end_idx);

		int cx, cy, cz;
		IndexConverter converter(this->dim_x, this->dim_y, this->dim_z);
		Vec3d ref(0.0, 0.0, 0.0);	
	
		for (int idx = start_idx; idx < end_idx; idx++)
		{
	
			converter.convert(idx);
			cx = converter.getX();
			cy = converter.getY();
			cz = converter.getZ();

			ref.setX((double) cx);
			ref.setY((double) cy);
			ref.setZ((double) cz);

			if(this->map[idx] == 0)
				this->field[idx] = (*this).dipsum_volume(ref, this->m_pore);
			else
				this->field[idx] = (*this).dipsum_volume(ref, this->m_grain);
			
			#pragma omp critical
			{
				// Update progress bar
				pBar.update(1);
				pBar.print();
			}
		}
	}

	cout << endl;
}

double DipolarSum::dipsum_volume(Vec3d &_ref, double m_factor)
{
	double dsum = 0.0;
	Vec3d ek(0.0, 0.0, 1.0);
	Vec3d curr(0.0, 0.0, 0.0);
	Vec3d dist(0.0, 0.0, 0.0);
	Vec3d unit(0.0, 0.0, 0.0);
	double norm_dist;
	double costheta;
	double Biz;
	long index;

	for(int z = 0; z < this->dim_z; z++)
	{
		for(int y = 0; y < this->dim_y; y++)
		{
			for(int x = 0; x < this->dim_x; x++)
			{
				index = PY2C_IDX3D(x, y, z, this->dim_x, this->dim_y);
				if(this->map[index] != 0)
				{
					curr.setX((double) x);
					curr.setY((double) y);
					curr.setZ((double) z);					
					dist.setX(this->resolution * (curr.getX() - _ref.getX()));
					dist.setY(this->resolution * (curr.getY() - _ref.getY()));
					dist.setZ(this->resolution * (curr.getZ() - _ref.getZ()));
					norm_dist = dist.norm();					
					unit.setX(dist.getX() / (norm_dist + 1e-20));
					unit.setY(dist.getY() / (norm_dist + 1e-20));
					unit.setZ(dist.getZ() / (norm_dist + 1e-20));
					costheta = unit.dot(ek);
					Biz = m_factor * (3.0 * pow(costheta, 2.0) - 1.0);
					Biz /= pow((norm_dist + 1e-20), 3.0);
					dsum += Biz * norm_dist;
				}
			}
		}	
	}

	return dsum;
}

void DipolarSum::analysis_periodic(uint8_pvm _accmode)
{
	int64_pvm vol = this->dim_x * this->dim_y * this->dim_z;
	ProgressBar pBar((double) vol);
	int working_threads = PVM_GLOBAL_NUM_THREADS;
	if(_accmode == ACC_NONE) working_threads = 1;

	const int num_cpu_threads = working_threads;
	#pragma omp parallel num_threads(num_cpu_threads) shared(vol, pBar)
	{			
		int start_idx, end_idx;
		const int tid = omp_get_thread_num();
		get_multi_thread_loop_limits(tid, num_cpu_threads, vol, start_idx, end_idx);

		int cx, cy, cz;
		IndexConverter converter(this->dim_x, this->dim_y, this->dim_z);
		Vec3d ref(0.0, 0.0, 0.0);	
		vector<Vec3d> pointsList;
		for(int idx = 0; idx < 27; idx++) pointsList.push_back(Vec3d(0.0, 0.0, 0.0));
	
		for (int idx = start_idx; idx < end_idx; idx++)
		{	
			converter.convert(idx);
			cx = converter.getX();
			cy = converter.getY();
			cz = converter.getZ();

			ref.setX((double) cx);
			ref.setY((double) cy);
			ref.setZ((double) cz);

			if(this->map[idx] == 0)
				this->field[idx] = (*this).dipsum_periodic(ref, this->m_pore, pointsList);
			else
				this->field[idx] = (*this).dipsum_periodic(ref, this->m_grain, pointsList);
			
			#pragma omp critical
			{
				// Update progress bar
				pBar.update(1);
				pBar.print();
			}
		}
	}

	cout << endl;
}

double DipolarSum::dipsum_periodic(Vec3d &_ref, double m_factor, vector<Vec3d> &_pointsList)
{
	double dsum = 0.0;
	Vec3d ek(0.0, 0.0, 1.0);
	Vec3d curr(0.0, 0.0, 0.0);
	Vec3d dist(0.0, 0.0, 0.0);
	Vec3d unit(0.0, 0.0, 0.0);
	double norm_dist;
	double costheta;
	double Biz;
	long index;

	for(int z = 0; z < this->dim_z; z++)
	{
		for(int y = 0; y < this->dim_y; y++)
		{
			for(int x = 0; x < this->dim_x; x++)
			{
				index = PY2C_IDX3D(x, y, z, this->dim_x, this->dim_y);
				if(this->map[index] != 0)
				{
					curr.setX((double) x);
					curr.setY((double) y);
					curr.setZ((double) z);
					(*this).update_points_list(curr, _pointsList);

					for(int idx = 0; idx < _pointsList.size(); idx++)
					{	
						dist.setX(this->resolution * (_pointsList[idx].getX() - _ref.getX()));
						dist.setY(this->resolution * (_pointsList[idx].getY() - _ref.getY()));
						dist.setZ(this->resolution * (_pointsList[idx].getZ() - _ref.getZ()));
						norm_dist = dist.norm();					
						unit.setX(dist.getX() / (norm_dist + 1e-20));
						unit.setY(dist.getY() / (norm_dist + 1e-20));
						unit.setZ(dist.getZ() / (norm_dist + 1e-20));
						costheta = unit.dot(ek);
						Biz = m_factor * (3.0 * pow(costheta, 2.0) - 1.0);
						Biz /= pow((norm_dist + 1e-20), 3.0);
						dsum += Biz * norm_dist;
					}
				}
			}
		}	
	}
	return dsum;
}

void DipolarSum::update_points_list(Vec3d &_ref, vector<Vec3d> &_pointsList)
{
	int pIndex;
	double x_coords[] = {-(_ref.getX() + 1.0), _ref.getX(), this->dim_x * 2.0 - (_ref.getX() + 1.0)};
	double y_coords[] = {-(_ref.getY() + 1.0), _ref.getY(), this->dim_y * 2.0 - (_ref.getY() + 1.0)};
	double z_coords[] = {-(_ref.getZ() + 1.0), _ref.getZ(), this->dim_z * 2.0 - (_ref.getZ() + 1.0)};

	for(int k = 0; k < 3; k++)
	{
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				pIndex = PY2C_IDX3D(j,i,k,3,3);
				_pointsList[pIndex].setX(x_coords[j]);
				_pointsList[pIndex].setY(y_coords[i]);
				_pointsList[pIndex].setZ(z_coords[k]);
			}		
		}	
	}

	return;
}



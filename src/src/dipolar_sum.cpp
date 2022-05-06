#include <cmath>
#include "dipolar_sum.h"
#include <vec_3d.h>
#include <progress_bar.h>

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
	vector<Vec3d> points_list();
	this->sus_contrast = this->matrix_sus - this->pore_sus;
	this->m_pore = ((4.0/3.0) * M_PI *  pow(0.5 * this->resolution, 3.0) * this->sus_contrast * this->external_field);
	this->m_grain = ((8.0/3.0) * M_PI *  pow(0.5 * this->resolution, 3.0) * this->matrix_sus * this->external_field);
}

void DipolarSum::run(bool _periodicBC)
{
	if(_periodicBC)	(*this).analysis_periodic();
	else (*this).analysis_volume();
}

void DipolarSum::analysis_volume()
{
	long index;
	Vec3d ref(0.0, 0.0, 0.0);
	ProgressBar pBar((double) this->dim_z);

	for(int z = 0; z < this->dim_z; z++)
	{
		for(int y = 0; y < this->dim_y; y++)
		{
			for(int x = 0; x < this->dim_x; x++)
			{
				index = PY2C_IDX3D(x, y, z, this->dim_x, this->dim_y);
				ref.setX(this->resolution * (double) x);
				ref.setY(this->resolution * (double) y);
				ref.setZ(this->resolution * (double) z);

				if(this->map[index] == 0)
					this->field[index] = (*this).dipsum_volume(ref, this->m_pore);
				else
					this->field[index] = (*this).dipsum_volume(ref, this->m_grain);
			}
		}
		// update progress bar
        pBar.update(1);
        pBar.print();		
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
					curr.setX(this->resolution * (double) x);
					curr.setY(this->resolution * (double) y);
					curr.setZ(this->resolution * (double) z);					
					dist.setX(curr.getX() - _ref.getX());
					dist.setY(curr.getY() - _ref.getY());
					dist.setZ(curr.getZ() - _ref.getZ());
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

void DipolarSum::analysis_periodic()
{
	// Initialize points list
	if(this->points_list.size() > 0) 
		this->points_list.clear();
	for(int idx = 0; idx < 27; idx++)
		this->points_list.push_back(Vec3d(0.0, 0.0, 0.0));

	long index;
	Vec3d ref(0.0, 0.0, 0.0);
	ProgressBar pBar((double) this->dim_z);  
	
	for(int z = 0; z < this->dim_z; z++)
	{
		for(int y = 0; y < this->dim_y; y++)
		{
			for(int x = 0; x < this->dim_x; x++)
			{
				index = PY2C_IDX3D(x, y, z, this->dim_x, this->dim_y);
				ref.setX(this->resolution * (double) x);
				ref.setY(this->resolution * (double) y);
				ref.setZ(this->resolution * (double) z);
				if(this->map[index] == 0)	
					this->field[index] = (*this).dipsum_periodic(ref, this->m_pore);
				else
					this->field[index] = (*this).dipsum_periodic(ref, this->m_grain);
			}
		}
		// update progress bar
        pBar.update(1);
        pBar.print();	
	}
	cout << endl;
}

double DipolarSum::dipsum_periodic(Vec3d &_ref, double m_factor)
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
					(*this).update_points_list(curr);

					for(int idx = 0; idx < this->points_list.size(); idx++)
					{	
						dist.setX(this->resolution * this->points_list[idx].getX() - _ref.getX());
						dist.setY(this->resolution * this->points_list[idx].getY() - _ref.getY());
						dist.setZ(this->resolution * this->points_list[idx].getZ() - _ref.getZ());
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

void DipolarSum::update_points_list(Vec3d &_ref)
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
				this->points_list[pIndex].setX(x_coords[j]);
				this->points_list[pIndex].setY(y_coords[i]);
				this->points_list[pIndex].setZ(z_coords[k]);
			}		
		}	
	}

	return;
}



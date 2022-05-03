#include <cmath>
#include "dipolar_sum.h"
#include <vec_3d.h>

DipolarSum::DipolarSum(double resolution, 
					   double external_field, 
					   double pore_sus, 
					   double matrix_sus, 
					   int dim_x, 
					   int dim_y, 
					   int dim_z, 
					   double* map, 
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
	this->m_factor = ((4.0/3.0) * M_PI *  pow(0.5 * this->resolution, 3.0) * this->sus_contrast * this->external_field);
	(*this).analysis();
}

void DipolarSum::analysis()
{
	long index;
	Vec3d ref(0.0, 0.0, 0.0);

	for(int z = 0; z < this->dim_z; z++)
	{
		for(int y = 0; y < this->dim_y; y++)
		{
			for(int x = 0; x < this->dim_x; x++)
			{
				index = PY2C_IDX3D(x, y, z, this->dim_x, this->dim_y);
				if(this->map[index] == 0)
				{
					ref.setX(this->resolution * (double) x);
					ref.setY(this->resolution * (double) y);
					ref.setZ(this->resolution * (double) z);
					this->field[index] = (*this).dipsum(ref);
				}
			}
		}	
	}
}

double DipolarSum::dipsum(Vec3d _ref)
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
					unit = dist.prod(1.0/norm_dist);
					costheta = unit.dot(ek);
					Biz = this->m_factor * (3.0 * pow(costheta, 2.0) - 1.0);
					Biz /= pow(norm_dist, 3.0);
					dsum += Biz * norm_dist;
				}
			}
		}	
	}

	return dsum;
}

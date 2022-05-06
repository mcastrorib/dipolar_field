#include <cmath>
#include "field_gradient.h"
#include <vec_3d.h>
#include <progress_bar.h>

FieldGradient::FieldGradient(double resolution, 
							 int dim_x, 
							 int dim_y, 
							 int dim_z, 
							 uint8_pvm* map, 
							 double* field, 
							 double* gradient) : resolution(resolution),
								   				 dim_x(dim_x),
								   				 dim_y(dim_y),
								   				 dim_z(dim_z),
								   				 map(map),
								   				 field(field),
								   				 grads(gradient)
{}

void FieldGradient::run(bool _periodicBC)
{
	if(_periodicBC)	(*this).gradient_periodic();
	else (*this).gradient_volume();
}

void FieldGradient::gradient_volume()
{}

void FieldGradient::gradient_periodic()
{
	long index;
	double ds = (0.5 / resolution);

	ProgressBar pBar((double) this->dim_z + 4.0);  

	// Shell computation	
	// z = 0 face
	for(int y = 0; y < this->dim_y; y++)
	{
		for(int x = 0; x < this->dim_x; x++)
		{
			index = PY2C_IDX3D(x, y, 0, this->dim_x, this->dim_y);
			grads[index] = (*this).derivative_3pt_centered_border(x, y, 0, ds);
		}
	}
	// update progress bar
    pBar.update(1);
    pBar.print();

	// z = (dim_z - 1) face
	for(int y = 0; y < this->dim_y; y++)
	{
		for(int x = 0; x < this->dim_x; x++)
		{
			index = PY2C_IDX3D(x, y, (this->dim_z - 1), this->dim_x, this->dim_y);
			grads[index] = (*this).derivative_3pt_centered_border(x, y, (this->dim_z - 1), ds);
		}
	}

	// update progress bar
    pBar.update(1);
    pBar.print();	

    // y = 0 face
    for(int z = 1; z < (this->dim_z - 1); z++)
	{
		for(int x = 0; x < this->dim_x; x++)
		{
			index = PY2C_IDX3D(x, 0, z, this->dim_x, this->dim_y);
			grads[index] = (*this).derivative_3pt_centered_border(x, 0, z, ds);
		}
	} 
	// update progress bar
    pBar.update(1);
    pBar.print();	

	// y = (dim_y - 1) face
    for(int z = 1; z < (this->dim_z - 1); z++)
	{
		for(int x = 0; x < this->dim_x; x++)
		{
			index = PY2C_IDX3D(x, (this->dim_y-1), z, this->dim_x, this->dim_y);
			grads[index] = (*this).derivative_3pt_centered_border(x, (this->dim_y - 1), z, ds);
		}
	}   
	// update progress bar
    pBar.update(1);
    pBar.print();	

	// x = 0 face
    for(int z = 1; z < (this->dim_z - 1); z++)
	{
		for(int y = 1; y < (this->dim_y - 1); y++)
		{
			index = PY2C_IDX3D(0, y, z, this->dim_x, this->dim_y);
			grads[index] = (*this).derivative_3pt_centered_border(0, y, z, ds);
		}
	} 
	// update progress bar
    pBar.update(1);
    pBar.print();	

	// x = (dim_x - 1) face
    for(int z = 1; z < (this->dim_z - 1); z++)
	{
		for(int y = 1; y < (this->dim_y - 1); y++)
		{
			index = PY2C_IDX3D((this->dim_x-1), y, z, this->dim_x, this->dim_y);
			grads[index] = (*this).derivative_3pt_centered_border((this->dim_x - 1), y, z, ds);
		}
	} 
	// update progress bar
    pBar.update(1);
    pBar.print();	

	// Core computation
	for(int z = 1; z < (this->dim_z - 1); z++)
	{
		for(int y = 1; y < (this->dim_y - 1); y++)
		{
			for(int x = 1; x < (this->dim_x - 1); x++)
			{
				index = PY2C_IDX3D(x, y, z, this->dim_x, this->dim_y);
				grads[index] = (*this).derivative_3pt_centered(x, y, z, ds);
			}
		}

		// update progress bar
        pBar.update(1);
        pBar.print();	
	}
	cout << endl;
}

double FieldGradient::derivative_3pt_centered(int x, int y, int z, double ds)
{
	int dXp = x + 1;
	int dXm = x - 1;
	double HXp = this->field[PY2C_IDX3D(dXp, y, z, this->dim_x, this->dim_y)];
	double HXm = this->field[PY2C_IDX3D(dXm, y, z, this->dim_x, this->dim_y)];
	double dHX =  ds * (HXp - HXm);  

	int dYp = y + 1;
	int dYm = y - 1;
	double HYp = this->field[PY2C_IDX3D(x, dYp, z, this->dim_x, this->dim_y)];
	double HYm = this->field[PY2C_IDX3D(x, dYm, z, this->dim_x, this->dim_y)];
	double dHY =  ds * (HYp - HYm); 
	
	int dZm = z - 1;
	int dZp = z + 1;
	double HZp = this->field[PY2C_IDX3D(x, y, dZp, this->dim_x, this->dim_y)];
	double HZm = this->field[PY2C_IDX3D(x, y, dZm, this->dim_x, this->dim_y)];
	double dHZ =  ds * (HZp - HZm);

	return sqrt(dHX*dHX + dHY*dHY + dHZ*dHZ);
}

double FieldGradient::derivative_3pt_centered_border(int x, int y, int z, double ds)
{
	int dXp = x + 1 - ((x + 1) == this->dim_x)*1;
	int dXm = x - 1 + ((x - 1) == -1)*1;
	double HXp = this->field[PY2C_IDX3D(dXp, y, z, this->dim_x, this->dim_y)];
	double HXm = this->field[PY2C_IDX3D(dXm, y, z, this->dim_x, this->dim_y)];
	double dHX =  ds * (HXp - HXm);  
	
	int dYp = y + 1 - ((y + 1) == this->dim_y)*1;
	int dYm = y - 1 + ((y - 1) == -1)*1;
	double HYp = this->field[PY2C_IDX3D(x, dYp, z, this->dim_x, this->dim_y)];
	double HYm = this->field[PY2C_IDX3D(x, dYm, z, this->dim_x, this->dim_y)];
	double dHY =  ds * (HYp - HYm); 

	int dZp = z + 1 - ((z + 1) == this->dim_z)*1;
	int dZm = z - 1 + ((z - 1) == -1)*1;
	double HZp = this->field[PY2C_IDX3D(x, y, dZp, this->dim_x, this->dim_y)];
	double HZm = this->field[PY2C_IDX3D(x, y, dZm, this->dim_x, this->dim_y)];
	double dHZ =  ds * (HZp - HZm);

	return sqrt(dHX*dHX + dHY*dHY + dHZ*dHZ);
}
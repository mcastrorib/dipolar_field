#ifndef DIPOLAR_SUM_H
#define DIPOLAR_SUM_H

#include <vector>
#include <pvm_misc.h>
#include <vec_3d.h>

using namespace std;

class DipolarSum
{
public:
	DipolarSum(double resolution, 
			   double external_field, 
			   double pore_sus, 
			   double matrix_sus, 
			   int dim_x, 
			   int dim_y, 
			   int dim_z, 
			   uint8_pvm* map, 
			   double* field);

	virtual ~DipolarSum(){ 
		if(this->points_list.size() > 0)
			this->points_list.clear();
	}

	void run(bool _periodicBC = false, uint8_pvm _accmode = ACC_NONE);

private:
	double resolution;
	double external_field;
	double pore_sus;
	double matrix_sus;
	int dim_x;
	int dim_y;
	int dim_z;
	uint8_pvm* map;
	double* field;
	double sus_contrast;
	double m_pore;
	double m_grain;
	vector<Vec3d> points_list;

	void analysis_volume();
	double dipsum_volume(Vec3d &_ref, double m_factor);
	void analysis_periodic();
	double dipsum_periodic(Vec3d &_ref, double m_factor);
	void update_points_list(Vec3d &_ref);
};

#endif // !DIPOLAR_SUM_H
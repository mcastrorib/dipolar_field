#ifndef INDEXCONVERTER_H
#define INDEXCONVERTER_H

#include <iostream>
#include <string>

using namespace std;

class IndexConverter 
{
    public:
        IndexConverter(int dim_x, 
                       int dim_y, 
                       int dim_z) : dim_x(dim_x), 
                                    dim_y(dim_y), 
                                    dim_z(dim_z){};
        virtual ~IndexConverter(){};

        inline void convert(long index)
        {
            this->z = index / (this->dim_x*this->dim_y);
            this->y = (index % (this->dim_x*this->dim_y)) / this->dim_x;
            this->x = (index % (this->dim_x*this->dim_y)) % this->dim_x;
        }   

        int getX(){ return this->x; }
        int getY(){ return this->y; }
        int getZ(){ return this->z; }
            

    private:
        int dim_x;
        int dim_y;
        int dim_z;
        int x;
        int y;
        int z;
};

#endif // !INDEXCONVERTER_H
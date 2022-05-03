#ifndef PVM_INTS_H
    #define PVM_INTS_H

    #ifdef __linux__
        #include <stdint.h>
        typedef uint8_t int8_pvm;
        typedef uint8_t uint8_pvm;
        typedef int16_t int16_pvm;
        typedef uint16_t uint16_pvm;
        typedef int32_t int32_pvm;
        typedef uint32_t uint32_pvm;
        typedef int64_t int64_pvm;
        typedef uint64_t uint64_pvm;
        typedef unsigned char byte_pvm;
    #else
        typedef char int8_pvm;
        typedef short int16_pvm;
        typedef int int32_pvm;
        typedef long long int int64_pvm;

        typedef unsigned char byte_pvm;
        typedef unsigned char uint8_pvm;
        typedef unsigned short uint16_pvm;
        typedef unsigned int uint32_pvm;
        typedef unsigned long long int uint64_pvm;
    #endif

#define M_PI 3.14159265358979323846
#define M_2PI 2.0 * 3.14159265358979323846

#endif // !PVM_INTS_H

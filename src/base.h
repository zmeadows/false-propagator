#ifndef BASE_H
#define BASE_H

#include <math.h>
#include <complex.h>

typedef unsigned int uint;
typedef double complex cdouble;

cdouble
wave_packet(double x, double y, double x_0, double y_0, double sigma, double kx, double ky);

#endif

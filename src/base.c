#include "base.h"

cdouble
wave_packet(double x, double y, double x_0, double y_0, double sigma, double kx, double ky)
{
    double var = sigma*sigma;
    double tolerance = 1e-50;

    double exparg = -1/(2*var) * ( pow((x-x_0),2) + pow((y-y_0),2) );
    cdouble value = sqrt(1/(M_PI*var)) * cexpl(exparg - I*(kx*x+ky*y));

    if (pow(cabsl(value),2) < tolerance) {
        return 0.0 + 0.0 * I;
    } else return value;
}

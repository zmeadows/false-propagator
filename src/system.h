#ifndef SYSTEM_H
#define SYSTEM_H

#include <complex.h>
#include <math.h>
#include <stdlib.h>

#include "base.h"

struct TwoDimSystem {
    cdouble *V, *psi, *k1, *m1, *k2, *m2, *k3, *m3, *k4;
    double L, dt, dxy, *psi2;
    uint DIM;
    uint steps_per_img;
    uint steps_per_diag;
    double time_elapsed;
};

struct TwoDimSystem*
make_system(double *_V, cdouble *_psi, double _L, double _dt, uint _DIM);

void
cleanup(struct TwoDimSystem *sys);

static inline cdouble
psi_dot(uint i, uint j, cdouble *psi, struct TwoDimSystem *sys);

void advance(struct TwoDimSystem *sys);

double update_psi2(struct TwoDimSystem *sys);


#endif

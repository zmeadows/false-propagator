#include <stdio.h>
#include <complex.h>
#include <math.h>

#include "system.h"
#include "base.h"
#include "anim.h"

#define DIM 201
#define L 100.0

int main(void)
{
    cdouble psi_0[DIM*DIM];
    double V[DIM*DIM];
    double dxy = L/(double)(DIM-1);

    cdouble w1,w2,w3,w4;
    uint i,j;
    for (i=0; i<DIM; ++i) {
        for (j=0; j<DIM; ++j) {
            w1 = wave_packet(i*dxy,j*dxy,L/2+L/4,L/2,5,0,0);
            w2 = wave_packet(i*dxy,j*dxy,L/2-L/4,L/2,5,0,0);
            w3 = wave_packet(i*dxy,j*dxy,L/2,L/2+L/4,5,0,0);
            w4 = wave_packet(i*dxy,j*dxy,L/2,L/2-L/4,5,0,0);
            psi_0[i*DIM+j] = w1+w2+w3+w4;
            V[i*DIM+j] = -10*exp( -pow((i*dxy-L/2),2)-pow((j*dxy-L/2),2)) ;
        }
    }

    struct TwoDimSystem *sys = make_system(V, psi_0, L, 0.01, DIM);

    make_gif(sys, 750, 1000, 400, 150, "kw");

    cleanup(sys);
    return 0;
}

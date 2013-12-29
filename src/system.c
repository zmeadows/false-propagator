#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "system.h"


double
update_psi2(struct TwoDimSystem *sys)
{
    uint i,j;
    double norm = 0;
    double temp;
    for (i=0; i<sys->DIM; ++i) {
        for (j=0; j<sys->DIM; ++j) {
            temp = pow(cabsl(sys->psi[i*sys->DIM+j]),2);
            sys->psi2[i*sys->DIM+j] = temp;
            norm += pow(sys->dxy,2) * temp;
        }
    }

    return norm;
}

struct TwoDimSystem*
make_system(double *_V, cdouble *_psi, double _L, double _dt, uint _DIM)
{
    struct TwoDimSystem *sys = malloc(sizeof (struct TwoDimSystem));

    sys->DIM = _DIM;
    sys->dt = _dt;
    sys->L = _L;
    sys->time_elapsed = 0.0;
    sys->dxy = _L / (double)(_DIM-1);
    sys->steps_per_diag = 10;
    sys->steps_per_img = 100;

    sys->k1 = malloc(_DIM*_DIM * sizeof(cdouble));
    sys->k2 = malloc(_DIM*_DIM * sizeof(cdouble));
    sys->k3 = malloc(_DIM*_DIM * sizeof(cdouble));
    sys->k4 = malloc(_DIM*_DIM * sizeof(cdouble));
    sys->m1 = malloc(_DIM*_DIM * sizeof(cdouble));
    sys->m2 = malloc(_DIM*_DIM * sizeof(cdouble));
    sys->m3 = malloc(_DIM*_DIM * sizeof(cdouble));

    sys->V = malloc(_DIM*_DIM * sizeof(double));
    sys->psi = malloc(_DIM*_DIM * sizeof(cdouble));
    sys->psi2 = malloc(_DIM*_DIM * sizeof(double));

    uint i,j;
    for (i=0; i<_DIM; ++i) {
        for (j=0; j<_DIM; ++j) {
            sys->V[i*_DIM+j] = _V[i*_DIM+j];
            sys->psi[i*_DIM+j] = _psi[i*_DIM+j];
        }
    }

    update_psi2(sys);
    return sys;
}

void
cleanup(struct TwoDimSystem *sys)
{
    free(sys->k1);
    free(sys->k2);
    free(sys->k3);
    free(sys->k4);
    free(sys->m1);
    free(sys->m2);
    free(sys->m3);
    free(sys->V);
    free(sys->psi);
    free(sys->psi2);
    free(sys);
}

/* psi is passed as argument here in order to allow for runge-kutta
 * algorithm, psi_dot does NOT use the member data 'psi' of the system by default */
cdouble
psi_dot(uint i, uint j, cdouble *psi, struct TwoDimSystem *sys)
{
    if (i == 0 || j == 0 || i == sys->DIM-1 || j == sys->DIM-1)
    {
        return 0; // Dirichlet BC (infinite potential at borders)
    }
    else
    {
        cdouble weight = 1/(2*pow(sys->dxy,2));
        cdouble lap = psi[(i+1)*sys->DIM+j] + psi[(i-1)*sys->DIM+j] + psi[i*sys->DIM+(j+1)]
                      + psi[i*sys->DIM+(j-1)] - 4*psi[i*sys->DIM+j];
        cdouble pot = sys->V[i*sys->DIM+j]*psi[i*sys->DIM+j];
        return I * (weight * lap - pot);
    }
};

/* Runge-Kutta 4th Order algorithm */
void
advance(struct TwoDimSystem *sys)
{
    cdouble temp;
    uint i,j;

    /* k1 */
    for (i=0; i<sys->DIM; ++i) {
        for (j=0; j<sys->DIM; ++j) {
            temp = sys->dt * psi_dot(i,j,sys->psi,sys);
            sys->k1[i*sys->DIM+j] = temp;
            sys->m1[i*sys->DIM+j] = sys->psi[i*sys->DIM+j] + 0.5 * temp;
        }
    }

    /* k2 */
    for (i=0; i<sys->DIM; ++i) {
        for (j=0; j<sys->DIM; ++j) {
            temp = sys->dt * psi_dot(i,j,sys->m1,sys);
            sys->k2[i*sys->DIM+j] = temp;
            sys->m2[i*sys->DIM+j] = sys->psi[i*sys->DIM+j] + 0.5 * temp;
        }
    }

    /* k3 */
    for (i=0; i<sys->DIM; ++i) {
        for (j=0; j<sys->DIM; ++j) {
            temp = sys->dt * psi_dot(i,j,sys->m2,sys);
            sys->k3[i*sys->DIM+j] = temp;
            sys->m3[i*sys->DIM+j] = sys->psi[i*sys->DIM+j] + temp;
        }
    }

    /* k4 */
    for (i=0; i<sys->DIM; ++i) {
        for (j=0; j<sys->DIM; ++j) {
            sys->k4[i*sys->DIM+j] = sys->dt * psi_dot(i,j,sys->m3,sys);
        }
    }

    cdouble t1,t2,t3,t4;
    for (i=0; i<sys->DIM; ++i) {
        for (j=0; j<sys->DIM; ++j) {
            t1 = sys->k1[i*sys->DIM+j];
            t2 = 2*sys->k2[i*sys->DIM+j];
            t3 = 2*sys->k3[i*sys->DIM+j];
            t4 = sys->k4[i*sys->DIM+j];

            sys->psi[i*sys->DIM+j] += (1/6.0) * (t1+t2+t3+t4);
        }
    }

    sys->time_elapsed += sys->dt;
}

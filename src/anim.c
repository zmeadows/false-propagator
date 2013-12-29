#include "anim.h"
#include <mgl2/mgl_cf.h>
#include <mgl2/data_cf.h>


void make_gif(struct TwoDimSystem *sys, uint pixel_DIM, double end_time, uint num_stills, uint ms, char *color_scheme)
{
    HMGL gr = mgl_create_graph(pixel_DIM,pixel_DIM);
    mgl_set_def_sch(gr, color_scheme);

    HMDT pixel_data = mgl_create_data();

    mgl_start_gif (gr, "test.gif", ms);

    sys->time_elapsed = 0;

    double norm;
    uint i;
    uint num_steps = end_time/sys->dt;
    printf("NUM STEPS: %d\n", num_steps);
    printf("STEP PER IMAGE: %d\n", num_steps/num_stills);
    for(i=0;i<num_steps;i++) {
       advance(sys);
       if (i % (num_steps/num_stills) == 0) {
         mgl_new_frame(gr);
         norm = update_psi2(sys);
         printf("%g \n",norm);
         mgl_data_set_double(pixel_data, sys->psi2,sys->DIM,sys->DIM,1);
         mgl_set_range_dat (gr, 'c', pixel_data, 0);
         mgl_dens(gr, pixel_data, "", "");
         mgl_end_frame(gr);
       }
    }

    mgl_close_gif(gr);
    mgl_delete_graph(gr);
}

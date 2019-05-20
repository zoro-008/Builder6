#ifndef _TSAICALIBRATOR_H_
#define _TSAICALIBRATOR_H_

#include "calibstruct.h"

//////////////////////////////////////////////////////////////////////
// C type functions called by cpp functions 
//////////////////////////////////////////////////////////////////////

extern "C" 
{
	//void initialize_photometrics_parms();
	//void print_cp_cc_data(FILE *fp,	struct camera_parameters *cp,	struct calibration_constants *cc);
	void __stdcall coplanar_calibration_with_full_optimization ();

	void __stdcall image_coord_to_world_coord(double Xfd, double Yfd, double zw, double *xw, double *yw);
        void __stdcall world_coord_to_image_coord (double xw, double yw, double zw, double *Xf, double *Yf);

        void __stdcall distorted_to_undistorted_image_coord (double Xfd, double Yfd, double *Xfu, double *Yfu);
        void __stdcall undistorted_to_distorted_image_coord (double Xfu, double Yfu, double *Xfd, double *Yfd);

	//global to communicate with C-Module
	extern camera_parameters *cp;
	extern calibration_data *cd;
	extern calibration_constants *cc;
	extern char camera_type[];	
}

#endif 
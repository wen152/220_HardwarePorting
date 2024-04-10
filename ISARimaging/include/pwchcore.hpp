//
// File: pwchcore.h
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//
#ifndef PWCHCORE_H
#define PWCHCORE_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.hpp"
#include "omp.h"
#include "Fmain_ISARimaging_types.hpp"

// Function Declarations
extern void b_pwchcore(const double x_data[], const int x_size[2], const creal_T
  y_data[], const creal_T s[2], double pp_breaks_data[], int pp_breaks_size[2],
  creal_T pp_coefs[4]);
extern void c_pwchcore(const double x_data[], const int x_size[2], const creal_T
  y_data[], int yoffset, const creal_T s_data[], const int s_size[2], const
  double dx_data[], const creal_T divdif_data[], double pp_breaks_data[], int
  pp_breaks_size[2], creal_T pp_coefs_data[], int pp_coefs_size[2]);
extern void pwchcore(const double x[1024], const creal_T y[1024], const creal_T
                     s[1024], const double dx[1023], const creal_T divdif[1023],
                     double pp_breaks[1024], creal_T pp_coefs[4092]);

#endif

//
// File trailer for pwchcore.h
//
// [EOF]
//

//
// File: spline.h
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//
#ifndef SPLINE_H
#define SPLINE_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.hpp"
#include "omp.h"
#include "Fmain_ISARimaging_types.hpp"

// Function Declarations
extern void b_spline(const double x_data[], const int x_size[2], const creal_T
                     y_data[], const int y_size[2], double output_breaks_data[],
                     int output_breaks_size[2], creal_T output_coefs_data[], int
                     output_coefs_size[2]);
extern void spline(const creal_T y[1024], double output_breaks[1024], creal_T
                   output_coefs[4092]);

#endif

//
// File trailer for spline.h
//
// [EOF]
//

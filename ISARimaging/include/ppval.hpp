//
// File: ppval.h
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//
#ifndef PPVAL_HPP
#define PPVAL_HPP

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.hpp"
#include "omp.h"
#include "Fmain_ISARimaging_types.hpp"

// Function Declarations
extern creal_T b_ppval(const double pp_breaks_data[], const int pp_breaks_size[2],
  const creal_T pp_coefs_data[], const int pp_coefs_size[2], double x);
extern creal_T ppval(const double pp_breaks[1024], const creal_T pp_coefs[4092],
                     double x);

#endif

//
// File trailer for ppval.h
//
// [EOF]
//

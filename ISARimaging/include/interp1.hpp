//
// File: interp1.hpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//
#ifndef INTERP1_HPP
#define INTERP1_HPP

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.hpp"
#include "omp.h"
#include "Fmain_ISARimaging_types.hpp"

// Function Declarations
extern void b_interp1(const double varargin_1_data[], const int varargin_1_size
                      [2], const creal_T varargin_2_data[], const int
                      varargin_2_size[2], const double varargin_3_data[], const
                      int varargin_3_size[2], creal_T Vq_data[], int Vq_size[2]);
extern void interp1(const creal_T varargin_2[1024], const double varargin_3[1024],
                    creal_T Vq[1024]);

#endif

//
// File trailer for interp1.hpp
//
// [EOF]
//

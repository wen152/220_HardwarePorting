//
// File: Amplitude_limiting.h
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//
#ifndef AMPLITUDE_LIMITING_HPP
#define AMPLITUDE_LIMITING_HPP

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.hpp"
#include "omp.h"
#include "Fmain_ISARimaging_types.hpp"

// Function Declarations
extern void Amplitude_limiting(const creal_T image[30000], double a, double b,
  double image_limit[30000]);

#endif

//
// File trailer for Amplitude_limiting.h
//
// [EOF]
//

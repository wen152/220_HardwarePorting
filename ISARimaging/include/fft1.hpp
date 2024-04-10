//
// File: fft1.h
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//
#ifndef FFT1_HPP
#define FFT1_HPP

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.hpp"
#include "omp.h"
#include "Fmain_ISARimaging_types.hpp"

// Function Declarations
extern void b_r2br_r2dit_trig(const emxArray_creal_T *x, emxArray_creal_T *y);
extern void c_fft(const emxArray_creal_T *x, int n, emxArray_creal_T *y);
extern void r2br_r2dit_trig(const emxArray_creal_T *x, emxArray_creal_T *y);

#endif

//
// File trailer for fft1.h
//
// [EOF]
//

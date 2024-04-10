//
// File: fft.hpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//
#ifndef FFT_HPP
#define FFT_HPP

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.hpp"
#include "omp.h"
#include "Fmain_ISARimaging_types.hpp"

// Function Declarations
extern void b_fft(const emxArray_creal_T *x, emxArray_creal_T *y);
extern void fft(const emxArray_creal_T *x, emxArray_creal_T *y);

#endif

//
// File trailer for fft.hpp
//
// [EOF]
//

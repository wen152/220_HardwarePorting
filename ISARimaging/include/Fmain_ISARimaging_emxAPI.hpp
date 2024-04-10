//
// File: Fmain_ISARimaging_emxAPI.h
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//
#ifndef FMAIN_ISARIMAGING_EMXAPI_HPP
#define FMAIN_ISARIMAGING_EMXAPI_HPP

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.hpp"
#include "omp.h"
#include "Fmain_ISARimaging_types.hpp"

// Function Declarations
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapperND_real_T(double *data, int
  numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols);
extern emxArray_real_T *emxCreate_real_T(int rows, int cols);
extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);
extern void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

//
// File trailer for Fmain_ISARimaging_emxAPI.h
//
// [EOF]
//

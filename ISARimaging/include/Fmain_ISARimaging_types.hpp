//
// File: Fmain_ISARimaging_types.h
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//
#ifndef FMAIN_ISARIMAGING_TYPES_HPP
#define FMAIN_ISARIMAGING_TYPES_HPP

// Include Files
#include "rtwtypes.hpp"

// Type Definitions
typedef struct {
  struct {
    creal_T fv_data[2144];
    creal_T tmp_data[2144];
    creal_T b_fv_data[2144];
  } f0;
} Fmain_ISARimagingTLS;

typedef struct {
  creal_T *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
} emxArray_creal_T;

struct emxArray_int32_T
{
  int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

struct emxArray_real_T
{
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif

//
// File trailer for Fmain_ISARimaging_types.h
//
// [EOF]
//

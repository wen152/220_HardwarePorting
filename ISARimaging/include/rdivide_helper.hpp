//
// File: rdivide_helper.hpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//
#ifndef RDIVIDE_HELPER_HPP
#define RDIVIDE_HELPER_HPP

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.hpp"
#include "omp.h"
#include "Fmain_ISARimaging_types.hpp"

// Function Declarations
extern void b_rdivide_helper(double x, const emxArray_real_T *y, emxArray_real_T
  *z);
extern void c_rdivide_helper(const double x_data[], const int x_size[2], const
  double y_data[], double z_data[], int z_size[2]);
extern void rdivide_helper(const emxArray_real_T *x, const emxArray_real_T *y,
  emxArray_real_T *z);

#endif

//
// File trailer for rdivide_helper.hpp
//
// [EOF]
//

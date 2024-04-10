//
// File: atan.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include <cmath>
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "atan.hpp"

// Function Definitions

//
// Arguments    : emxArray_real_T *x
// Return Type  : void
//
void b_atan(emxArray_real_T *x)
{
  int nx;
  int k;
  nx = x->size[0];
  for (k = 0; k < nx; k++) {
    x->data[k] = std::atan(x->data[k]);
  }
}

//
// File trailer for atan.cpp
//
// [EOF]
//

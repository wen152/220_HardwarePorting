//
// File: sqrt.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include <cmath>
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "sqrt.hpp"

// Function Definitions

//
// Arguments    : emxArray_real_T *x
// Return Type  : void
//
void b_sqrt(emxArray_real_T *x)
{
  int nx;
  int k;
  nx = x->size[0];
  for (k = 0; k < nx; k++) {
    x->data[k] = std::sqrt(x->data[k]);
  }
}

//
// Arguments    : double *x
// Return Type  : void
//
void c_sqrt(double *x)
{
  *x = std::sqrt(*x);
}

//
// File trailer for sqrt.cpp
//
// [EOF]
//

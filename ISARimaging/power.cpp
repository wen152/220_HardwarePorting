//
// File: power.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "power.hpp"
#include "Fmain_ISARimaging_emxutil.hpp"

// Function Definitions

//
// Arguments    : const emxArray_real_T *a
//                emxArray_real_T *y
// Return Type  : void
//
void power(const emxArray_real_T *a, emxArray_real_T *y)
{
  unsigned int a_idx_0;
  int nx;
  int k;
  a_idx_0 = (unsigned int)a->size[0];
  nx = y->size[0];
  y->size[0] = (int)a_idx_0;
  emxEnsureCapacity_real_T(y, nx);
  a_idx_0 = (unsigned int)a->size[0];
  nx = (int)a_idx_0;
  for (k = 0; k < nx; k++) {
    y->data[k] = a->data[k] * a->data[k];
  }
}

//
// File trailer for power.cpp
//
// [EOF]
//

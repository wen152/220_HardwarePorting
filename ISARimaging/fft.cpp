//
// File: fft.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "fft.hpp"
#include "Fmain_ISARimaging_emxutil.hpp"
#include "fft1.hpp"

// Function Definitions

//
// Arguments    : const emxArray_creal_T *x
//                emxArray_creal_T *y
// Return Type  : void
//
void b_fft(const emxArray_creal_T *x, emxArray_creal_T *y)
{
  emxArray_creal_T *b_x;
  int i8;
  emxArray_creal_T *r2;
  int loop_ub;
  int i9;
  emxInit_creal_T(&b_x, 2);
  i8 = b_x->size[0] * b_x->size[1];
  b_x->size[0] = x->size[1];
  b_x->size[1] = 1024;
  emxEnsureCapacity_creal_T(b_x, i8);
  for (i8 = 0; i8 < 1024; i8++) {
    loop_ub = x->size[1];
    for (i9 = 0; i9 < loop_ub; i9++) {
      b_x->data[i9 + b_x->size[0] * i8] = x->data[i8 + (i9 << 10)];
    }
  }

  emxInit_creal_T(&r2, 2);
  c_fft(b_x, x->size[1], r2);
  i8 = y->size[0] * y->size[1];
  y->size[0] = 1024;
  y->size[1] = r2->size[0];
  emxEnsureCapacity_creal_T(y, i8);
  loop_ub = r2->size[0];
  emxFree_creal_T(&b_x);
  for (i8 = 0; i8 < loop_ub; i8++) {
    for (i9 = 0; i9 < 1024; i9++) {
      y->data[i9 + (i8 << 10)] = r2->data[i8 + r2->size[0] * i9];
    }
  }

  emxFree_creal_T(&r2);
}

//
// Arguments    : const emxArray_creal_T *x
//                emxArray_creal_T *y
// Return Type  : void
//
void fft(const emxArray_creal_T *x, emxArray_creal_T *y)
{
  if (x->size[1] == 0) {
    y->size[0] = 1024;
    y->size[1] = 0;
  } else {
    r2br_r2dit_trig(x, y);
  }
}

//
// File trailer for fft.cpp
//
// [EOF]
//

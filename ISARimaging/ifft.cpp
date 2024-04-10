//
// File: ifft.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "ifft.hpp"
#include "fft1.hpp"

// Function Definitions

//
// Arguments    : const emxArray_creal_T *x
//                emxArray_creal_T *y
// Return Type  : void
//
void ifft(const emxArray_creal_T *x, emxArray_creal_T *y)
{
  if (x->size[1] == 0) {
    y->size[0] = 1024;
    y->size[1] = 0;
  } else {
    b_r2br_r2dit_trig(x, y);
  }
}

//
// File trailer for ifft.cpp
//
// [EOF]
//

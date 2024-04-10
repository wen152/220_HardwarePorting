//
// File: fftshift.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "fftshift.hpp"

// Function Definitions

//
// Arguments    : emxArray_creal_T *x
// Return Type  : void
//
void fftshift(emxArray_creal_T *x)
{
  int vlend2;
  int midoffset;
  int i1;
  int j;
  int ia;
  int ib;
  double xtmp_re;
  int k;
  double xtmp_im;
  if (x->size[1] > 1) {
    vlend2 = x->size[1] / 2;
    midoffset = vlend2 << 10;
    if (vlend2 << 1 == x->size[1]) {
      i1 = -1;
      for (j = 0; j < 1024; j++) {
        i1++;
        ia = i1;
        ib = i1 + midoffset;
        for (k = 0; k < vlend2; k++) {
          xtmp_re = x->data[ia].re;
          xtmp_im = x->data[ia].im;
          x->data[ia] = x->data[ib];
          x->data[ib].re = xtmp_re;
          x->data[ib].im = xtmp_im;
          ia += 1024;
          ib += 1024;
        }
      }
    } else {
      i1 = -1;
      for (j = 0; j < 1024; j++) {
        i1++;
        ia = i1;
        ib = i1 + midoffset;
        xtmp_re = x->data[ib].re;
        xtmp_im = x->data[ib].im;
        for (k = 0; k < vlend2; k++) {
          x->data[ib] = x->data[ia];
          x->data[ia] = x->data[ib + 1024];
          ia += 1024;
          ib += 1024;
        }

        x->data[ib].re = xtmp_re;
        x->data[ib].im = xtmp_im;
      }
    }
  }
}

//
// File trailer for fftshift.cpp
//
// [EOF]
//

//
// File: exp.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include <cmath>
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "exp.hpp"

// Function Definitions

//
// Arguments    : creal_T x[1024]
// Return Type  : void
//
void b_exp(creal_T x[1024])
{
  int k;
  double r;
  double x_im;
  for (k = 0; k < 1024; k++) {
    if (x[k].im == 0.0) {
      x[k].re = std::exp(x[k].re);
      x[k].im = 0.0;
    } else if (rtIsInf(x[k].im) && rtIsInf(x[k].re) && (x[k].re < 0.0)) {
      x[k].re = 0.0;
      x[k].im = 0.0;
    } else {
      r = std::exp(x[k].re / 2.0);
      x_im = x[k].im;
      x[k].re = r * (r * std::cos(x[k].im));
      x[k].im = r * (r * std::sin(x_im));
    }
  }
}

//
// File trailer for exp.cpp
//
// [EOF]
//

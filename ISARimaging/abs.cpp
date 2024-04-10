//
// File: abs.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include <cmath>
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "abs.hpp"

// Function Declarations
static double rt_hypotd_snf(double u0, double u1);

// Function Definitions

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_hypotd_snf(double u0, double u1)
{
  double y;
  double a;
  double b;
  a = std::abs(u0);
  b = std::abs(u1);
  if (a < b) {
    a /= b;
    y = b * std::sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * std::sqrt(b * b + 1.0);
  } else if (rtIsNaN(b)) {
    y = b;
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

//
// Arguments    : const double x[1024]
//                double y[1024]
// Return Type  : void
//
void b_abs(const double x[1024], double y[1024])
{
  int k;
  for (k = 0; k < 1024; k++) {
    y[k] = std::abs(x[k]);
  }
}

//
// Arguments    : const creal_T x[30000]
//                double y[30000]
// Return Type  : void
//
void c_abs(const creal_T x[30000], double y[30000])
{
  int k;
  for (k = 0; k < 30000; k++) {
    y[k] = rt_hypotd_snf(x[k].re, x[k].im);
  }
}

//
// File trailer for abs.cpp
//
// [EOF]
//

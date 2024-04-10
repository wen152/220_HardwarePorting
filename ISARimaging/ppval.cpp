//
// File: ppval.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "ppval.hpp"

// Function Definitions

//
// Arguments    : const double pp_breaks_data[]
//                const int pp_breaks_size[2]
//                const creal_T pp_coefs_data[]
//                const int pp_coefs_size[2]
//                double x
// Return Type  : creal_T
//
creal_T b_ppval(const double pp_breaks_data[], const int pp_breaks_size[2],
                const creal_T pp_coefs_data[], const int pp_coefs_size[2],
                double x)
{
  creal_T v;
  int numTerms;
  int high_i;
  int low_i;
  int low_ip1;
  double xloc;
  int mid_i;
  numTerms = pp_coefs_size[1];
  if (rtIsNaN(x)) {
    v.re = x;
    v.im = 0.0;
  } else {
    high_i = pp_breaks_size[1];
    low_i = 1;
    low_ip1 = 2;
    while (high_i > low_ip1) {
      mid_i = (low_i >> 1) + (high_i >> 1);
      if (((low_i & 1) == 1) && ((high_i & 1) == 1)) {
        mid_i++;
      }

      if (x >= pp_breaks_data[mid_i - 1]) {
        low_i = mid_i;
        low_ip1 = mid_i + 1;
      } else {
        high_i = mid_i;
      }
    }

    xloc = x - pp_breaks_data[low_i - 1];
    v = pp_coefs_data[low_i - 1];
    for (high_i = 2; high_i <= numTerms; high_i++) {
      v.re = xloc * v.re + pp_coefs_data[(low_i + (high_i - 1) *
        (pp_breaks_size[1] - 1)) - 1].re;
      v.im = xloc * v.im + pp_coefs_data[(low_i + (high_i - 1) *
        (pp_breaks_size[1] - 1)) - 1].im;
    }
  }

  return v;
}

//
// Arguments    : const double pp_breaks[1024]
//                const creal_T pp_coefs[4092]
//                double x
// Return Type  : creal_T
//
creal_T ppval(const double pp_breaks[1024], const creal_T pp_coefs[4092], double
              x)
{
  creal_T v;
  int low_i;
  int low_ip1;
  int high_i;
  double xloc;
  int mid_i;
  if (rtIsNaN(x)) {
    v.re = x;
    v.im = 0.0;
  } else {
    low_i = 0;
    low_ip1 = 2;
    high_i = 1024;
    while (high_i > low_ip1) {
      mid_i = ((low_i + high_i) + 1) >> 1;
      if (x >= pp_breaks[mid_i - 1]) {
        low_i = mid_i - 1;
        low_ip1 = mid_i + 1;
      } else {
        high_i = mid_i;
      }
    }

    xloc = x - pp_breaks[low_i];
    v.re = xloc * pp_coefs[low_i].re + pp_coefs[low_i + 1023].re;
    v.im = xloc * pp_coefs[low_i].im + pp_coefs[low_i + 1023].im;
    v.re = xloc * v.re + pp_coefs[low_i + 2046].re;
    v.im = xloc * v.im + pp_coefs[low_i + 2046].im;
    v.re = xloc * v.re + pp_coefs[low_i + 3069].re;
    v.im = xloc * v.im + pp_coefs[low_i + 3069].im;
  }

  return v;
}

//
// File trailer for ppval.cpp
//
// [EOF]
//

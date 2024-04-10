//
// File: spline.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include <string.h>
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "spline.hpp"
#include "pwchcore.hpp"

// Function Declarations
static void degenerateSpline(const double x_data[], const int x_size[2], const
  creal_T y_data[], const int y_size[2], double pp_breaks_data[], int
  pp_breaks_size[2], creal_T pp_coefs_data[], int pp_coefs_size[2]);

// Function Definitions

//
// Arguments    : const double x_data[]
//                const int x_size[2]
//                const creal_T y_data[]
//                const int y_size[2]
//                double pp_breaks_data[]
//                int pp_breaks_size[2]
//                creal_T pp_coefs_data[]
//                int pp_coefs_size[2]
// Return Type  : void
//
static void degenerateSpline(const double x_data[], const int x_size[2], const
  creal_T y_data[], const int y_size[2], double pp_breaks_data[], int
  pp_breaks_size[2], creal_T pp_coefs_data[], int pp_coefs_size[2])
{
  boolean_T has_endslopes;
  double d21_tmp;
  double ar_tmp;
  double ai_tmp;
  creal_T endslopes[2];
  double dvdf1_re;
  double dvdf1_im;
  double br;
  double expl_temp_data[536];
  int expl_temp_size[2];
  creal_T pp1_coefs[4];
  int pp_coefs_idx_1;
  has_endslopes = (y_size[1] == x_size[1] + 2);
  pp_coefs_size[0] = 1;
  if (x_size[1] <= 2) {
    if (has_endslopes) {
      pp_coefs_size[1] = 4;
    } else {
      pp_coefs_size[1] = 2;
    }

    if (has_endslopes) {
      endslopes[0] = y_data[0];
      endslopes[1] = y_data[y_size[1] - 1];
      b_pwchcore(x_data, x_size, y_data, endslopes, expl_temp_data,
                 expl_temp_size, pp1_coefs);
      pp_coefs_idx_1 = pp_coefs_size[1];
      pp_coefs_size[0] = 1;
      memcpy(&pp_coefs_data[0], &pp1_coefs[0], (unsigned int)(pp_coefs_idx_1 *
              (int)sizeof(creal_T)));
    } else {
      ai_tmp = y_data[1].re - y_data[0].re;
      ar_tmp = y_data[1].im - y_data[0].im;
      br = x_data[1] - x_data[0];
      if (ar_tmp == 0.0) {
        pp_coefs_data[0].re = ai_tmp / br;
        pp_coefs_data[0].im = 0.0;
      } else if (ai_tmp == 0.0) {
        pp_coefs_data[0].re = 0.0;
        pp_coefs_data[0].im = ar_tmp / br;
      } else {
        pp_coefs_data[0].re = ai_tmp / br;
        pp_coefs_data[0].im = ar_tmp / br;
      }

      pp_coefs_data[1] = y_data[0];
    }

    pp_breaks_size[0] = 1;
    pp_breaks_size[1] = x_size[1];
    if (0 <= x_size[1] - 1) {
      memcpy(&pp_breaks_data[0], &x_data[0], (unsigned int)(x_size[1] * (int)
              sizeof(double)));
    }
  } else {
    pp_coefs_size[1] = 3;
    d21_tmp = x_data[1] - x_data[0];
    ar_tmp = y_data[1].re - y_data[0].re;
    ai_tmp = y_data[1].im - y_data[0].im;
    if (ai_tmp == 0.0) {
      dvdf1_re = ar_tmp / d21_tmp;
      dvdf1_im = 0.0;
    } else if (ar_tmp == 0.0) {
      dvdf1_re = 0.0;
      dvdf1_im = ai_tmp / d21_tmp;
    } else {
      dvdf1_re = ar_tmp / d21_tmp;
      dvdf1_im = ai_tmp / d21_tmp;
    }

    ai_tmp = y_data[2].re - y_data[1].re;
    ar_tmp = y_data[2].im - y_data[1].im;
    br = x_data[2] - x_data[1];
    if (ar_tmp == 0.0) {
      ai_tmp /= br;
      ar_tmp = 0.0;
    } else if (ai_tmp == 0.0) {
      ai_tmp = 0.0;
      ar_tmp /= br;
    } else {
      ai_tmp /= br;
      ar_tmp /= br;
    }

    ai_tmp -= dvdf1_re;
    ar_tmp -= dvdf1_im;
    br = x_data[2] - x_data[0];
    if (ar_tmp == 0.0) {
      pp_coefs_data[0].re = ai_tmp / br;
      pp_coefs_data[0].im = 0.0;
    } else if (ai_tmp == 0.0) {
      pp_coefs_data[0].re = 0.0;
      pp_coefs_data[0].im = ar_tmp / br;
    } else {
      pp_coefs_data[0].re = ai_tmp / br;
      pp_coefs_data[0].im = ar_tmp / br;
    }

    pp_coefs_data[1].re = dvdf1_re - pp_coefs_data[0].re * d21_tmp;
    pp_coefs_data[1].im = dvdf1_im - pp_coefs_data[0].im * d21_tmp;
    pp_coefs_data[2] = y_data[0];
    pp_breaks_size[0] = 1;
    pp_breaks_size[1] = 2;
    pp_breaks_data[0] = x_data[0];
    pp_breaks_data[1] = x_data[2];
  }
}

//
// Arguments    : const double x_data[]
//                const int x_size[2]
//                const creal_T y_data[]
//                const int y_size[2]
//                double output_breaks_data[]
//                int output_breaks_size[2]
//                creal_T output_coefs_data[]
//                int output_coefs_size[2]
// Return Type  : void
//
void b_spline(const double x_data[], const int x_size[2], const creal_T y_data[],
              const int y_size[2], double output_breaks_data[], int
              output_breaks_size[2], creal_T output_coefs_data[], int
              output_coefs_size[2])
{
  boolean_T has_endslopes;
  int nxm1;
  creal_T t1_coefs_data[4];
  int s_size[2];
  short szs_idx_1;
  int yoffset;
  int k;
  double dnnm2;
  double dx_data[535];
  double r;
  int ar_tmp;
  double d31;
  double ai;
  double ar;
  creal_T s_data[536];
  creal_T dvdf_data[535];
  double md_data[536];
  static creal_T t2_coefs_data[2140];
  int t2_coefs_size[2];
  has_endslopes = (y_size[1] == x_size[1] + 2);
  if ((x_size[1] <= 2) || ((x_size[1] <= 3) && (!has_endslopes))) {
    degenerateSpline(x_data, x_size, y_data, y_size, output_breaks_data,
                     output_breaks_size, t1_coefs_data, s_size);
    output_coefs_size[0] = 1;
    output_coefs_size[1] = s_size[1];
    nxm1 = s_size[0] * s_size[1];
    if (0 <= nxm1 - 1) {
      memcpy(&output_coefs_data[0], &t1_coefs_data[0], (unsigned int)(nxm1 *
              (int)sizeof(creal_T)));
    }
  } else {
    nxm1 = x_size[1] - 1;
    if (has_endslopes) {
      szs_idx_1 = (short)(y_size[1] - 2);
      yoffset = 1;
    } else {
      szs_idx_1 = (short)y_size[1];
      yoffset = 0;
    }

    s_size[0] = 1;
    s_size[1] = szs_idx_1;
    for (k = 0; k < nxm1; k++) {
      dnnm2 = x_data[k + 1] - x_data[k];
      dx_data[k] = dnnm2;
      ar_tmp = yoffset + k;
      ar = y_data[ar_tmp + 1].re - y_data[ar_tmp].re;
      ai = y_data[(yoffset + k) + 1].im - y_data[yoffset + k].im;
      if (ai == 0.0) {
        dvdf_data[k].re = ar / dnnm2;
        dvdf_data[k].im = 0.0;
      } else if (ar == 0.0) {
        dvdf_data[k].re = 0.0;
        dvdf_data[k].im = ai / dnnm2;
      } else {
        dvdf_data[k].re = ar / dnnm2;
        dvdf_data[k].im = ai / dnnm2;
      }
    }

    for (k = 2; k <= nxm1; k++) {
      r = dx_data[k - 1];
      ai = dx_data[k - 2];
      s_data[k - 1].re = 3.0 * (r * dvdf_data[k - 2].re + ai * dvdf_data[k - 1].
        re);
      s_data[k - 1].im = 3.0 * (r * dvdf_data[k - 2].im + ai * dvdf_data[k - 1].
        im);
    }

    if (has_endslopes) {
      d31 = 0.0;
      dnnm2 = 0.0;
      s_data[0].re = dx_data[1] * y_data[0].re;
      s_data[0].im = dx_data[1] * y_data[0].im;
      s_data[x_size[1] - 1].re = dx_data[x_size[1] - 3] * y_data[x_size[1] + 1].
        re;
      s_data[x_size[1] - 1].im = dx_data[x_size[1] - 3] * y_data[x_size[1] + 1].
        im;
    } else {
      d31 = x_data[2] - x_data[0];
      dnnm2 = x_data[x_size[1] - 1] - x_data[x_size[1] - 3];
      r = (dx_data[0] + 2.0 * d31) * dx_data[1];
      ai = dx_data[0] * dx_data[0];
      ar = r * dvdf_data[0].re + ai * dvdf_data[1].re;
      ai = r * dvdf_data[0].im + ai * dvdf_data[1].im;
      if (ai == 0.0) {
        s_data[0].re = ar / d31;
        s_data[0].im = 0.0;
      } else if (ar == 0.0) {
        s_data[0].re = 0.0;
        s_data[0].im = ai / d31;
      } else {
        s_data[0].re = ar / d31;
        s_data[0].im = ai / d31;
      }

      r = dx_data[x_size[1] - 2];
      ai = (r + 2.0 * dnnm2) * dx_data[x_size[1] - 3];
      r *= r;
      ar = ai * dvdf_data[x_size[1] - 2].re + r * dvdf_data[x_size[1] - 3].re;
      ai = ai * dvdf_data[x_size[1] - 2].im + r * dvdf_data[x_size[1] - 3].im;
      if (ai == 0.0) {
        s_data[x_size[1] - 1].re = ar / dnnm2;
        s_data[x_size[1] - 1].im = 0.0;
      } else if (ar == 0.0) {
        s_data[x_size[1] - 1].re = 0.0;
        s_data[x_size[1] - 1].im = ai / dnnm2;
      } else {
        s_data[x_size[1] - 1].re = ar / dnnm2;
        s_data[x_size[1] - 1].im = ai / dnnm2;
      }
    }

    md_data[0] = dx_data[1];
    md_data[x_size[1] - 1] = dx_data[x_size[1] - 3];
    for (k = 2; k <= nxm1; k++) {
      md_data[k - 1] = 2.0 * (dx_data[k - 1] + dx_data[k - 2]);
    }

    r = dx_data[1] / md_data[0];
    md_data[1] -= r * d31;
    s_data[1].re -= r * s_data[0].re;
    s_data[1].im -= r * s_data[0].im;
    for (k = 3; k <= nxm1; k++) {
      r = dx_data[k - 1] / md_data[k - 2];
      md_data[k - 1] -= r * dx_data[k - 3];
      s_data[k - 1].re -= r * s_data[k - 2].re;
      s_data[k - 1].im -= r * s_data[k - 2].im;
    }

    r = dnnm2 / md_data[x_size[1] - 2];
    md_data[x_size[1] - 1] -= r * dx_data[x_size[1] - 3];
    s_data[x_size[1] - 1].re -= r * s_data[x_size[1] - 2].re;
    s_data[x_size[1] - 1].im -= r * s_data[x_size[1] - 2].im;
    r = s_data[x_size[1] - 1].re;
    ai = s_data[x_size[1] - 1].im;
    dnnm2 = md_data[x_size[1] - 1];
    if (ai == 0.0) {
      s_data[x_size[1] - 1].re = r / dnnm2;
      s_data[x_size[1] - 1].im = 0.0;
    } else if (r == 0.0) {
      s_data[x_size[1] - 1].re = 0.0;
      s_data[x_size[1] - 1].im = ai / dnnm2;
    } else {
      s_data[x_size[1] - 1].re = r / dnnm2;
      s_data[x_size[1] - 1].im = ai / dnnm2;
    }

    for (k = nxm1; k >= 2; k--) {
      ai = dx_data[k - 2];
      r = s_data[k - 1].re - ai * s_data[k].re;
      ai = s_data[k - 1].im - ai * s_data[k].im;
      dnnm2 = md_data[k - 1];
      if (ai == 0.0) {
        s_data[k - 1].re = r / dnnm2;
        s_data[k - 1].im = 0.0;
      } else if (r == 0.0) {
        s_data[k - 1].re = 0.0;
        s_data[k - 1].im = ai / dnnm2;
      } else {
        s_data[k - 1].re = r / dnnm2;
        s_data[k - 1].im = ai / dnnm2;
      }
    }

    r = s_data[0].re - d31 * s_data[1].re;
    ai = s_data[0].im - d31 * s_data[1].im;
    if (ai == 0.0) {
      s_data[0].re = r / md_data[0];
      s_data[0].im = 0.0;
    } else if (r == 0.0) {
      s_data[0].re = 0.0;
      s_data[0].im = ai / md_data[0];
    } else {
      s_data[0].re = r / md_data[0];
      s_data[0].im = ai / md_data[0];
    }

    c_pwchcore(x_data, x_size, y_data, yoffset, s_data, s_size, dx_data,
               dvdf_data, output_breaks_data, output_breaks_size, t2_coefs_data,
               t2_coefs_size);
    output_coefs_size[0] = t2_coefs_size[0];
    output_coefs_size[1] = 4;
    nxm1 = t2_coefs_size[0] * t2_coefs_size[1];
    if (0 <= nxm1 - 1) {
      memcpy(&output_coefs_data[0], &t2_coefs_data[0], (unsigned int)(nxm1 *
              (int)sizeof(creal_T)));
    }
  }
}

//
// Arguments    : const creal_T y[1024]
//                double output_breaks[1024]
//                creal_T output_coefs[4092]
// Return Type  : void
//
void spline(const creal_T y[1024], double output_breaks[1024], creal_T
            output_coefs[4092])
{
  int k;
  double r;
  creal_T dvdf[1023];
  double dx[1023];
  double ai;
  creal_T s[1024];
  double md[1024];
  double br;
  for (k = 0; k < 1023; k++) {
    dx[k] = 1.0;
    r = y[k + 1].re - y[k].re;
    ai = y[k + 1].im - y[k].im;
    if (ai == 0.0) {
      dvdf[k].re = r;
      dvdf[k].im = 0.0;
    } else if (r == 0.0) {
      dvdf[k].re = 0.0;
      dvdf[k].im = ai;
    } else {
      dvdf[k].re = r;
      dvdf[k].im = ai;
    }
  }

  r = 5.0 * dvdf[0].re + dvdf[1].re;
  ai = 5.0 * dvdf[0].im + dvdf[1].im;
  if (ai == 0.0) {
    s[0].re = r / 2.0;
    s[0].im = 0.0;
  } else if (r == 0.0) {
    s[0].re = 0.0;
    s[0].im = ai / 2.0;
  } else {
    s[0].re = r / 2.0;
    s[0].im = ai / 2.0;
  }

  r = 5.0 * dvdf[1022].re + dvdf[1021].re;
  ai = 5.0 * dvdf[1022].im + dvdf[1021].im;
  if (ai == 0.0) {
    s[1023].re = r / 2.0;
    s[1023].im = 0.0;
  } else if (r == 0.0) {
    s[1023].re = 0.0;
    s[1023].im = ai / 2.0;
  } else {
    s[1023].re = r / 2.0;
    s[1023].im = ai / 2.0;
  }

  md[0] = 1.0;
  md[1023] = 1.0;
  for (k = 0; k < 1022; k++) {
    s[k + 1].re = 3.0 * (dvdf[k].re + dvdf[k + 1].re);
    s[k + 1].im = 3.0 * (dvdf[k].im + dvdf[k + 1].im);
    md[k + 1] = 4.0;
  }

  r = 1.0 / md[0];
  md[1] -= r * 2.0;
  s[1].re -= r * s[0].re;
  s[1].im -= r * s[0].im;
  for (k = 0; k < 1021; k++) {
    r = 1.0 / md[k + 1];
    md[k + 2] -= r;
    s[k + 2].re -= r * s[k + 1].re;
    s[k + 2].im -= r * s[k + 1].im;
  }

  r = 2.0 / md[1022];
  md[1023] -= r;
  s[1023].re -= r * s[1022].re;
  s[1023].im -= r * s[1022].im;
  if (s[1023].im == 0.0) {
    s[1023].re /= md[1023];
    s[1023].im = 0.0;
  } else if (s[1023].re == 0.0) {
    s[1023].re = 0.0;
    s[1023].im /= md[1023];
  } else {
    s[1023].re /= md[1023];
    s[1023].im /= md[1023];
  }

  for (k = 1021; k >= 0; k--) {
    r = s[k + 1].re - s[k + 2].re;
    ai = s[k + 1].im - s[k + 2].im;
    br = md[k + 1];
    if (ai == 0.0) {
      s[k + 1].re = r / br;
      s[k + 1].im = 0.0;
    } else if (r == 0.0) {
      s[k + 1].re = 0.0;
      s[k + 1].im = ai / br;
    } else {
      s[k + 1].re = r / br;
      s[k + 1].im = ai / br;
    }
  }

  r = s[0].re - 2.0 * s[1].re;
  ai = s[0].im - 2.0 * s[1].im;
  if (ai == 0.0) {
    s[0].re = r / md[0];
    s[0].im = 0.0;
  } else if (r == 0.0) {
    s[0].re = 0.0;
    s[0].im = ai / md[0];
  } else {
    s[0].re = r / md[0];
    s[0].im = ai / md[0];
  }

  for (k = 0; k < 1024; k++) {
    md[k] = 1.0 + (double)k;
  }

  pwchcore(md, y, s, dx, dvdf, output_breaks, output_coefs);
}

//
// File trailer for spline.cpp
//
// [EOF]
//

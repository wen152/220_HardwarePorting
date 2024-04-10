//
// File: pwchcore.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include <string.h>
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "pwchcore.hpp"

// Function Definitions

//
// Arguments    : const double x_data[]
//                const int x_size[2]
//                const creal_T y_data[]
//                const creal_T s[2]
//                double pp_breaks_data[]
//                int pp_breaks_size[2]
//                creal_T pp_coefs[4]
// Return Type  : void
//
void b_pwchcore(const double x_data[], const int x_size[2], const creal_T
                y_data[], const creal_T s[2], double pp_breaks_data[], int
                pp_breaks_size[2], creal_T pp_coefs[4])
{
  int nxm1;
  int j;
  double dxj;
  creal_T ar_tmp;
  double ar;
  double ai;
  double divdifij_re;
  double divdifij_im;
  double dzzdx_re;
  double dzzdx_im;
  nxm1 = x_size[1];
  pp_breaks_size[0] = 1;
  pp_breaks_size[1] = x_size[1];
  if (0 <= x_size[1] - 1) {
    memcpy(&pp_breaks_data[0], &x_data[0], (unsigned int)(x_size[1] * (int)
            sizeof(double)));
  }

  for (j = 0; j <= nxm1 - 2; j++) {
    dxj = x_data[j + 1] - x_data[j];
    ar_tmp = y_data[j + 1];
    ar = y_data[j + 2].re - ar_tmp.re;
    ai = y_data[j + 2].im - y_data[j + 1].im;
    if (ai == 0.0) {
      divdifij_re = ar / dxj;
      divdifij_im = 0.0;
    } else if (ar == 0.0) {
      divdifij_re = 0.0;
      divdifij_im = ai / dxj;
    } else {
      divdifij_re = ar / dxj;
      divdifij_im = ai / dxj;
    }

    ar = divdifij_re - s[j].re;
    ai = divdifij_im - s[j].im;
    if (ai == 0.0) {
      dzzdx_re = ar / dxj;
      dzzdx_im = 0.0;
    } else if (ar == 0.0) {
      dzzdx_re = 0.0;
      dzzdx_im = ai / dxj;
    } else {
      dzzdx_re = ar / dxj;
      dzzdx_im = ai / dxj;
    }

    divdifij_re = s[1].re - divdifij_re;
    divdifij_im = s[1].im - divdifij_im;
    if (divdifij_im == 0.0) {
      divdifij_re /= dxj;
      divdifij_im = 0.0;
    } else if (divdifij_re == 0.0) {
      divdifij_re = 0.0;
      divdifij_im /= dxj;
    } else {
      divdifij_re /= dxj;
      divdifij_im /= dxj;
    }

    ar = divdifij_re - dzzdx_re;
    ai = divdifij_im - dzzdx_im;
    if (ai == 0.0) {
      pp_coefs[j].re = ar / dxj;
      pp_coefs[j].im = 0.0;
    } else if (ar == 0.0) {
      pp_coefs[j].re = 0.0;
      pp_coefs[j].im = ai / dxj;
    } else {
      pp_coefs[j].re = ar / dxj;
      pp_coefs[j].im = ai / dxj;
    }

    pp_coefs[j + 1].re = 2.0 * dzzdx_re - divdifij_re;
    pp_coefs[j + 1].im = 2.0 * dzzdx_im - divdifij_im;
    pp_coefs[j + 2] = s[j];
    pp_coefs[3] = ar_tmp;
  }
}

//
// Arguments    : const double x_data[]
//                const int x_size[2]
//                const creal_T y_data[]
//                int yoffset
//                const creal_T s_data[]
//                const int s_size[2]
//                const double dx_data[]
//                const creal_T divdif_data[]
//                double pp_breaks_data[]
//                int pp_breaks_size[2]
//                creal_T pp_coefs_data[]
//                int pp_coefs_size[2]
// Return Type  : void
//
void c_pwchcore(const double x_data[], const int x_size[2], const creal_T
                y_data[], int yoffset, const creal_T s_data[], const int s_size
                [2], const double dx_data[], const creal_T divdif_data[], double
                pp_breaks_data[], int pp_breaks_size[2], creal_T pp_coefs_data[],
                int pp_coefs_size[2])
{
  int nxm1;
  int cpage;
  int j;
  double ar;
  double ai;
  double dzzdx_re;
  double dzzdx_im;
  double dzdxdx_re;
  double dzdxdx_im;
  int i7;
  nxm1 = x_size[1];
  pp_breaks_size[0] = 1;
  pp_breaks_size[1] = x_size[1];
  if (0 <= x_size[1] - 1) {
    memcpy(&pp_breaks_data[0], &x_data[0], (unsigned int)(x_size[1] * (int)
            sizeof(double)));
  }

  cpage = s_size[1] - 1;
  pp_coefs_size[0] = s_size[1] - 1;
  pp_coefs_size[1] = 4;
  for (j = 0; j <= nxm1 - 2; j++) {
    ar = divdif_data[j].re - s_data[j].re;
    ai = divdif_data[j].im - s_data[j].im;
    if (ai == 0.0) {
      dzzdx_re = ar / dx_data[j];
      dzzdx_im = 0.0;
    } else if (ar == 0.0) {
      dzzdx_re = 0.0;
      dzzdx_im = ai / dx_data[j];
    } else {
      dzzdx_re = ar / dx_data[j];
      dzzdx_im = ai / dx_data[j];
    }

    ar = s_data[j + 1].re - divdif_data[j].re;
    ai = s_data[j + 1].im - divdif_data[j].im;
    if (ai == 0.0) {
      dzdxdx_re = ar / dx_data[j];
      dzdxdx_im = 0.0;
    } else if (ar == 0.0) {
      dzdxdx_re = 0.0;
      dzdxdx_im = ai / dx_data[j];
    } else {
      dzdxdx_re = ar / dx_data[j];
      dzdxdx_im = ai / dx_data[j];
    }

    ar = dzdxdx_re - dzzdx_re;
    ai = dzdxdx_im - dzzdx_im;
    if (ai == 0.0) {
      pp_coefs_data[j].re = ar / dx_data[j];
      pp_coefs_data[j].im = 0.0;
    } else if (ar == 0.0) {
      pp_coefs_data[j].re = 0.0;
      pp_coefs_data[j].im = ai / dx_data[j];
    } else {
      pp_coefs_data[j].re = ar / dx_data[j];
      pp_coefs_data[j].im = ai / dx_data[j];
    }

    i7 = cpage + j;
    pp_coefs_data[i7].re = 2.0 * dzzdx_re - dzdxdx_re;
    pp_coefs_data[i7].im = 2.0 * dzzdx_im - dzdxdx_im;
    pp_coefs_data[(cpage << 1) + j] = s_data[j];
    pp_coefs_data[3 * cpage + j] = y_data[yoffset + j];
  }
}

//
// Arguments    : const double x[1024]
//                const creal_T y[1024]
//                const creal_T s[1024]
//                const double dx[1023]
//                const creal_T divdif[1023]
//                double pp_breaks[1024]
//                creal_T pp_coefs[4092]
// Return Type  : void
//
void pwchcore(const double x[1024], const creal_T y[1024], const creal_T s[1024],
              const double dx[1023], const creal_T divdif[1023], double
              pp_breaks[1024], creal_T pp_coefs[4092])
{
  int j;
  double ar;
  double ai;
  double dzzdx_re;
  double dzzdx_im;
  double dzdxdx_re;
  double dzdxdx_im;
  memcpy(&pp_breaks[0], &x[0], sizeof(double) << 10);
  for (j = 0; j < 1023; j++) {
    ar = divdif[j].re - s[j].re;
    ai = divdif[j].im - s[j].im;
    if (ai == 0.0) {
      dzzdx_re = ar / dx[j];
      dzzdx_im = 0.0;
    } else if (ar == 0.0) {
      dzzdx_re = 0.0;
      dzzdx_im = ai / dx[j];
    } else {
      dzzdx_re = ar / dx[j];
      dzzdx_im = ai / dx[j];
    }

    ar = s[j + 1].re - divdif[j].re;
    ai = s[j + 1].im - divdif[j].im;
    if (ai == 0.0) {
      dzdxdx_re = ar / dx[j];
      dzdxdx_im = 0.0;
    } else if (ar == 0.0) {
      dzdxdx_re = 0.0;
      dzdxdx_im = ai / dx[j];
    } else {
      dzdxdx_re = ar / dx[j];
      dzdxdx_im = ai / dx[j];
    }

    ar = dzdxdx_re - dzzdx_re;
    ai = dzdxdx_im - dzzdx_im;
    if (ai == 0.0) {
      pp_coefs[j].re = ar / dx[j];
      pp_coefs[j].im = 0.0;
    } else if (ar == 0.0) {
      pp_coefs[j].re = 0.0;
      pp_coefs[j].im = ai / dx[j];
    } else {
      pp_coefs[j].re = ar / dx[j];
      pp_coefs[j].im = ai / dx[j];
    }

    pp_coefs[j + 1023].re = 2.0 * dzzdx_re - dzdxdx_re;
    pp_coefs[j + 1023].im = 2.0 * dzzdx_im - dzdxdx_im;
    pp_coefs[j + 2046] = s[j];
    pp_coefs[j + 3069] = y[j];
  }
}

//
// File trailer for pwchcore.cpp
//
// [EOF]
//

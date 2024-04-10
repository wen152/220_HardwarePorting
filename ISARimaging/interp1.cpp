//
// File: interp1.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include <string.h>
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "interp1.hpp"
#include "ppval.hpp"
#include "spline.hpp"

// Type Definitions
typedef struct {
  creal_T data[2140];
  int size[2];
} emxArray_creal_T_535x4;

struct emxArray_real_T_1x536
{
  double data[536];
  int size[2];
};

struct sOrV6PiDhJZhAs88pZcXHhG_tag
{
  emxArray_real_T_1x536 breaks;
  emxArray_creal_T_535x4 coefs;
};

typedef sOrV6PiDhJZhAs88pZcXHhG_tag b_struct_T;
typedef struct {
  double breaks[1024];
  creal_T coefs[4092];
} struct_T;

// Function Declarations
static void b_interp1SplineOrPCHIP(const creal_T y_data[], const int y_size[2],
  const double xi_data[], const int xi_size[2], creal_T yi_data[], const double
  x_data[], const int x_size[2]);
static void interp1SplineOrPCHIP(const creal_T y[1024], const double xi[1024],
  creal_T yi[1024]);

// Function Definitions

//
// Arguments    : const creal_T y_data[]
//                const int y_size[2]
//                const double xi_data[]
//                const int xi_size[2]
//                creal_T yi_data[]
//                const double x_data[]
//                const int x_size[2]
// Return Type  : void
//
static void b_interp1SplineOrPCHIP(const creal_T y_data[], const int y_size[2],
  const double xi_data[], const int xi_size[2], creal_T yi_data[], const double
  x_data[], const int x_size[2])
{
  int b_y_size[2];
  creal_T b_y_data[536];
  static b_struct_T pp;
  double x1;
  double xend;
  int ub_loop;
  int k;
  b_y_size[0] = 1;
  b_y_size[1] = y_size[1];
  if (0 <= y_size[1] - 1) {
    memcpy(&b_y_data[0], &y_data[0], (unsigned int)(y_size[1] * (int)sizeof
            (creal_T)));
  }

  b_spline(x_data, x_size, b_y_data, b_y_size, pp.breaks.data, pp.breaks.size,
           pp.coefs.data, pp.coefs.size);
  x1 = x_data[0];
  xend = x_data[x_size[1] - 1];
  ub_loop = xi_size[1] - 1;

#pragma omp parallel for \
 num_threads(omp_get_max_threads())

  for (k = 0; k <= ub_loop; k++) {
    if (rtIsNaN(xi_data[k])) {
      yi_data[k].re = rtNaN;
      yi_data[k].im = 0.0;
    } else {
      if ((xi_data[k] >= x1) && (xi_data[k] <= xend)) {
        yi_data[k] = b_ppval(pp.breaks.data, pp.breaks.size, pp.coefs.data,
                             pp.coefs.size, xi_data[k]);
      }
    }
  }
}

//
// Arguments    : const creal_T y[1024]
//                const double xi[1024]
//                creal_T yi[1024]
// Return Type  : void
//
static void interp1SplineOrPCHIP(const creal_T y[1024], const double xi[1024],
  creal_T yi[1024])
{
  static struct_T pp;
  int k;
  spline(y, pp.breaks, pp.coefs);

#pragma omp parallel for \
 num_threads(omp_get_max_threads())

  for (k = 0; k < 1024; k++) {
    if (rtIsNaN(xi[k])) {
      yi[k].re = rtNaN;
      yi[k].im = 0.0;
    } else {
      if ((xi[k] >= 1.0) && (xi[k] <= 1024.0)) {
        yi[k] = ppval(pp.breaks, pp.coefs, xi[k]);
      }
    }
  }
}

//
// Arguments    : const double varargin_1_data[]
//                const int varargin_1_size[2]
//                const creal_T varargin_2_data[]
//                const int varargin_2_size[2]
//                const double varargin_3_data[]
//                const int varargin_3_size[2]
//                creal_T Vq_data[]
//                int Vq_size[2]
// Return Type  : void
//
void b_interp1(const double varargin_1_data[], const int varargin_1_size[2],
               const creal_T varargin_2_data[], const int varargin_2_size[2],
               const double varargin_3_data[], const int varargin_3_size[2],
               creal_T Vq_data[], int Vq_size[2])
{
  int y_size[2];
  int nd2;
  creal_T y_data[536];
  int x_size[2];
  double x_data[536];
  int nx;
  short outsize_idx_1;
  int j2;
  int exitg1;
  int b_j1;
  double xtmp;
  double xtmp_im;
  y_size[0] = 1;
  y_size[1] = varargin_2_size[1];
  nd2 = varargin_2_size[0] * varargin_2_size[1];
  if (0 <= nd2 - 1) {
    memcpy(&y_data[0], &varargin_2_data[0], (unsigned int)(nd2 * (int)sizeof
            (creal_T)));
  }

  x_size[0] = 1;
  x_size[1] = varargin_1_size[1];
  nd2 = varargin_1_size[0] * varargin_1_size[1];
  if (0 <= nd2 - 1) {
    memcpy(&x_data[0], &varargin_1_data[0], (unsigned int)(nd2 * (int)sizeof
            (double)));
  }

  nx = varargin_1_size[1] - 1;
  outsize_idx_1 = (short)varargin_3_size[1];
  Vq_size[0] = 1;
  Vq_size[1] = outsize_idx_1;
  nd2 = outsize_idx_1;
  for (j2 = 0; j2 < nd2; j2++) {
    Vq_data[j2].re = 0.0;
    Vq_data[j2].im = 0.0;
  }

  if (varargin_3_size[1] != 0) {
    nd2 = 0;
    do {
      exitg1 = 0;
      if (nd2 <= nx) {
        if (rtIsNaN(varargin_1_data[nd2])) {
          exitg1 = 1;
        } else {
          nd2++;
        }
      } else {
        if (varargin_1_data[1] < varargin_1_data[0]) {
          j2 = (nx + 1) >> 1;
          for (b_j1 = 0; b_j1 < j2; b_j1++) {
            xtmp = x_data[b_j1];
            nd2 = nx - b_j1;
            x_data[b_j1] = x_data[nd2];
            x_data[nd2] = xtmp;
          }

          nd2 = varargin_2_size[1] >> 1;
          for (b_j1 = 0; b_j1 < nd2; b_j1++) {
            j2 = (varargin_2_size[1] - b_j1) - 1;
            xtmp = y_data[b_j1].re;
            xtmp_im = y_data[b_j1].im;
            y_data[b_j1] = y_data[j2];
            y_data[j2].re = xtmp;
            y_data[j2].im = xtmp_im;
          }
        }

        b_interp1SplineOrPCHIP(y_data, y_size, varargin_3_data, varargin_3_size,
          Vq_data, x_data, x_size);
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
}

//
// Arguments    : const creal_T varargin_2[1024]
//                const double varargin_3[1024]
//                creal_T Vq[1024]
// Return Type  : void
//
void interp1(const creal_T varargin_2[1024], const double varargin_3[1024],
             creal_T Vq[1024])
{
  memset(&Vq[0], 0, sizeof(creal_T) << 10);
  interp1SplineOrPCHIP(varargin_2, varargin_3, Vq);
}

//
// File trailer for interp1.cpp
//
// [EOF]
//

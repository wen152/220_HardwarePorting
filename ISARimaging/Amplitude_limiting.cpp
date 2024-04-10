//
// File: Amplitude_limiting.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include <string.h>
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "Amplitude_limiting.hpp"
#include "Fmain_ISARimaging_emxutil.hpp"
#include "abs.hpp"

// Function Definitions

//
// %%函数对输入图像限幅,a为限幅最低限，b为限幅最高限
// Arguments    : const creal_T image[30000]
//                double a
//                double b
//                double image_limit[30000]
// Return Type  : void
//
void Amplitude_limiting(const creal_T image[30000], double a, double b, double
  image_limit[30000])
{
  static double normalization_image[30000];
  int jj;
  double maxval[150];
  int idx;
  int i;
  boolean_T exitg1;
  double d0;
  double max_image;
  emxArray_int32_T *b_i;
  int ii;
  boolean_T guard1 = false;
  unsigned char j_data[30000];
  memset(&image_limit[0], 0, 30000U * sizeof(double));
  c_abs(image, normalization_image);
  for (jj = 0; jj < 150; jj++) {
    maxval[jj] = normalization_image[200 * jj];
    for (i = 0; i < 199; i++) {
      d0 = normalization_image[(i + 200 * jj) + 1];
      if ((!rtIsNaN(d0)) && (rtIsNaN(maxval[jj]) || (maxval[jj] < d0))) {
        maxval[jj] = d0;
      }
    }
  }

  if (!rtIsNaN(maxval[0])) {
    idx = 1;
  } else {
    idx = 0;
    jj = 2;
    exitg1 = false;
    while ((!exitg1) && (jj < 151)) {
      if (!rtIsNaN(maxval[jj - 1])) {
        idx = jj;
        exitg1 = true;
      } else {
        jj++;
      }
    }
  }

  if (idx == 0) {
    max_image = maxval[0];
  } else {
    max_image = maxval[idx - 1];
    i = idx + 1;
    for (jj = i; jj < 151; jj++) {
      d0 = maxval[jj - 1];
      if (max_image < d0) {
        max_image = d0;
      }
    }
  }

  a /= max_image;
  b /= max_image;
  for (i = 0; i < 30000; i++) {
    normalization_image[i] /= max_image;
  }

  emxInit_int32_T(&b_i, 1);

  // %%中间层
  idx = 0;
  i = b_i->size[0];
  b_i->size[0] = 30000;
  emxEnsureCapacity_int32_T(b_i, i);
  ii = 0;
  jj = 0;
  exitg1 = false;
  while ((!exitg1) && (jj + 1 <= 150)) {
    d0 = normalization_image[ii + 200 * jj];
    guard1 = false;
    if ((d0 >= a) && (d0 <= b)) {
      idx++;
      b_i->data[idx - 1] = ii + 1;
      j_data[idx - 1] = (unsigned char)(jj + 1);
      if (idx >= 30000) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      ii++;
      if (ii + 1 > 200) {
        ii = 0;
        jj++;
      }
    }
  }

  if (1 > idx) {
    b_i->size[0] = 0;
  } else {
    i = b_i->size[0];
    b_i->size[0] = idx;
    emxEnsureCapacity_int32_T(b_i, i);
  }

  i = b_i->size[0];
  for (ii = 0; ii < i; ii++) {
    image_limit[(b_i->data[ii] + 200 * (j_data[ii] - 1)) - 1] =
      normalization_image[(b_i->data[ii] + 200 * (j_data[ii] - 1)) - 1];
  }

  // 断层检测
  //  Magnification=0.8/((max(max(image_limit))));
  //  if Magnification>1
  //      image_limit=normalization_image*Magnification;
  //  else
  //      image_limit=normalization_image;
  //  end
  // %%%下限幅
  idx = 0;
  i = b_i->size[0];
  b_i->size[0] = 30000;
  emxEnsureCapacity_int32_T(b_i, i);
  ii = 1;
  jj = 1;
  exitg1 = false;
  while ((!exitg1) && (jj <= 150)) {
    guard1 = false;
    if (normalization_image[(ii + 200 * (jj - 1)) - 1] < a) {
      idx++;
      b_i->data[idx - 1] = ii;
      j_data[idx - 1] = (unsigned char)jj;
      if (idx >= 30000) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      ii++;
      if (ii > 200) {
        ii = 1;
        jj++;
      }
    }
  }

  if (1 > idx) {
    b_i->size[0] = 0;
  } else {
    i = b_i->size[0];
    b_i->size[0] = idx;
    emxEnsureCapacity_int32_T(b_i, i);
  }

  i = b_i->size[0];
  for (ii = 0; ii < i; ii++) {
    image_limit[(b_i->data[ii] + 200 * (j_data[ii] - 1)) - 1] = 1.0E-100;
  }

  // %%上限幅
  idx = 0;
  i = b_i->size[0];
  b_i->size[0] = 30000;
  emxEnsureCapacity_int32_T(b_i, i);
  ii = 1;
  jj = 1;
  exitg1 = false;
  while ((!exitg1) && (jj <= 150)) {
    guard1 = false;
    if (normalization_image[(ii + 200 * (jj - 1)) - 1] > b) {
      idx++;
      b_i->data[idx - 1] = ii;
      j_data[idx - 1] = (unsigned char)jj;
      if (idx >= 30000) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      ii++;
      if (ii > 200) {
        ii = 1;
        jj++;
      }
    }
  }

  if (1 > idx) {
    b_i->size[0] = 0;
  } else {
    i = b_i->size[0];
    b_i->size[0] = idx;
    emxEnsureCapacity_int32_T(b_i, i);
  }

  i = b_i->size[0];
  for (ii = 0; ii < i; ii++) {
    image_limit[(b_i->data[ii] + 200 * (j_data[ii] - 1)) - 1] = 1.0;
  }

  emxFree_int32_T(&b_i);
}

//
// File trailer for Amplitude_limiting.cpp
//
// [EOF]
//

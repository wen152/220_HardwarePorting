//
// File: rdivide_helper.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "rdivide_helper.hpp"
#include "Fmain_ISARimaging_emxutil.hpp"

// Function Definitions

//
// Arguments    : double x
//                const emxArray_real_T *y
//                emxArray_real_T *z
// Return Type  : void
//
void b_rdivide_helper(double x, const emxArray_real_T *y, emxArray_real_T *z)
{
  int i4;
  int loop_ub;
  i4 = z->size[0];
  z->size[0] = y->size[0];
  emxEnsureCapacity_real_T(z, i4);
  loop_ub = y->size[0];
  for (i4 = 0; i4 < loop_ub; i4++) {
    z->data[i4] = x / y->data[i4];
  }
}

//
// Arguments    : const double x_data[]
//                const int x_size[2]
//                const double y_data[]
//                double z_data[]
//                int z_size[2]
// Return Type  : void
//
void c_rdivide_helper(const double x_data[], const int x_size[2], const double
                      y_data[], double z_data[], int z_size[2])
{
  int loop_ub;
  int i6;
  z_size[0] = 1;
  z_size[1] = x_size[1];
  loop_ub = x_size[0] * x_size[1];
  for (i6 = 0; i6 < loop_ub; i6++) {
    z_data[i6] = x_data[i6] / y_data[i6];
  }
}

//
// Arguments    : const emxArray_real_T *x
//                const emxArray_real_T *y
//                emxArray_real_T *z
// Return Type  : void
//
void rdivide_helper(const emxArray_real_T *x, const emxArray_real_T *y,
                    emxArray_real_T *z)
{
  int i3;
  int loop_ub;
  i3 = z->size[0];
  z->size[0] = x->size[0];
  emxEnsureCapacity_real_T(z, i3);
  loop_ub = x->size[0];
  for (i3 = 0; i3 < loop_ub; i3++) {
    z->data[i3] = x->data[i3] / y->data[i3];
  }
}

//
// File trailer for rdivide_helper.cpp
//
// [EOF]
//

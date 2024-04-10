//
// File: Fmain_ISARimaging_terminate.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "Fmain_ISARimaging_terminate.hpp"
#include "Fmain_ISARimaging_rtwutil.hpp"
#include "Fmain_ISARimaging_data.hpp"

// Function Definitions

//
// Arguments    : void
// Return Type  : void
//
void Fmain_ISARimaging_terminate()
{
  emlrtFreeThreadStackData();
  omp_destroy_nest_lock(&emlrtNestLockGlobal);
}

//
// File trailer for Fmain_ISARimaging_terminate.cpp
//
// [EOF]
//

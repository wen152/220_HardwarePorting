//
// File: Fmain_ISARimaging_initialize.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "Fmain_ISARimaging_initialize.hpp"
#include "Fmain_ISARimaging_rtwutil.hpp"
#include "Fmain_ISARimaging_data.hpp"

// Function Definitions

//
// Arguments    : void
// Return Type  : void
//
void Fmain_ISARimaging_initialize()
{
  rt_InitInfAndNaN(8U);
  omp_init_nest_lock(&emlrtNestLockGlobal);
  emlrtInitThreadStackData();
}

//
// File trailer for Fmain_ISARimaging_initialize.cpp
//
// [EOF]
//

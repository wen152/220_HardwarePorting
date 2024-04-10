//
// File: Fmain_ISARimaging_rtwutil.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging_rtwutil.hpp"

// Variable Definitions
static Fmain_ISARimagingTLS *Fmain_ISARimagingTLSGlobal;

#pragma omp threadprivate (Fmain_ISARimagingTLSGlobal)

// Function Definitions

//
// Arguments    : void
// Return Type  : void
//
void emlrtFreeThreadStackData()
{
  int i;

#pragma omp parallel for schedule(static)\
 num_threads(omp_get_max_threads())

  for (i = 1; i <= omp_get_max_threads(); i++) {
    free(Fmain_ISARimagingTLSGlobal);
  }
}

//
// Arguments    : void
// Return Type  : Fmain_ISARimagingTLS *
//
Fmain_ISARimagingTLS *emlrtGetThreadStackData()
{
  return Fmain_ISARimagingTLSGlobal;
}

//
// Arguments    : void
// Return Type  : void
//
void emlrtInitThreadStackData()
{
  int i;

#pragma omp parallel for schedule(static)\
 num_threads(omp_get_max_threads())

  for (i = 1; i <= omp_get_max_threads(); i++) {
    Fmain_ISARimagingTLSGlobal = (Fmain_ISARimagingTLS *)malloc(1U * sizeof
      (Fmain_ISARimagingTLS));
  }
}

//
// File trailer for Fmain_ISARimaging_rtwutil.cpp
//
// [EOF]
//

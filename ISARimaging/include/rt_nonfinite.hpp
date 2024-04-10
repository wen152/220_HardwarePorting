/*
 * File: rt_nonfinite.h
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 07-Apr-2023 11:28:25
 */

#ifndef RT_NONFINITE_HPP
#define RT_NONFINITE_HPP
#include <stddef.h>
#include "rtwtypes.hpp"

extern real_T rtInf;
extern real_T rtMinusInf;
extern real_T rtNaN;
extern real32_T rtInfF;
extern real32_T rtMinusInfF;
extern real32_T rtNaNF;
extern void rt_InitInfAndNaN(size_t realSize);
extern boolean_T rtIsInf(real_T value);
extern boolean_T rtIsInfF(real32_T value);
extern boolean_T rtIsNaN(real_T value);
extern boolean_T rtIsNaNF(real32_T value);

#endif

/*
 * File trailer for rt_nonfinite.h
 *
 * [EOF]
 */

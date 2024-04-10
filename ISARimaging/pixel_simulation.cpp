//
// File: pixel_simulation.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include <cmath>
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "pixel_simulation.hpp"

// Function Definitions

//
// ���������ͷ�λ��ֱ���
//    �˴���ʾ��ϸ˵��
//  ������Ч���ٶ��뷽λ��ֱ���
// Arguments    : const emxArray_real_T *X_radar
//                const emxArray_real_T *Y_radar
//                double *pixel_a
//                double *pixel_r
// Return Type  : void
//
void pixel_simulation(const emxArray_real_T *X_radar, const emxArray_real_T
                      *Y_radar, double *pixel_a, double *pixel_r)
{
  double delta_theta;
  double theta_end;
  double b_delta_theta;
  double c_delta_theta;
  double d_delta_theta;
  double e_delta_theta;
  double f_delta_theta;
  double g_delta_theta;
  double h_delta_theta;
  double i_delta_theta;
  double j_delta_theta;
  double k_delta_theta;
  double l_delta_theta;
  double m_delta_theta;
  delta_theta = 0.0;
  if ((Y_radar->data[0] > 0.0) && (Y_radar->data[Y_radar->size[0] - 1] > 0.0) &&
      (X_radar->data[0] > 0.0) && (X_radar->data[Y_radar->size[0] - 1] > 0.0)) {
    // ��һ����
    delta_theta = std::atan(std::abs(Y_radar->data[0]) / X_radar->data[0]);
    theta_end = std::atan(std::abs(Y_radar->data[Y_radar->size[0] - 1]) /
                          X_radar->data[Y_radar->size[0] - 1]);
    if ((delta_theta > theta_end) || rtIsNaN(theta_end)) {
      b_delta_theta = delta_theta;
    } else {
      b_delta_theta = theta_end;
    }

    if ((delta_theta < theta_end) || rtIsNaN(theta_end)) {
      f_delta_theta = delta_theta;
    } else {
      f_delta_theta = theta_end;
    }

    delta_theta = std::abs(b_delta_theta - f_delta_theta);
  } else if ((Y_radar->data[0] > 0.0) && (Y_radar->data[Y_radar->size[0] - 1] <
              0.0) && (X_radar->data[0] > 0.0) && (X_radar->data[Y_radar->size[0]
              - 1] > 0.0)) {
    // ��һ���޵���������
    delta_theta = std::atan(std::abs(Y_radar->data[0]) / std::abs(X_radar->data
      [0]));
    theta_end = std::atan(std::abs(Y_radar->data[Y_radar->size[0] - 1]) / std::
                          abs(X_radar->data[Y_radar->size[0] - 1]));
    if ((delta_theta > theta_end) || rtIsNaN(theta_end)) {
      c_delta_theta = delta_theta;
    } else {
      c_delta_theta = theta_end;
    }

    if ((delta_theta < theta_end) || rtIsNaN(theta_end)) {
      h_delta_theta = delta_theta;
    } else {
      h_delta_theta = theta_end;
    }

    delta_theta = c_delta_theta + h_delta_theta;

    // abs(theta_start-theta_end)
  } else if ((Y_radar->data[0] > 0.0) && (Y_radar->data[Y_radar->size[0] - 1] <
              0.0) && (X_radar->data[0] > 0.0) && (X_radar->data[Y_radar->size[0]
              - 1] < 0.0)) {
    // ��һ���޵���������
    delta_theta = std::atan(std::abs(Y_radar->data[0]) / std::abs(X_radar->data
      [0]));
    theta_end = std::atan(std::abs(X_radar->data[Y_radar->size[0] - 1]) / std::
                          abs(Y_radar->data[Y_radar->size[0] - 1]));
    if ((delta_theta > theta_end) || rtIsNaN(theta_end)) {
      d_delta_theta = delta_theta;
    } else {
      d_delta_theta = theta_end;
    }

    if ((delta_theta < theta_end) || rtIsNaN(theta_end)) {
      j_delta_theta = delta_theta;
    } else {
      j_delta_theta = theta_end;
    }

    delta_theta = (d_delta_theta + j_delta_theta) + 1.5707963267948966;

    // abs(theta_start-theta_end)
  } else if ((Y_radar->data[0] < 0.0) && (Y_radar->data[Y_radar->size[0] - 1] <
              0.0) && (X_radar->data[0] > 0.0) && (X_radar->data[Y_radar->size[0]
              - 1] > 0.0)) {
    // ��������
    delta_theta = std::atan(std::abs(Y_radar->data[0]) / X_radar->data[0]);
    theta_end = std::atan(std::abs(Y_radar->data[Y_radar->size[0] - 1]) /
                          X_radar->data[Y_radar->size[0] - 1]);
    if ((delta_theta > theta_end) || rtIsNaN(theta_end)) {
      e_delta_theta = delta_theta;
    } else {
      e_delta_theta = theta_end;
    }

    if ((delta_theta < theta_end) || rtIsNaN(theta_end)) {
      k_delta_theta = delta_theta;
    } else {
      k_delta_theta = theta_end;
    }

    delta_theta = std::abs(e_delta_theta - k_delta_theta);
  } else if ((Y_radar->data[0] < 0.0) && (Y_radar->data[Y_radar->size[0] - 1] <
              0.0) && (X_radar->data[0] * X_radar->data[Y_radar->size[0] - 1] <
                       0.0)) {
    // �������޵��������޻�������޵���������
    delta_theta = std::atan(std::abs(X_radar->data[0]) / std::abs(Y_radar->data
      [0]));
    theta_end = std::atan(std::abs(X_radar->data[Y_radar->size[0] - 1]) / std::
                          abs(Y_radar->data[Y_radar->size[0] - 1]));
    if ((delta_theta > theta_end) || rtIsNaN(theta_end)) {
      g_delta_theta = delta_theta;
    } else {
      g_delta_theta = theta_end;
    }

    if ((delta_theta < theta_end) || rtIsNaN(theta_end)) {
      l_delta_theta = delta_theta;
    } else {
      l_delta_theta = theta_end;
    }

    delta_theta = g_delta_theta + l_delta_theta;

    // abs(theta_start-theta_end)
  } else {
    if ((Y_radar->data[0] < 0.0) && (Y_radar->data[Y_radar->size[0] - 1] < 0.0) &&
        (X_radar->data[0] < 0.0) && (X_radar->data[Y_radar->size[0] - 1] < 0.0))
    {
      // ��������
      delta_theta = std::atan(std::abs(X_radar->data[0]) / std::abs
        (Y_radar->data[0]));
      theta_end = std::atan(std::abs(X_radar->data[Y_radar->size[0] - 1]) / std::
                            abs(Y_radar->data[Y_radar->size[0] - 1]));
      if ((delta_theta > theta_end) || rtIsNaN(theta_end)) {
        i_delta_theta = delta_theta;
      } else {
        i_delta_theta = theta_end;
      }

      if ((delta_theta < theta_end) || rtIsNaN(theta_end)) {
        m_delta_theta = delta_theta;
      } else {
        m_delta_theta = theta_end;
      }

      delta_theta = i_delta_theta - m_delta_theta;

      // abs(theta_start-theta_end)
    }
  }

  *pixel_r = 0.05;

  // ���������ظ���
  *pixel_a = 0.015 / delta_theta;
}

//
// File trailer for pixel_simulation.cpp
//
// [EOF]
//

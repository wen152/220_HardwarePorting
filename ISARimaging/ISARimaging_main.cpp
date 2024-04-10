#define _CRT_SECURE_NO_WARNINGS
/*
 * File: main.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 28-Mar-2023 11:38:36
 */

 /*************************************************************************/
 /* This automatically generated example C main file shows how to call
  entry-point functions that MATLAB Coder generated. You must customize
 this file for your application. Do not modify this file directly.
 Instead, make a copy of this file, modify it, and integrate it into
 your development environment.                                         */
 /*                                                                    */
 /* This file initializes entry-point function arguments to a default
  size and value before calling the entry-point functions. It does
  not store or use any values returned from the entry-point functions.
  If necessary, it does pre-allocate memory for returned values.
  You can use this file as a starting point for a main function that
 you can deploy in your application.                                   */
 /*                                                                    */
 /* After you copy the file, and before you deploy it, you must make the
 following changes:                                                    */
 /*  For variable-size function arguments, change the example sizes to
 the sizes that your application requires.
 Change the example values of function arguments to the values that
 your application requires.                                            */
 /* * If the entry-point functions return values, store these values or   */
 /* otherwise use them as required by your application.                   */
 /*                                                                       */
 /*************************************************************************/
 /* Include Files */
#include "math.h"
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
//#include "main.h"
#include "Fmain_ISARimaging_terminate.hpp"
#include "Fmain_ISARimaging_emxAPI.hpp"
#include "Fmain_ISARimaging_initialize.hpp"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <string>
#include "command_option.hpp"
#include "ISARimaging_main.hpp"

using namespace cv;
using std::chrono::system_clock;

int ISARimaging()
{
    int range = 5;
    emxArray_real_T* I1;
    int M = 150;
    int N = 200;	//ͼƬ���С�������
    Mat img = Mat(M, N, CV_8UC1);	//����һ��M*N��200��150��С�ı���  
    
  /* Initialize the application.
     You do not need to do this more than one time. */
    printf("Return data: 0.Parameter defined\n");
    system_clock::time_point time_1 = system_clock::now();
    Fmain_ISARimaging_initialize();
    printf("Return data: 1.Initialize finished\n");
    //std::string path = argv[1];//成像结果保存
    //std::string file1 = argv[2];//data.csv
    //std::string file2 = argv[3]; //posxy.csv
    std::string path = FLAGS_isar_output_dir + "/out.tif";
    emxInitArray_real_T(&I1, 2);
    Fmain_ISARimaging(range, I1, img, time_1);
    printf("Return data: 2.IASR imaging finished\n");
    system_clock::time_point end = system_clock::now();
    std::chrono::duration<double, std::micro> elapsed = end - time_1;
    std::cout << "execution time:" << (double)elapsed.count() << "us" << std::endl;
    //imshow("source image1", img);
    imwrite(path, img);
    waitKey(0);
    return 0;
}

/*
 * File trailer for main.cpp
 *
 * [EOF]
 */

//
// Created by hp on 2023/10/15.
//
#ifndef INCLUDE_FUNC_TRACK_HPP
#define INCLUDE_FUNC_TRACK_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <numeric>
// #define _USE_MATH_DEFINES        //M_PI
#include <cmath>
#include <complex>
#include "fftw3.h"
//#pragma comment(lib, "libfftw3-3.lib")

using namespace std;

void read_data(double *data, int rowr, string file);
void fftshift(complex<double> *x, int size);
int find_max(complex<double> *x, int size);
int find_max(double *x, int size);
int Keystone_s(complex<double> *x, int p0, double Kr, double fc,
             double T, int N, double fs, double tau);

class MyFFT {
private:
    int Nfft;
    int Sw;
    fftw_complex *_fftIn;
    fftw_complex *_fftOut;
    fftw_plan _fftPlan;
public:
    MyFFT();
    MyFFT(int N, int s);
    ~MyFFT();
    void DFT(complex<double> in[], int size_in, complex<double> out[]);
    void IDFT(std::complex<double> in[], std::complex<double> out[]);
};

#endif
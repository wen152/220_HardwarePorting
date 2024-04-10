//
// Created by hp on 2023/10/15.
//
#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<numeric>
// #define _USE_MATH_DEFINES        //M_PI
#include<cmath>
#include<complex>
#include "fftw3.hpp"
//#pragma comment(lib, "libfftw3-3.lib")
#include "Func_track.hpp"

const double PI = 3.1415926535897932384626433832795;

void read_data(double *data, int rowr, string file) {
    std::ifstream fp(file);

    if (!fp.is_open()) {
        cout << "Error: opening file fail" << endl;
        exit(1);
    }

    //  set variable to save data
    string line;
    string word;
    istringstream sin;
    int count0 = 0;


    getline(fp, line);

    while (getline(fp, line)) {

        if (count0 < rowr) {
            count0++;
            continue;
        }

        sin.clear();
        sin.str(line);
        getline(sin, word, ',');
        int count1 = 0;

        while (getline(sin, word, ',')) {
            stringstream tra;
            tra << word;
            tra >> *(data + (count0 - rowr) * 7 + count1);
            count1++;
        }
        count0++;
        if (count0 == (rowr + 128)) {
            break;
        }
    }

    fp.close();
    fp.clear();
    return;
};

MyFFT::MyFFT() {
    Nfft   = 0;
    Sw    = 1;
    _fftIn  = nullptr;
    _fftOut = nullptr;
};

MyFFT::MyFFT(int N, int s) {
    Nfft = N;
    Sw = s;
    _fftIn = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * Nfft);
    _fftOut = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * Nfft);
    if(Sw = 1){
        _fftPlan = fftw_plan_dft_1d(Nfft, _fftIn, _fftOut, FFTW_FORWARD, FFTW_MEASURE);
    }else{
        _fftPlan = fftw_plan_dft_1d(Nfft, _fftIn, _fftOut, FFTW_BACKWARD, FFTW_MEASURE);
    };
};
MyFFT::~MyFFT() {
    if (_fftIn != nullptr) {
        fftw_free(_fftIn);
    };
    if (_fftOut != nullptr) {
        fftw_free(_fftOut);
    };
    fftw_destroy_plan(_fftPlan);
};
void MyFFT::DFT(std::complex<double> in[], int size_in, std::complex<double> out[]) {
    for (int i = 0; i < Nfft; i++) {
        if (i < size_in) {
            _fftIn[i][0] = in[i].real();
            _fftIn[i][1] = in[i].imag();
        } else {
            _fftIn[i][0] = 0.0;
            _fftIn[i][1] = 0.0;
        }
    }

    fftw_execute(_fftPlan);

    for (int i = 0; i < Nfft; i++) {
        out[i] = std::complex<double>(_fftOut[i][0], _fftOut[i][1]);
    }
};
void MyFFT::IDFT(std::complex<double> in[], std::complex<double> out[]) {
    for (int i = 0; i < Nfft; i++) {
        _fftIn[i][0] = in[i].real();
        _fftIn[i][1] = in[i].imag();
    }

    fftw_execute(_fftPlan);

    for (int i = 0; i < Nfft; i++) {
        out[i] = complex<double>(_fftOut[i][0] / Nfft, _fftOut[i][1] / Nfft) ;
    }
};

void fftshift(complex<double> *x, int size) {
    int space = floor(size / 2);
    complex<double> temp;
    for (int i = 0; i < space; i++) {
        temp = x[i];
        x[i] = x[i + space];
        x[i + space] = temp;
    }
    return;
};

int find_max(complex<double> *x, int size) {
    int index = 0;
    for (int i = 0; i < size; i++) {
        if (abs(x[i]) > abs(x[index])) {
            index = i;
        }

    }
    return index;

};

int find_max(double *x, int size) {
    int index = 0;
    for (int i = 0; i < size; i++) {
        if (x[i] > x[index]) {
            index = i;
        }

    }
    return index;

};

int Keystone_s(complex<double> *x1, int p0, double Kr, double fc,
              double T, int N, double fs, double tau){
    int Km[112];
    int l = 256;
    double *t = new double[l];
    int j = 0;
    for(int i = -120; i<121; i++){
        if(i>-65 & i<65){
            continue;
        }
        Km[j]=i;
        j++;
    };
    for(int i=0; i <l; i++){
        t[i]=i*N/fs-tau;
    };
    complex<double> *src = new complex<double>[l];
    complex<double> *Src1 = new complex<double>[l];
    int K=0;
    int pf = 20;
    MyFFT *FFT= new MyFFT(l, 1);
    for(int i = 0; i<112; i++){
        for(int m = 0; m<l; m++){
            complex<double> w0 = exp(complex<double>(0,-2*PI*((fc+t[m]*Kr)/fc)*Km[i]*127));
            src[m] = w0 * x1[m];
        };
//        FFT->DFT( x1, 1024, Src0);
        FFT->DFT( src, l, Src1);
        int p1 = find_max(Src1,l);
        int p = abs(p1-p0);
        if(p==0){
            K = Km[i];
            break;
        };
        if(p<pf){
            pf = p;
            K = Km[i];
        }
    };

    delete[] src, delete[] Src1, delete FFT;
    delete[] t;
    return K;
};

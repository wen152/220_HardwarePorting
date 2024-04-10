#ifndef INCLUDE_TRACK_HPP
#define INCLUDE_TRACK_HPP

#include <iostream>
#include <complex>
using namespace std;

const int Nm = 128;
const int pl = 128;
const double Pi = acos(-1);
static const double c = 3e+8;
static const double Tp = 0.003;
static const double Br = 2.5e+5;
static const double Kr = Br / Tp;
static const double fc = 4e+9;
static const double fs = 2 * Br;
static const double T = 0.1;
static const int snr = -5;
static const double Pw = Tp / T;
static const double Ref = 1e+5;
static const double Tc = Tp + 2 * 1.1e5 / c;
static const int Nr = round(Tp / (1 / fs));
static const int N = pow(2, ceil(log2(Nr)));
int Track();
double echo_gen(int sta, string file);       //step 1 function
void read_data1(int rowr, string file);
void read_ref(string file);
std::pair<double, double> Target_position();             //step 2 function
void Keystone_t( int k, double tau);
extern double Rmean;
extern double Vmean;
extern double d_xyz[4];
extern double *ts;
extern double *Range;
extern complex<double> (*sr)[1500];
//complex<double> s1[128][150000];
//complex<double> S1[Nm][262144];
extern complex<double> *srf ;
extern complex<double> (*s2)[256];
extern complex<double> (*Src1)[256];
//头文件中谨慎定义变量（int a），会造成链接时重定义
//extern int a就不会出错，只声明变量，不分配内存

// 头文件中定义 static const 类型的变量时，这些变量被视
// 为每个包含该头文件的源文件独立存在的局部变量，而不
// 是全局变量。因此，不会发生重复链接的错误。

#endif
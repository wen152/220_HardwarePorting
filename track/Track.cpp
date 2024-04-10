// Created by hp on 2023/10/15.
#include <iostream>
#include "Track.hpp"
#include <numeric>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <complex>
#include <algorithm>
#include <string>
#include "Func_track.hpp"
#include "command_option.hpp"

using namespace std;
double *ts = new double[Nr];
double *Range = new double[N];
complex<double> (*sr)[1500] = new complex<double>[Nm][1500];
//complex<double> s1[128][150000];
//complex<double> S1[Nm][262144];
complex<double> *srf = new complex<double>[Nr];
complex<double> (*s2)[256] = new complex<double>[Nm][256];
complex<double> (*Src1)[256] = new complex<double>[Nm][256];
double d_xyz[4];
double Rmean;
double Vmean;
int Track() {
//double *ta = new double[Nm];
//double *f_s = new double[Nm];
//double *Vr = new double[Nm];
// double (*ts_delay)[150000] = new double[Nm][150000];
// complex<double> Wn[262144];
    int sta = 1100, en = 1110;          //0<=sta<en<=2812
    double error_D = 0;
    double error_V = 0;
    for(int a = 0; a<en-sta; a++){
        clock_t start,end;  //定义clock_t变量
        srand((int) time(0));
        read_data1(a*Nm+1, FLAGS_track_data);
        read_ref(FLAGS_track_dataref);
        echo_gen(sta+a, FLAGS_track_targettocamera);
        start = clock();    //开始时间
        std::pair<double, double> result = Target_position();
        end = clock();
        cout<<"One loop time = "<<double(end-start)/CLOCKS_PER_SEC<<"s"<<endl;  //输出时间（单位：ｓ）
        double Distance = result.first;
        double V_result = result.second;
        error_D += abs(abs(Distance)-Rmean)/Rmean*100;
        error_V += abs(abs(V_result)-abs(Vmean))/abs(Vmean)*100;
    };
    error_D = error_D/(en - sta);
    error_V = error_V/(en - sta);
    cout << "Signal processing complete.\nThe Range error is " << error_D << "%" << endl;
    cout << "The Velocity error is " << error_V << "%\n";
    return 0;
}

void read_data1(int rowr,string file) {
    ifstream fp(file);

    if (!fp.is_open()) {
        cout << "Error: opening file fail" << endl;
        exit(1);
    }

    //  set variable to save data
    string line;
    string word;
    string part;
    istringstream sin;
    istringstream win;
    int count0 = 0;
//    getline(fp, line);

    while (getline(fp, line)) {

        if (count0 < rowr) {
            count0++;
            continue;
        }

        sin.clear();
        sin.str(line);
//        getline(sin, word, ',');
        int count1 = 0;

        while (getline(sin, word, ',')) {
            win.clear();
            win.str(word);
            getline(win, part, '+');
            double d;
            d = stod(part);
            sr[count0-rowr][count1].real(d);
            getline(win, part, '+');
            string imag=part.substr(0,part.length()-1);
            d = stod(imag);
//            tra << imag;
//            tra >> d;

            sr[count0-rowr][count1].imag(d);
            count1++;
        }
        count0++;
        if (count0 == (rowr + 128)) {
            break;
        }
    }

    fp.close();
    fp.clear();
//    cout << sr[0][227] << endl;

    return;
};

void read_ref(string file) {
    std::ifstream fp(file);

    if (!fp.is_open()) {
        cout << "Error: opening file fail" << endl;
        exit(1);
    }

    //  set variable to save data
    string line;
    string word;
    string part;
    istringstream sin;
    istringstream win;
//    int count0 = 0;
    getline(fp, line);
    getline(fp,line);

    sin.clear();
    sin.str(line);

    int count1 = 0;

    while (getline(sin, word, ',')) {
        win.clear();
        win.str(word);

        getline(win, part, '+');
        double d;
        d =stod(part);
        srf[count1].real(d);
        getline(win, part, '+');
        string imag=part.substr(0,part.length()-1);
        d = stod(imag);
//            tra << imag;
//            tra >> d;

        srf[count1].imag(d);
        count1++;
    }

    fp.close();
    fp.clear();

    return;
};

double echo_gen(int sta, string file) {


    double data[128][7];
    double *d = &data[0][0];
    int rowr = Nm * sta;
    read_data(d, rowr, file);


    double Tx[Nm];
    double Ty[Nm];
    double Tz[Nm];
    double Rm[Nm];
    double Vmx[Nm];
    double Vmy[Nm];
    double Vmz[Nm];
    double dxy[Nm];
    double Vm[Nm];


    for (int i = 0; i < Nm; i++) {
        Tx[i] = data[i][0] * 1000;
        Ty[i] = data[i][1] * 1000;
        Tz[i] = data[i][2] * 1000;
        Rm[i] = data[i][3] * 1000;
        Vmx[i] = data[i][4] * 1000;
        Vmy[i] = data[i][5] * 1000;
        Vmz[i] = data[i][6] * 1000;
        dxy[i] = sqrt(pow(Ty[i], 2) + pow(Tx[i], 2));
        double vxy = Ty[i] / dxy[i] * Vmy[i] + Tx[i] / dxy[i] * Vmx[i];
        Vm[i] = dxy[i] / Rm[i] * vxy + Tz[i] / Rm[i] * Vmz[i];
    }
    Rmean = accumulate(Rm, Rm + Nm, .0) / Nm;
    Vmean = accumulate(Vm, Vm + Nm, .0) / Nm;
    double Txmean = accumulate(Tx, Tx + Nm, .0);
    double Tymean = accumulate(Ty, Ty + Nm, .0);
    double Tzmean = accumulate(Tz, Tz + Nm, .0);
    double dxymean = accumulate(dxy, dxy + Nm, .0);

    d_xyz[0] = Tzmean / Nm / Rmean;
    d_xyz[1] = dxymean / Nm / Rmean;
    d_xyz[2] = Txmean / dxymean;                  //x
    d_xyz[3] = Tymean / dxymean;                  //y

//    /*生成回波*/
//    double step = Tp / (Nr - 1);
//    for (int i = 0; i < Nr; i++) {
//        ts[i] = i * step - Tp / 2;
//    }
//    complex<double> noise;
//    double rectp;
//    double ts_delay;
////    complex<double> summ = 0 ;
//    for (int i = 0; i < Nm; i++)
//    {
//        for (int j = 0; j < Nr; j++) {
//            ts_delay = ts[j] - Rm[i] * 2 / c;
//            if (abs(ts_delay) < Tp / 2)
//            {
//                rectp = 1.0;
//            } else {
//                rectp = 0.0;
//            }
//            // noise = Pw/pow(10, snr/10)*complex<double>(random(-1,1),random(-1,1));
//            // sr[i][j] = noise;
//            sr[i][j] = rectp * complex<double>(cos(2 * Pi * (fc * (ts_delay) + 1.0 / 2 * Kr * pow(ts_delay, 2))),
//                                               sin(2 * Pi * (fc * (ts_delay) + 1.0 / 2 * Kr * pow(ts_delay, 2))));
//            summ += sr[i][j];

//        }

//    }

    return 0;
}
pair<double, double> Target_position(){
    complex<double> (*s1)[1500] = new complex<double>[Nm][1500];
    complex<double> (*S1)[2048] = new complex<double>[Nm][2048];
    complex<double> (*S2)[256] = new complex<double>[Nm][256];
     for (int i = 0; i < Nm; i++)                            //dechirp
     {
         for (int j = 0; j < Nr; j++)
         {
             s1[i][j] = srf[j]*conj(sr[i][j]);
             // if (j%10000 == 2)
             // {
                 // cout << srf[j] << " -- " << sr[i][j] << " -- " << s1[i][j] << endl;
             // }

         }
     }
     double step = fs/(N-1);
     for (int i = 0; i < N; i++)
     {
         Range[i] = Ref - (i*step - fs/2)*c/2/Kr;
     }
     int ind_max[Nm];
     double *Rang_d = new double[2*pl];
     MyFFT *FFT_ALg = new MyFFT(N, 1);

     for (int i = 0; i < Nm; i++)
     {
//         fft(s1[i], Nr, N, S1[i]);
         FFT_ALg->DFT(s1[i], Nr, S1[i]);
         fftshift(S1[i], N);
         ind_max[i] = find_max(S1[i], N);
     };
     int position = ind_max[Nm/2];
     MyFFT *FFT_I = new MyFFT(2*pl, 0);
     for (int i = 0; i < Nm; i++)
     {
         for (int j = 0; j < 2*pl; j++)
         {
             if (j+position-pl >= N || j+position-pl < 0)
             {
                S2[i][j]=complex<double>(0,0);
                Rang_d[j] = 9e20;
             }
             else{
                S2[i][j] = S1[i][j+position-pl];
                Rang_d[j] = Range[j+position-pl];
             }
         }
         FFT_I -> IDFT(S2[i], s2[i]);
//         ifft(S2[i], 2*pl, 2*pl, s2[i]);
     };
     delete FFT_I;delete FFT_ALg; delete[] s1; delete[] S1;
     double tau = Range[position]*2/c;
     int p0 = find_max(S2[Nm/2], 2*pl);
     delete[] S2;
     int V_k = Keystone_s( s2[Nm-1], p0, Kr, fc,
             T, N, fs, tau);
     Keystone_t(V_k, tau);

     complex<double> *S_sum = new complex<double>[2*pl]();
     for(int i=0; i<2*pl; i++){
         for(int j=0; j<Nm; j++){
             S_sum[i] += Src1[j][i];
         }
     }
     p0 = find_max(S_sum, 2*pl);
     double Distance = Rang_d[p0]-37095;
     delete[] S_sum, delete[] Rang_d;

     double *V_real = new double[Nm];
     for(int i = 0; i<Nm; i++){
        V_real[i] =  -1*(T/Nm*i - T/2 + V_k/T)*c/2/fc;
     };

    complex<double> (*Src1_T)[128] = new complex<double>[256][128];    //Matrix Transpose
    complex<double> (*Src2_T)[128] = new complex<double>[256][128];
    complex<double> *Src2_sum = new complex<double>[Nm]();
    MyFFT *FFT = new MyFFT(Nm, 1);
    for(int i = 0; i<Nm; i++){
        for(int j = 0; j<2*pl; j++){
            Src1_T[j][i] = Src1[i][j];
        };
    };
    for(int i=0; i<2*pl; i++){
        FFT->DFT(Src1_T[i], Nm, Src2_T[i]);
    };
    for(int i = 0; i<Nm; i++){
        for(int j = 0; j<2*pl; j++){
            Src2_sum[i] += Src2_T[j][i];
        };
    };
    p0  = find_max(Src2_sum, Nm);

    double V_result = V_real[p0];
    delete[] Src1_T, delete[] Src2_T, delete[] Src2_sum, delete[] V_real, delete FFT;
    std::pair<double, double> result = std::make_pair(Distance, V_result);
     return result;
 }

void Keystone_t( int k, double tau){
    int l = 2*pl;
    complex<double> (*src)[256] = new complex<double>[Nm][256];
    double *t = new double[l];
    for(int i=0; i <l; i++){
        t[i]=i*N/fs-tau;
    };
    MyFFT *FFT= new MyFFT(l, 1);
    for(int i = 0; i<Nm; i++){
        for(int m = 0; m<l; m++){
            complex<double> w0 = exp(complex<double>(0,-2*Pi*((fc+t[m]*Kr)/fc)*k*i));
            src[i][m] = w0 * s2[i][m];
        };
//        FFT->DFT( x1, 1024, Src0);
        FFT->DFT( src[i], l, Src1[i]);
    };
    delete[] t; delete[] src; delete FFT;
};
// void read_data(double *data, int rowr, string file) {
//     std::ifstream fp(file);

//     if (!fp.is_open()) {
//         cout << "Error: opening file fail" << endl;
//         exit(1);
//     }

//     //  set variable to save data
//     string line;
//     string word;
//     istringstream sin;
//     int count0 = 0;


//     getline(fp, line);

//     while (getline(fp, line)) {

//         if (count0 < rowr) {
//             count0++;
//             continue;
//         }

//         sin.clear();
//         sin.str(line);
//         getline(sin, word, ',');
//         int count1 = 0;

//         while (getline(sin, word, ',')) {
//             stringstream tra;
//             tra << word;
//             tra >> *(data + (count0 - rowr) * 7 + count1);
//             count1++;
//         }
//         count0++;
//         if (count0 == (rowr + 128)) {
//             break;
//         }
//     }

//     fp.close();
//     fp.clear();
//     return;
// };

//
// File: fft1.cpp
//
// MATLAB Coder version            : 4.1
// C/C++ source code generated on  : 07-Apr-2023 11:28:25
//

// Include Files
#include <string.h>
#include <cmath>
#include "rt_nonfinite.hpp"
#include "Fmain_ISARimaging.hpp"
#include "fft1.hpp"
#include "Fmain_ISARimaging_emxutil.hpp"
#include "Fmain_ISARimaging_rtwutil.hpp"
#include "bluesteinSetup.hpp"

// Variable Definitions
static const double dv0[513] = { 1.0, 0.99998117528260111, 0.9999247018391445,
  0.9998305817958234, 0.99969881869620425, 0.99952941750109314,
  0.99932238458834954, 0.99907772775264536, 0.99879545620517241,
  0.99847558057329477, 0.99811811290014918, 0.99772306664419164,
  0.99729045667869021, 0.99682029929116567, 0.996312612182778,
  0.99576741446765982, 0.99518472667219693, 0.99456457073425542,
  0.99390697000235606, 0.9932119492347945, 0.99247953459871, 0.99170975366909953,
  0.99090263542778, 0.99005821026229712, 0.989176509964781, 0.98825756773074946,
  0.98730141815785843, 0.98630809724459867, 0.98527764238894122,
  0.984210092386929, 0.98310548743121629, 0.98196386910955524,
  0.98078528040323043, 0.97956976568544052, 0.97831737071962765,
  0.97702814265775439, 0.97570213003852857, 0.97433938278557586,
  0.97293995220556018, 0.97150389098625178, 0.970031253194544,
  0.96852209427441727, 0.96697647104485207, 0.9653944416976894,
  0.96377606579543984, 0.96212140426904158, 0.96043051941556579,
  0.9587034748958716, 0.95694033573220882, 0.95514116830577078,
  0.95330604035419386, 0.95143502096900834, 0.94952818059303667,
  0.94758559101774109, 0.94560732538052128, 0.94359345816196039,
  0.94154406518302081, 0.93945922360218992, 0.937339011912575,
  0.93518350993894761, 0.932992798834739, 0.93076696107898371,
  0.92850608047321559, 0.92621024213831138, 0.92387953251128674,
  0.9215140393420419, 0.91911385169005777, 0.9166790599210427,
  0.91420975570353069, 0.91170603200542988, 0.90916798309052238,
  0.90659570451491533, 0.90398929312344334, 0.901348847046022,
  0.89867446569395382, 0.89596624975618522, 0.89322430119551532,
  0.89044872324475788, 0.88763962040285393, 0.88479709843093779,
  0.881921264348355, 0.87901222642863353, 0.8760700941954066,
  0.87309497841829009, 0.87008699110871146, 0.86704624551569265,
  0.8639728561215867, 0.86086693863776731, 0.85772861000027212,
  0.85455798836540053, 0.8513551931052652, 0.84812034480329723,
  0.84485356524970712, 0.84155497743689844, 0.83822470555483808,
  0.83486287498638, 0.83146961230254524, 0.8280450452577558, 0.82458930278502529,
  0.82110251499110465, 0.81758481315158371, 0.81403632970594841,
  0.81045719825259477, 0.80684755354379933, 0.80320753148064494,
  0.799537269107905, 0.79583690460888357, 0.79210657730021239,
  0.78834642762660634, 0.78455659715557524, 0.78073722857209449,
  0.77688846567323244, 0.773010453362737, 0.7691033376455797, 0.765167265622459,
  0.76120238548426178, 0.75720884650648457, 0.75318679904361252,
  0.74913639452345937, 0.745057785441466, 0.74095112535495922,
  0.73681656887736979, 0.73265427167241282, 0.7284643904482252,
  0.724247082951467, 0.72000250796138165, 0.71573082528381859,
  0.71143219574521643, 0.70710678118654757, 0.7027547444572253,
  0.69837624940897292, 0.69397146088965389, 0.68954054473706683,
  0.68508366777270036, 0.680600997795453, 0.67609270357531592,
  0.67155895484701833, 0.66699992230363747, 0.66241577759017178,
  0.65780669329707864, 0.65317284295377676, 0.64851440102211244,
  0.64383154288979139, 0.63912444486377573, 0.63439328416364549,
  0.629638238914927, 0.62485948814238634, 0.6200572117632891,
  0.61523159058062682, 0.61038280627630948, 0.60551104140432555,
  0.600616479383869, 0.59569930449243336, 0.59075970185887416,
  0.58579785745643886, 0.58081395809576453, 0.57580819141784534,
  0.57078074588696726, 0.56573181078361312, 0.560661576197336,
  0.55557023301960218, 0.55045797293660481, 0.54532498842204646,
  0.54017147272989285, 0.53499761988709715, 0.52980362468629461,
  0.524589682678469, 0.51935599016558964, 0.51410274419322166, 0.508830142543107,
  0.50353838372571758, 0.49822766697278181, 0.49289819222978404,
  0.487550160148436, 0.48218377207912272, 0.47679923006332209,
  0.47139673682599764, 0.46597649576796618, 0.46053871095824,
  0.45508358712634384, 0.44961132965460654, 0.4441221445704292,
  0.43861623853852766, 0.43309381885315196, 0.42755509343028208,
  0.42200027079979968, 0.41642956009763715, 0.41084317105790391,
  0.40524131400498986, 0.39962419984564679, 0.3939920400610481,
  0.38834504669882625, 0.38268343236508978, 0.37700741021641826,
  0.37131719395183749, 0.36561299780477385, 0.35989503653498811,
  0.35416352542049034, 0.34841868024943456, 0.34266071731199438,
  0.33688985339222005, 0.33110630575987643, 0.32531029216226293,
  0.31950203081601569, 0.31368174039889152, 0.30784964004153487,
  0.30200594931922808, 0.29615088824362379, 0.29028467725446233,
  0.28440753721127188, 0.27851968938505306, 0.272621355449949,
  0.26671275747489837, 0.26079411791527551, 0.25486565960451457,
  0.24892760574572015, 0.24298017990326387, 0.2370236059943672,
  0.23105810828067111, 0.22508391135979283, 0.2191012401568698,
  0.21311031991609136, 0.20711137619221856, 0.2011046348420919,
  0.19509032201612825, 0.18906866414980619, 0.18303988795514095,
  0.17700422041214875, 0.17096188876030122, 0.16491312048996992,
  0.15885814333386145, 0.15279718525844344, 0.14673047445536175,
  0.14065823933284921, 0.13458070850712617, 0.12849811079379317,
  0.1224106751992162, 0.11631863091190475, 0.11022220729388306,
  0.10412163387205459, 0.0980171403295606, 0.091908956497132724,
  0.0857973123444399, 0.079682437971430126, 0.073564563599667426,
  0.067443919563664051, 0.061320736302208578, 0.055195244349689941,
  0.049067674327418015, 0.04293825693494082, 0.036807222941358832,
  0.030674803176636626, 0.024541228522912288, 0.01840672990580482,
  0.012271538285719925, 0.0061358846491544753, 0.0, -0.0061358846491544753,
  -0.012271538285719925, -0.01840672990580482, -0.024541228522912288,
  -0.030674803176636626, -0.036807222941358832, -0.04293825693494082,
  -0.049067674327418015, -0.055195244349689941, -0.061320736302208578,
  -0.067443919563664051, -0.073564563599667426, -0.079682437971430126,
  -0.0857973123444399, -0.091908956497132724, -0.0980171403295606,
  -0.10412163387205459, -0.11022220729388306, -0.11631863091190475,
  -0.1224106751992162, -0.12849811079379317, -0.13458070850712617,
  -0.14065823933284921, -0.14673047445536175, -0.15279718525844344,
  -0.15885814333386145, -0.16491312048996992, -0.17096188876030122,
  -0.17700422041214875, -0.18303988795514095, -0.18906866414980619,
  -0.19509032201612825, -0.2011046348420919, -0.20711137619221856,
  -0.21311031991609136, -0.2191012401568698, -0.22508391135979283,
  -0.23105810828067111, -0.2370236059943672, -0.24298017990326387,
  -0.24892760574572015, -0.25486565960451457, -0.26079411791527551,
  -0.26671275747489837, -0.272621355449949, -0.27851968938505306,
  -0.28440753721127188, -0.29028467725446233, -0.29615088824362379,
  -0.30200594931922808, -0.30784964004153487, -0.31368174039889152,
  -0.31950203081601569, -0.32531029216226293, -0.33110630575987643,
  -0.33688985339222005, -0.34266071731199438, -0.34841868024943456,
  -0.35416352542049034, -0.35989503653498811, -0.36561299780477385,
  -0.37131719395183749, -0.37700741021641826, -0.38268343236508978,
  -0.38834504669882625, -0.3939920400610481, -0.39962419984564679,
  -0.40524131400498986, -0.41084317105790391, -0.41642956009763715,
  -0.42200027079979968, -0.42755509343028208, -0.43309381885315196,
  -0.43861623853852766, -0.4441221445704292, -0.44961132965460654,
  -0.45508358712634384, -0.46053871095824, -0.46597649576796618,
  -0.47139673682599764, -0.47679923006332209, -0.48218377207912272,
  -0.487550160148436, -0.49289819222978404, -0.49822766697278181,
  -0.50353838372571758, -0.508830142543107, -0.51410274419322166,
  -0.51935599016558964, -0.524589682678469, -0.52980362468629461,
  -0.53499761988709715, -0.54017147272989285, -0.54532498842204646,
  -0.55045797293660481, -0.55557023301960218, -0.560661576197336,
  -0.56573181078361312, -0.57078074588696726, -0.57580819141784534,
  -0.58081395809576453, -0.58579785745643886, -0.59075970185887416,
  -0.59569930449243336, -0.600616479383869, -0.60551104140432555,
  -0.61038280627630948, -0.61523159058062682, -0.6200572117632891,
  -0.62485948814238634, -0.629638238914927, -0.63439328416364549,
  -0.63912444486377573, -0.64383154288979139, -0.64851440102211244,
  -0.65317284295377676, -0.65780669329707864, -0.66241577759017178,
  -0.66699992230363747, -0.67155895484701833, -0.67609270357531592,
  -0.680600997795453, -0.68508366777270036, -0.68954054473706683,
  -0.69397146088965389, -0.69837624940897292, -0.7027547444572253,
  -0.70710678118654757, -0.71143219574521643, -0.71573082528381859,
  -0.72000250796138165, -0.724247082951467, -0.7284643904482252,
  -0.73265427167241282, -0.73681656887736979, -0.74095112535495922,
  -0.745057785441466, -0.74913639452345937, -0.75318679904361252,
  -0.75720884650648457, -0.76120238548426178, -0.765167265622459,
  -0.7691033376455797, -0.773010453362737, -0.77688846567323244,
  -0.78073722857209449, -0.78455659715557524, -0.78834642762660634,
  -0.79210657730021239, -0.79583690460888357, -0.799537269107905,
  -0.80320753148064494, -0.80684755354379933, -0.81045719825259477,
  -0.81403632970594841, -0.81758481315158371, -0.82110251499110465,
  -0.82458930278502529, -0.8280450452577558, -0.83146961230254524,
  -0.83486287498638, -0.83822470555483808, -0.84155497743689844,
  -0.84485356524970712, -0.84812034480329723, -0.8513551931052652,
  -0.85455798836540053, -0.85772861000027212, -0.86086693863776731,
  -0.8639728561215867, -0.86704624551569265, -0.87008699110871146,
  -0.87309497841829009, -0.8760700941954066, -0.87901222642863353,
  -0.881921264348355, -0.88479709843093779, -0.88763962040285393,
  -0.89044872324475788, -0.89322430119551532, -0.89596624975618522,
  -0.89867446569395382, -0.901348847046022, -0.90398929312344334,
  -0.90659570451491533, -0.90916798309052238, -0.91170603200542988,
  -0.91420975570353069, -0.9166790599210427, -0.91911385169005777,
  -0.9215140393420419, -0.92387953251128674, -0.92621024213831138,
  -0.92850608047321559, -0.93076696107898371, -0.932992798834739,
  -0.93518350993894761, -0.937339011912575, -0.93945922360218992,
  -0.94154406518302081, -0.94359345816196039, -0.94560732538052128,
  -0.94758559101774109, -0.94952818059303667, -0.95143502096900834,
  -0.95330604035419386, -0.95514116830577078, -0.95694033573220882,
  -0.9587034748958716, -0.96043051941556579, -0.96212140426904158,
  -0.96377606579543984, -0.9653944416976894, -0.96697647104485207,
  -0.96852209427441727, -0.970031253194544, -0.97150389098625178,
  -0.97293995220556018, -0.97433938278557586, -0.97570213003852857,
  -0.97702814265775439, -0.97831737071962765, -0.97956976568544052,
  -0.98078528040323043, -0.98196386910955524, -0.98310548743121629,
  -0.984210092386929, -0.98527764238894122, -0.98630809724459867,
  -0.98730141815785843, -0.98825756773074946, -0.989176509964781,
  -0.99005821026229712, -0.99090263542778, -0.99170975366909953,
  -0.99247953459871, -0.9932119492347945, -0.99390697000235606,
  -0.99456457073425542, -0.99518472667219693, -0.99576741446765982,
  -0.996312612182778, -0.99682029929116567, -0.99729045667869021,
  -0.99772306664419164, -0.99811811290014918, -0.99847558057329477,
  -0.99879545620517241, -0.99907772775264536, -0.99932238458834954,
  -0.99952941750109314, -0.99969881869620425, -0.9998305817958234,
  -0.9999247018391445, -0.99998117528260111, -1.0 };

// Function Declarations
static void b_r2br_r2dit_trig_impl(const emxArray_creal_T *x, int xoffInit, int
  unsigned_nRows, const double costab_data[], const double sintab_data[],
  creal_T y_data[], int y_size[1]);
static void bluestein(const emxArray_creal_T *x, int xoffInit, int nfft, int
                      nRows, const double costab_data[], const double
                      sintab_data[], const double costabinv_data[], const double
                      sintabinv_data[], const creal_T wwc_data[], const int
                      wwc_size[1], creal_T y_data[], int y_size[1]);
static void c_r2br_r2dit_trig(const emxArray_creal_T *x, int n1_unsigned, const
  double costab_data[], const double sintab_data[], emxArray_creal_T *y);
static void c_r2br_r2dit_trig_impl(const creal_T x_data[], const int x_size[1],
  int unsigned_nRows, const double costab_data[], const double sintab_data[],
  creal_T y_data[], int y_size[1]);
static void d_r2br_r2dit_trig(const creal_T x_data[], const int x_size[1], int
  n1_unsigned, const double costab_data[], const double sintab_data[], creal_T
  y_data[], int y_size[1]);
static void dobluesteinfft(const emxArray_creal_T *x, int N2, int n1, const
  double costab_data[], const double sintab_data[], const double sintabinv_data[],
  emxArray_creal_T *y);
static void e_r2br_r2dit_trig(const creal_T x_data[], const int x_size[1], int
  n1_unsigned, const double costab_data[], const double sintab_data[], creal_T
  y_data[], int y_size[1]);
static void generate_twiddle_tables(int nRows, boolean_T useRadix2, double
  costab_data[], int costab_size[2], double sintab_data[], int sintab_size[2],
  double sintabinv_data[], int sintabinv_size[2]);
static void get_algo_sizes(int n1, boolean_T useRadix2, int *N2blue, int *nRows);
static void r2br_r2dit_trig_impl(const emxArray_creal_T *x, int xoffInit, const
  double costab[513], const double sintab[513], creal_T y[1024]);

// Function Definitions

//
// Arguments    : const emxArray_creal_T *x
//                int xoffInit
//                int unsigned_nRows
//                const double costab_data[]
//                const double sintab_data[]
//                creal_T y_data[]
//                int y_size[1]
// Return Type  : void
//
static void b_r2br_r2dit_trig_impl(const emxArray_creal_T *x, int xoffInit, int
  unsigned_nRows, const double costab_data[], const double sintab_data[],
  creal_T y_data[], int y_size[1])
{
  int istart;
  int nRowsM2;
  int nRowsD2;
  int nRowsD4;
  int iy;
  cuint8_T b_y_data[2144];
  int ix;
  int ju;
  int i;
  boolean_T tst;
  double temp_re;
  double temp_im;
  double twid_re;
  double twid_im;
  int temp_re_tmp;
  int ihi;
  istart = x->size[0];
  if (istart >= unsigned_nRows) {
    istart = unsigned_nRows;
  }

  nRowsM2 = unsigned_nRows - 2;
  nRowsD2 = unsigned_nRows / 2;
  nRowsD4 = nRowsD2 / 2;
  if (unsigned_nRows > x->size[0]) {
    for (iy = 0; iy < unsigned_nRows; iy++) {
      b_y_data[iy].re = 0U;
      b_y_data[iy].im = 0U;
    }
  }

  y_size[0] = unsigned_nRows;
  for (iy = 0; iy < unsigned_nRows; iy++) {
    y_data[iy].re = 0.0;
    y_data[iy].im = b_y_data[iy].im;
  }

  ix = xoffInit;
  ju = 0;
  iy = 0;
  for (i = 0; i <= istart - 2; i++) {
    y_data[iy] = x->data[ix];
    iy = unsigned_nRows;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  y_data[iy] = x->data[ix];
  if (unsigned_nRows > 1) {
    for (i = 0; i <= nRowsM2; i += 2) {
      temp_re = y_data[i + 1].re;
      temp_im = y_data[i + 1].im;
      twid_re = y_data[i].re;
      twid_im = y_data[i].im;
      y_data[i + 1].re = y_data[i].re - y_data[i + 1].re;
      y_data[i + 1].im = y_data[i].im - y_data[i + 1].im;
      twid_re += temp_re;
      twid_im += temp_im;
      y_data[i].re = twid_re;
      y_data[i].im = twid_im;
    }
  }

  iy = 2;
  ix = 4;
  ju = 1 + ((nRowsD4 - 1) << 2);
  while (nRowsD4 > 0) {
    for (i = 0; i < ju; i += ix) {
      temp_re_tmp = i + iy;
      temp_re = y_data[temp_re_tmp].re;
      temp_im = y_data[i + iy].im;
      y_data[temp_re_tmp].re = y_data[i].re - y_data[temp_re_tmp].re;
      y_data[temp_re_tmp].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }

    istart = 1;
    for (nRowsM2 = nRowsD4; nRowsM2 < nRowsD2; nRowsM2 += nRowsD4) {
      twid_re = costab_data[nRowsM2];
      twid_im = sintab_data[nRowsM2];
      i = istart;
      ihi = istart + ju;
      while (i < ihi) {
        temp_re_tmp = i + iy;
        temp_re = twid_re * y_data[temp_re_tmp].re - twid_im * y_data[i + iy].im;
        temp_im = twid_re * y_data[i + iy].im + twid_im * y_data[i + iy].re;
        y_data[temp_re_tmp].re = y_data[i].re - temp_re;
        y_data[temp_re_tmp].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += ix;
      }

      istart++;
    }

    nRowsD4 /= 2;
    iy = ix;
    ix += ix;
    ju -= iy;
  }
}

//
// Arguments    : const emxArray_creal_T *x
//                int xoffInit
//                int nfft
//                int nRows
//                const double costab_data[]
//                const double sintab_data[]
//                const double costabinv_data[]
//                const double sintabinv_data[]
//                const creal_T wwc_data[]
//                const int wwc_size[1]
//                creal_T y_data[]
//                int y_size[1]
// Return Type  : void
//
static void bluestein(const emxArray_creal_T *x, int xoffInit, int nfft, int
                      nRows, const double costab_data[], const double
                      sintab_data[], const double costabinv_data[], const double
                      sintabinv_data[], const creal_T wwc_data[], const int
                      wwc_size[1], creal_T y_data[], int y_size[1])
{
  int minNrowsNx;
  int fv_size[1];
  int i15;
  int xidx;
  int k;
  double a_re;
  double a_im;
  int tmp_size[1];
  int b_fv_size[1];
  Fmain_ISARimagingTLS *Fmain_ISARimagingTLSThread;
  Fmain_ISARimagingTLSThread = emlrtGetThreadStackData();
  minNrowsNx = x->size[0];
  if (nRows < minNrowsNx) {
    minNrowsNx = nRows;
  }

  fv_size[0] = nRows;
  if (nRows > x->size[0]) {
    fv_size[0] = nRows;
    for (i15 = 0; i15 < nRows; i15++) {
      Fmain_ISARimagingTLSThread->f0.fv_data[i15].re = 0.0;
      Fmain_ISARimagingTLSThread->f0.fv_data[i15].im = 0.0;
    }
  }

  y_size[0] = fv_size[0];
  if (0 <= fv_size[0] - 1) {
    memcpy(&y_data[0], &Fmain_ISARimagingTLSThread->f0.fv_data[0], (unsigned int)
           (fv_size[0] * (int)sizeof(creal_T)));
  }

  xidx = xoffInit;
  for (k = 0; k < minNrowsNx; k++) {
    a_re = wwc_data[(nRows + k) - 1].re;
    a_im = wwc_data[(nRows + k) - 1].im;
    y_data[k].re = a_re * x->data[xidx].re + a_im * x->data[xidx].im;
    y_data[k].im = a_re * x->data[xidx].im - a_im * x->data[xidx].re;
    xidx++;
  }

  i15 = minNrowsNx + 1;
  for (k = i15; k <= nRows; k++) {
    y_data[k - 1].re = 0.0;
    y_data[k - 1].im = 0.0;
  }

  c_r2br_r2dit_trig_impl(y_data, y_size, nfft, costab_data, sintab_data,
    Fmain_ISARimagingTLSThread->f0.fv_data, fv_size);
  d_r2br_r2dit_trig(wwc_data, wwc_size, nfft, costab_data, sintab_data,
                    Fmain_ISARimagingTLSThread->f0.tmp_data, tmp_size);
  b_fv_size[0] = fv_size[0];
  xidx = fv_size[0];
  for (i15 = 0; i15 < xidx; i15++) {
    Fmain_ISARimagingTLSThread->f0.b_fv_data[i15].re =
      Fmain_ISARimagingTLSThread->f0.fv_data[i15].re *
      Fmain_ISARimagingTLSThread->f0.tmp_data[i15].re -
      Fmain_ISARimagingTLSThread->f0.fv_data[i15].im *
      Fmain_ISARimagingTLSThread->f0.tmp_data[i15].im;
    Fmain_ISARimagingTLSThread->f0.b_fv_data[i15].im =
      Fmain_ISARimagingTLSThread->f0.fv_data[i15].re *
      Fmain_ISARimagingTLSThread->f0.tmp_data[i15].im +
      Fmain_ISARimagingTLSThread->f0.fv_data[i15].im *
      Fmain_ISARimagingTLSThread->f0.tmp_data[i15].re;
  }

  e_r2br_r2dit_trig(Fmain_ISARimagingTLSThread->f0.b_fv_data, b_fv_size, nfft,
                    costabinv_data, sintabinv_data,
                    Fmain_ISARimagingTLSThread->f0.fv_data, fv_size);
  xidx = 0;
  i15 = wwc_size[0];
  for (k = nRows; k <= i15; k++) {
    y_data[xidx].re = wwc_data[k - 1].re *
      Fmain_ISARimagingTLSThread->f0.fv_data[k - 1].re + wwc_data[k - 1].im *
      Fmain_ISARimagingTLSThread->f0.fv_data[k - 1].im;
    y_data[xidx].im = wwc_data[k - 1].re *
      Fmain_ISARimagingTLSThread->f0.fv_data[k - 1].im - wwc_data[k - 1].im *
      Fmain_ISARimagingTLSThread->f0.fv_data[k - 1].re;
    xidx++;
  }
}

//
// Arguments    : const emxArray_creal_T *x
//                int n1_unsigned
//                const double costab_data[]
//                const double sintab_data[]
//                emxArray_creal_T *y
// Return Type  : void
//
static void c_r2br_r2dit_trig(const emxArray_creal_T *x, int n1_unsigned, const
  double costab_data[], const double sintab_data[], emxArray_creal_T *y)
{
  int n1;
  int nrows;
  int i11;
  int k;
  int xoff;
  int loop_ub;
  int i12;
  creal_T rwork_data[536];
  int rwork_size[1];
  int l;
  n1 = n1_unsigned;
  nrows = x->size[0];
  i11 = y->size[0] * y->size[1];
  y->size[0] = n1_unsigned;
  y->size[1] = 1024;
  emxEnsureCapacity_creal_T(y, i11);
  if (n1_unsigned > x->size[0]) {
    i11 = y->size[0] * y->size[1];
    y->size[1] = 1024;
    emxEnsureCapacity_creal_T(y, i11);
    for (i11 = 0; i11 < 1024; i11++) {
      loop_ub = y->size[0];
      for (i12 = 0; i12 < loop_ub; i12++) {
        y->data[i12 + y->size[0] * i11].re = 0.0;
        y->data[i12 + y->size[0] * i11].im = 0.0;
      }
    }
  }

#pragma omp parallel for \
 num_threads(omp_get_max_threads()) \
 private(xoff,rwork_data,rwork_size,l)

  for (k = 0; k < 1024; k++) {
    xoff = k * nrows;
    b_r2br_r2dit_trig_impl(x, xoff, n1_unsigned, costab_data, sintab_data,
      rwork_data, rwork_size);
    for (l = 0; l < n1; l++) {
      xoff = 1 + l;
      y->data[(xoff + y->size[0] * k) - 1] = rwork_data[xoff - 1];
    }
  }
}

//
// Arguments    : const creal_T x_data[]
//                const int x_size[1]
//                int unsigned_nRows
//                const double costab_data[]
//                const double sintab_data[]
//                creal_T y_data[]
//                int y_size[1]
// Return Type  : void
//
static void c_r2br_r2dit_trig_impl(const creal_T x_data[], const int x_size[1],
  int unsigned_nRows, const double costab_data[], const double sintab_data[],
  creal_T y_data[], int y_size[1])
{
  int istart;
  int nRowsM2;
  int nRowsD2;
  int nRowsD4;
  int ix;
  int iy;
  int ju;
  int i;
  boolean_T tst;
  double temp_re;
  double temp_im;
  double twid_re;
  double twid_im;
  int temp_re_tmp;
  int ihi;
  istart = x_size[0];
  if (istart >= unsigned_nRows) {
    istart = unsigned_nRows;
  }

  nRowsM2 = unsigned_nRows - 2;
  nRowsD2 = unsigned_nRows / 2;
  nRowsD4 = nRowsD2 / 2;
  y_size[0] = unsigned_nRows;
  if (unsigned_nRows > x_size[0]) {
    y_size[0] = unsigned_nRows;
    for (iy = 0; iy < unsigned_nRows; iy++) {
      y_data[iy].re = 0.0;
      y_data[iy].im = 0.0;
    }
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 0; i <= istart - 2; i++) {
    y_data[iy] = x_data[ix];
    iy = unsigned_nRows;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  y_data[iy] = x_data[ix];
  if (unsigned_nRows > 1) {
    for (i = 0; i <= nRowsM2; i += 2) {
      temp_re = y_data[i + 1].re;
      temp_im = y_data[i + 1].im;
      twid_re = y_data[i].re;
      twid_im = y_data[i].im;
      y_data[i + 1].re = y_data[i].re - y_data[i + 1].re;
      y_data[i + 1].im = y_data[i].im - y_data[i + 1].im;
      twid_re += temp_re;
      twid_im += temp_im;
      y_data[i].re = twid_re;
      y_data[i].im = twid_im;
    }
  }

  iy = 2;
  ix = 4;
  ju = 1 + ((nRowsD4 - 1) << 2);
  while (nRowsD4 > 0) {
    for (i = 0; i < ju; i += ix) {
      temp_re_tmp = i + iy;
      temp_re = y_data[temp_re_tmp].re;
      temp_im = y_data[i + iy].im;
      y_data[temp_re_tmp].re = y_data[i].re - y_data[temp_re_tmp].re;
      y_data[temp_re_tmp].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }

    istart = 1;
    for (nRowsM2 = nRowsD4; nRowsM2 < nRowsD2; nRowsM2 += nRowsD4) {
      twid_re = costab_data[nRowsM2];
      twid_im = sintab_data[nRowsM2];
      i = istart;
      ihi = istart + ju;
      while (i < ihi) {
        temp_re_tmp = i + iy;
        temp_re = twid_re * y_data[temp_re_tmp].re - twid_im * y_data[i + iy].im;
        temp_im = twid_re * y_data[i + iy].im + twid_im * y_data[i + iy].re;
        y_data[temp_re_tmp].re = y_data[i].re - temp_re;
        y_data[temp_re_tmp].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += ix;
      }

      istart++;
    }

    nRowsD4 /= 2;
    iy = ix;
    ix += ix;
    ju -= iy;
  }
}

//
// Arguments    : const creal_T x_data[]
//                const int x_size[1]
//                int n1_unsigned
//                const double costab_data[]
//                const double sintab_data[]
//                creal_T y_data[]
//                int y_size[1]
// Return Type  : void
//
static void d_r2br_r2dit_trig(const creal_T x_data[], const int x_size[1], int
  n1_unsigned, const double costab_data[], const double sintab_data[], creal_T
  y_data[], int y_size[1])
{
  int istart;
  int nRowsM2;
  int nRowsD2;
  int nRowsD4;
  int ix;
  int iy;
  int ju;
  int i;
  boolean_T tst;
  double temp_re;
  double temp_im;
  double twid_re;
  double twid_im;
  int temp_re_tmp;
  int ihi;
  istart = x_size[0];
  if (istart >= n1_unsigned) {
    istart = n1_unsigned;
  }

  nRowsM2 = n1_unsigned - 2;
  nRowsD2 = n1_unsigned / 2;
  nRowsD4 = nRowsD2 / 2;
  y_size[0] = n1_unsigned;
  if (n1_unsigned > x_size[0]) {
    y_size[0] = n1_unsigned;
    for (iy = 0; iy < n1_unsigned; iy++) {
      y_data[iy].re = 0.0;
      y_data[iy].im = 0.0;
    }
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 0; i <= istart - 2; i++) {
    y_data[iy] = x_data[ix];
    iy = n1_unsigned;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  y_data[iy] = x_data[ix];
  if (n1_unsigned > 1) {
    for (i = 0; i <= nRowsM2; i += 2) {
      temp_re = y_data[i + 1].re;
      temp_im = y_data[i + 1].im;
      twid_re = y_data[i].re;
      twid_im = y_data[i].im;
      y_data[i + 1].re = y_data[i].re - y_data[i + 1].re;
      y_data[i + 1].im = y_data[i].im - y_data[i + 1].im;
      twid_re += temp_re;
      twid_im += temp_im;
      y_data[i].re = twid_re;
      y_data[i].im = twid_im;
    }
  }

  iy = 2;
  ix = 4;
  ju = 1 + ((nRowsD4 - 1) << 2);
  while (nRowsD4 > 0) {
    for (i = 0; i < ju; i += ix) {
      temp_re_tmp = i + iy;
      temp_re = y_data[temp_re_tmp].re;
      temp_im = y_data[i + iy].im;
      y_data[temp_re_tmp].re = y_data[i].re - y_data[temp_re_tmp].re;
      y_data[temp_re_tmp].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }

    istart = 1;
    for (nRowsM2 = nRowsD4; nRowsM2 < nRowsD2; nRowsM2 += nRowsD4) {
      twid_re = costab_data[nRowsM2];
      twid_im = sintab_data[nRowsM2];
      i = istart;
      ihi = istart + ju;
      while (i < ihi) {
        temp_re_tmp = i + iy;
        temp_re = twid_re * y_data[temp_re_tmp].re - twid_im * y_data[i + iy].im;
        temp_im = twid_re * y_data[i + iy].im + twid_im * y_data[i + iy].re;
        y_data[temp_re_tmp].re = y_data[i].re - temp_re;
        y_data[temp_re_tmp].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += ix;
      }

      istart++;
    }

    nRowsD4 /= 2;
    iy = ix;
    ix += ix;
    ju -= iy;
  }
}

//
// Arguments    : const emxArray_creal_T *x
//                int N2
//                int n1
//                const double costab_data[]
//                const double sintab_data[]
//                const double sintabinv_data[]
//                emxArray_creal_T *y
// Return Type  : void
//
static void dobluesteinfft(const emxArray_creal_T *x, int N2, int n1, const
  double costab_data[], const double sintab_data[], const double sintabinv_data[],
  emxArray_creal_T *y)
{
  creal_T wwc_data[1071];
  int wwc_size[1];
  int nrows;
  int i13;
  int k;
  int xoff;
  int loop_ub;
  int i14;
  creal_T rwork_data[536];
  int rwork_size[1];
  int l;
  bluesteinSetup(n1, wwc_data, wwc_size);
  nrows = x->size[0];
  i13 = y->size[0] * y->size[1];
  y->size[0] = n1;
  y->size[1] = 1024;
  emxEnsureCapacity_creal_T(y, i13);
  if (n1 > x->size[0]) {
    i13 = y->size[0] * y->size[1];
    y->size[1] = 1024;
    emxEnsureCapacity_creal_T(y, i13);
    for (i13 = 0; i13 < 1024; i13++) {
      loop_ub = y->size[0];
      for (i14 = 0; i14 < loop_ub; i14++) {
        y->data[i14 + y->size[0] * i13].re = 0.0;
        y->data[i14 + y->size[0] * i13].im = 0.0;
      }
    }
  }

#pragma omp parallel for \
 num_threads(omp_get_max_threads()) \
 private(xoff,rwork_data,rwork_size,l)

  for (k = 0; k < 1024; k++) {
    xoff = k * nrows;
    bluestein(x, xoff, N2, n1, costab_data, sintab_data, costab_data,
              sintabinv_data, wwc_data, wwc_size, rwork_data, rwork_size);
    for (l = 0; l < n1; l++) {
      xoff = 1 + l;
      y->data[(xoff + y->size[0] * k) - 1] = rwork_data[xoff - 1];
    }
  }
}

//
// Arguments    : const creal_T x_data[]
//                const int x_size[1]
//                int n1_unsigned
//                const double costab_data[]
//                const double sintab_data[]
//                creal_T y_data[]
//                int y_size[1]
// Return Type  : void
//
static void e_r2br_r2dit_trig(const creal_T x_data[], const int x_size[1], int
  n1_unsigned, const double costab_data[], const double sintab_data[], creal_T
  y_data[], int y_size[1])
{
  int j;
  int nRowsM2;
  int nRowsD2;
  int nRowsD4;
  int ix;
  int iDelta2;
  int ju;
  int iy;
  int i;
  boolean_T tst;
  double temp_re;
  double temp_im;
  double twid_re;
  double twid_im;
  int temp_re_tmp;
  j = x_size[0];
  if (j >= n1_unsigned) {
    j = n1_unsigned;
  }

  nRowsM2 = n1_unsigned - 2;
  nRowsD2 = n1_unsigned / 2;
  nRowsD4 = nRowsD2 / 2;
  y_size[0] = n1_unsigned;
  if (n1_unsigned > x_size[0]) {
    y_size[0] = n1_unsigned;
    for (iDelta2 = 0; iDelta2 < n1_unsigned; iDelta2++) {
      y_data[iDelta2].re = 0.0;
      y_data[iDelta2].im = 0.0;
    }
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 0; i <= j - 2; i++) {
    y_data[iy] = x_data[ix];
    iDelta2 = n1_unsigned;
    tst = true;
    while (tst) {
      iDelta2 >>= 1;
      ju ^= iDelta2;
      tst = ((ju & iDelta2) == 0);
    }

    iy = ju;
    ix++;
  }

  y_data[iy] = x_data[ix];
  if (n1_unsigned > 1) {
    for (i = 0; i <= nRowsM2; i += 2) {
      temp_re = y_data[i + 1].re;
      temp_im = y_data[i + 1].im;
      twid_re = y_data[i].re;
      twid_im = y_data[i].im;
      y_data[i + 1].re = y_data[i].re - y_data[i + 1].re;
      y_data[i + 1].im = y_data[i].im - y_data[i + 1].im;
      twid_re += temp_re;
      twid_im += temp_im;
      y_data[i].re = twid_re;
      y_data[i].im = twid_im;
    }
  }

  iy = 2;
  iDelta2 = 4;
  ix = 1 + ((nRowsD4 - 1) << 2);
  while (nRowsD4 > 0) {
    for (i = 0; i < ix; i += iDelta2) {
      temp_re_tmp = i + iy;
      temp_re = y_data[temp_re_tmp].re;
      temp_im = y_data[i + iy].im;
      y_data[temp_re_tmp].re = y_data[i].re - y_data[temp_re_tmp].re;
      y_data[temp_re_tmp].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }

    ju = 1;
    for (j = nRowsD4; j < nRowsD2; j += nRowsD4) {
      twid_re = costab_data[j];
      twid_im = sintab_data[j];
      i = ju;
      nRowsM2 = ju + ix;
      while (i < nRowsM2) {
        temp_re_tmp = i + iy;
        temp_re = twid_re * y_data[temp_re_tmp].re - twid_im * y_data[i + iy].im;
        temp_im = twid_re * y_data[i + iy].im + twid_im * y_data[i + iy].re;
        y_data[temp_re_tmp].re = y_data[i].re - temp_re;
        y_data[temp_re_tmp].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += iDelta2;
      }

      ju++;
    }

    nRowsD4 /= 2;
    iy = iDelta2;
    iDelta2 += iDelta2;
    ix -= iy;
  }

  if (y_size[0] > 1) {
    twid_re = 1.0 / (double)y_size[0];
    iy = y_size[0];
    for (iDelta2 = 0; iDelta2 < iy; iDelta2++) {
      y_data[iDelta2].re *= twid_re;
      y_data[iDelta2].im *= twid_re;
    }
  }
}

//
// Arguments    : int nRows
//                boolean_T useRadix2
//                double costab_data[]
//                int costab_size[2]
//                double sintab_data[]
//                int sintab_size[2]
//                double sintabinv_data[]
//                int sintabinv_size[2]
// Return Type  : void
//
static void generate_twiddle_tables(int nRows, boolean_T useRadix2, double
  costab_data[], int costab_size[2], double sintab_data[], int sintab_size[2],
  double sintabinv_data[], int sintabinv_size[2])
{
  double e;
  int n;
  double costab1q_data[537];
  int nd2;
  int k;
  int i10;
  e = 6.2831853071795862 / (double)nRows;
  n = nRows / 2 / 2;
  costab1q_data[0] = 1.0;
  nd2 = n / 2 - 1;
  for (k = 0; k <= nd2; k++) {
    costab1q_data[1 + k] = std::cos(e * (1.0 + (double)k));
  }

  i10 = nd2 + 2;
  nd2 = n - 1;
  for (k = i10; k <= nd2; k++) {
    costab1q_data[k] = std::sin(e * (double)(n - k));
  }

  costab1q_data[n] = 0.0;
  if (!useRadix2) {
    i10 = n << 1;
    costab_size[0] = 1;
    costab_size[1] = i10 + 1;
    sintab_size[0] = 1;
    sintab_size[1] = i10 + 1;
    costab_data[0] = 1.0;
    sintab_data[0] = 0.0;
    sintabinv_size[0] = 1;
    sintabinv_size[1] = i10 + 1;
    for (k = 0; k < n; k++) {
      sintabinv_data[1 + k] = costab1q_data[(n - k) - 1];
    }

    nd2 = n + 1;
    for (k = nd2; k <= i10; k++) {
      sintabinv_data[k] = costab1q_data[k - n];
    }

    for (k = 0; k < n; k++) {
      costab_data[1 + k] = costab1q_data[1 + k];
      sintab_data[1 + k] = -costab1q_data[(n - k) - 1];
    }

    nd2 = n + 1;
    for (k = nd2; k <= i10; k++) {
      costab_data[k] = -costab1q_data[i10 - k];
      sintab_data[k] = -costab1q_data[k - n];
    }
  } else {
    nd2 = n << 1;
    costab_size[0] = 1;
    costab_size[1] = nd2 + 1;
    sintab_size[0] = 1;
    sintab_size[1] = nd2 + 1;
    costab_data[0] = 1.0;
    sintab_data[0] = 0.0;
    for (k = 0; k < n; k++) {
      costab_data[1 + k] = costab1q_data[1 + k];
      sintab_data[1 + k] = -costab1q_data[(n - k) - 1];
    }

    i10 = n + 1;
    for (k = i10; k <= nd2; k++) {
      costab_data[k] = -costab1q_data[nd2 - k];
      sintab_data[k] = -costab1q_data[k - n];
    }

    sintabinv_size[0] = 1;
    sintabinv_size[1] = 0;
  }
}

//
// Arguments    : int n1
//                boolean_T useRadix2
//                int *N2blue
//                int *nRows
// Return Type  : void
//
static void get_algo_sizes(int n1, boolean_T useRadix2, int *N2blue, int *nRows)
{
  int n;
  int pmax;
  int pmin;
  boolean_T exitg1;
  int p;
  int pow2p;
  *N2blue = 1;
  if (useRadix2) {
    *nRows = n1;
  } else {
    n = (n1 + n1) - 1;
    pmax = 31;
    if (n <= 1) {
      pmax = 0;
    } else {
      pmin = 0;
      exitg1 = false;
      while ((!exitg1) && (pmax - pmin > 1)) {
        p = (pmin + pmax) >> 1;
        pow2p = 1 << p;
        if (pow2p == n) {
          pmax = p;
          exitg1 = true;
        } else if (pow2p > n) {
          pmax = p;
        } else {
          pmin = p;
        }
      }
    }

    *N2blue = 1 << pmax;
    *nRows = *N2blue;
  }
}

//
// Arguments    : const emxArray_creal_T *x
//                int xoffInit
//                const double costab[513]
//                const double sintab[513]
//                creal_T y[1024]
// Return Type  : void
//
static void r2br_r2dit_trig_impl(const emxArray_creal_T *x, int xoffInit, const
  double costab[513], const double sintab[513], creal_T y[1024])
{
  int ix;
  int ju;
  int iy;
  int i;
  boolean_T tst;
  double temp_re;
  double temp_im;
  double twid_re;
  int iheight;
  double twid_im;
  int istart;
  int temp_re_tmp;
  int j;
  int ihi;
  ix = xoffInit;
  ju = 0;
  iy = 0;
  for (i = 0; i < 1023; i++) {
    y[iy] = x->data[ix];
    iy = 1024;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  y[iy] = x->data[ix];
  for (i = 0; i <= 1022; i += 2) {
    temp_re = y[i + 1].re;
    temp_im = y[i + 1].im;
    twid_re = y[i].re;
    twid_im = y[i].im;
    y[i + 1].re = y[i].re - y[i + 1].re;
    y[i + 1].im = y[i].im - y[i + 1].im;
    twid_re += temp_re;
    twid_im += temp_im;
    y[i].re = twid_re;
    y[i].im = twid_im;
  }

  iy = 2;
  ix = 4;
  ju = 256;
  iheight = 1021;
  while (ju > 0) {
    for (i = 0; i < iheight; i += ix) {
      temp_re_tmp = i + iy;
      temp_re = y[temp_re_tmp].re;
      temp_im = y[i + iy].im;
      y[temp_re_tmp].re = y[i].re - y[temp_re_tmp].re;
      y[temp_re_tmp].im = y[i].im - temp_im;
      y[i].re += temp_re;
      y[i].im += temp_im;
    }

    istart = 1;
    for (j = ju; j < 512; j += ju) {
      twid_re = costab[j];
      twid_im = sintab[j];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re_tmp = i + iy;
        temp_re = twid_re * y[temp_re_tmp].re - twid_im * y[i + iy].im;
        temp_im = twid_re * y[i + iy].im + twid_im * y[i + iy].re;
        y[temp_re_tmp].re = y[i].re - temp_re;
        y[temp_re_tmp].im = y[i].im - temp_im;
        y[i].re += temp_re;
        y[i].im += temp_im;
        i += ix;
      }

      istart++;
    }

    ju /= 2;
    iy = ix;
    ix += ix;
    iheight -= iy;
  }
}

//
// Arguments    : const emxArray_creal_T *x
//                emxArray_creal_T *y
// Return Type  : void
//
void b_r2br_r2dit_trig(const emxArray_creal_T *x, emxArray_creal_T *y)
{
  short sx_idx_1;
  int i5;
  int ub_loop;
  int k;
  int xoff;
  creal_T rwork[1024];
  int xoff_tmp;
  static const double dv5[513] = { 0.0, 0.0061358846491544753,
    0.012271538285719925, 0.01840672990580482, 0.024541228522912288,
    0.030674803176636626, 0.036807222941358832, 0.04293825693494082,
    0.049067674327418015, 0.055195244349689941, 0.061320736302208578,
    0.067443919563664051, 0.073564563599667426, 0.079682437971430126,
    0.0857973123444399, 0.091908956497132724, 0.0980171403295606,
    0.10412163387205459, 0.11022220729388306, 0.11631863091190475,
    0.1224106751992162, 0.12849811079379317, 0.13458070850712617,
    0.14065823933284921, 0.14673047445536175, 0.15279718525844344,
    0.15885814333386145, 0.16491312048996992, 0.17096188876030122,
    0.17700422041214875, 0.18303988795514095, 0.18906866414980619,
    0.19509032201612825, 0.2011046348420919, 0.20711137619221856,
    0.21311031991609136, 0.2191012401568698, 0.22508391135979283,
    0.23105810828067111, 0.2370236059943672, 0.24298017990326387,
    0.24892760574572015, 0.25486565960451457, 0.26079411791527551,
    0.26671275747489837, 0.272621355449949, 0.27851968938505306,
    0.28440753721127188, 0.29028467725446233, 0.29615088824362379,
    0.30200594931922808, 0.30784964004153487, 0.31368174039889152,
    0.31950203081601569, 0.32531029216226293, 0.33110630575987643,
    0.33688985339222005, 0.34266071731199438, 0.34841868024943456,
    0.35416352542049034, 0.35989503653498811, 0.36561299780477385,
    0.37131719395183749, 0.37700741021641826, 0.38268343236508978,
    0.38834504669882625, 0.3939920400610481, 0.39962419984564679,
    0.40524131400498986, 0.41084317105790391, 0.41642956009763715,
    0.42200027079979968, 0.42755509343028208, 0.43309381885315196,
    0.43861623853852766, 0.4441221445704292, 0.44961132965460654,
    0.45508358712634384, 0.46053871095824, 0.46597649576796618,
    0.47139673682599764, 0.47679923006332209, 0.48218377207912272,
    0.487550160148436, 0.49289819222978404, 0.49822766697278181,
    0.50353838372571758, 0.508830142543107, 0.51410274419322166,
    0.51935599016558964, 0.524589682678469, 0.52980362468629461,
    0.53499761988709715, 0.54017147272989285, 0.54532498842204646,
    0.55045797293660481, 0.55557023301960218, 0.560661576197336,
    0.56573181078361312, 0.57078074588696726, 0.57580819141784534,
    0.58081395809576453, 0.58579785745643886, 0.59075970185887416,
    0.59569930449243336, 0.600616479383869, 0.60551104140432555,
    0.61038280627630948, 0.61523159058062682, 0.6200572117632891,
    0.62485948814238634, 0.629638238914927, 0.63439328416364549,
    0.63912444486377573, 0.64383154288979139, 0.64851440102211244,
    0.65317284295377676, 0.65780669329707864, 0.66241577759017178,
    0.66699992230363747, 0.67155895484701833, 0.67609270357531592,
    0.680600997795453, 0.68508366777270036, 0.68954054473706683,
    0.69397146088965389, 0.69837624940897292, 0.7027547444572253,
    0.70710678118654757, 0.71143219574521643, 0.71573082528381859,
    0.72000250796138165, 0.724247082951467, 0.7284643904482252,
    0.73265427167241282, 0.73681656887736979, 0.74095112535495922,
    0.745057785441466, 0.74913639452345937, 0.75318679904361252,
    0.75720884650648457, 0.76120238548426178, 0.765167265622459,
    0.7691033376455797, 0.773010453362737, 0.77688846567323244,
    0.78073722857209449, 0.78455659715557524, 0.78834642762660634,
    0.79210657730021239, 0.79583690460888357, 0.799537269107905,
    0.80320753148064494, 0.80684755354379933, 0.81045719825259477,
    0.81403632970594841, 0.81758481315158371, 0.82110251499110465,
    0.82458930278502529, 0.8280450452577558, 0.83146961230254524,
    0.83486287498638, 0.83822470555483808, 0.84155497743689844,
    0.84485356524970712, 0.84812034480329723, 0.8513551931052652,
    0.85455798836540053, 0.85772861000027212, 0.86086693863776731,
    0.8639728561215867, 0.86704624551569265, 0.87008699110871146,
    0.87309497841829009, 0.8760700941954066, 0.87901222642863353,
    0.881921264348355, 0.88479709843093779, 0.88763962040285393,
    0.89044872324475788, 0.89322430119551532, 0.89596624975618522,
    0.89867446569395382, 0.901348847046022, 0.90398929312344334,
    0.90659570451491533, 0.90916798309052238, 0.91170603200542988,
    0.91420975570353069, 0.9166790599210427, 0.91911385169005777,
    0.9215140393420419, 0.92387953251128674, 0.92621024213831138,
    0.92850608047321559, 0.93076696107898371, 0.932992798834739,
    0.93518350993894761, 0.937339011912575, 0.93945922360218992,
    0.94154406518302081, 0.94359345816196039, 0.94560732538052128,
    0.94758559101774109, 0.94952818059303667, 0.95143502096900834,
    0.95330604035419386, 0.95514116830577078, 0.95694033573220882,
    0.9587034748958716, 0.96043051941556579, 0.96212140426904158,
    0.96377606579543984, 0.9653944416976894, 0.96697647104485207,
    0.96852209427441727, 0.970031253194544, 0.97150389098625178,
    0.97293995220556018, 0.97433938278557586, 0.97570213003852857,
    0.97702814265775439, 0.97831737071962765, 0.97956976568544052,
    0.98078528040323043, 0.98196386910955524, 0.98310548743121629,
    0.984210092386929, 0.98527764238894122, 0.98630809724459867,
    0.98730141815785843, 0.98825756773074946, 0.989176509964781,
    0.99005821026229712, 0.99090263542778, 0.99170975366909953, 0.99247953459871,
    0.9932119492347945, 0.99390697000235606, 0.99456457073425542,
    0.99518472667219693, 0.99576741446765982, 0.996312612182778,
    0.99682029929116567, 0.99729045667869021, 0.99772306664419164,
    0.99811811290014918, 0.99847558057329477, 0.99879545620517241,
    0.99907772775264536, 0.99932238458834954, 0.99952941750109314,
    0.99969881869620425, 0.9998305817958234, 0.9999247018391445,
    0.99998117528260111, 1.0, 0.99998117528260111, 0.9999247018391445,
    0.9998305817958234, 0.99969881869620425, 0.99952941750109314,
    0.99932238458834954, 0.99907772775264536, 0.99879545620517241,
    0.99847558057329477, 0.99811811290014918, 0.99772306664419164,
    0.99729045667869021, 0.99682029929116567, 0.996312612182778,
    0.99576741446765982, 0.99518472667219693, 0.99456457073425542,
    0.99390697000235606, 0.9932119492347945, 0.99247953459871,
    0.99170975366909953, 0.99090263542778, 0.99005821026229712,
    0.989176509964781, 0.98825756773074946, 0.98730141815785843,
    0.98630809724459867, 0.98527764238894122, 0.984210092386929,
    0.98310548743121629, 0.98196386910955524, 0.98078528040323043,
    0.97956976568544052, 0.97831737071962765, 0.97702814265775439,
    0.97570213003852857, 0.97433938278557586, 0.97293995220556018,
    0.97150389098625178, 0.970031253194544, 0.96852209427441727,
    0.96697647104485207, 0.9653944416976894, 0.96377606579543984,
    0.96212140426904158, 0.96043051941556579, 0.9587034748958716,
    0.95694033573220882, 0.95514116830577078, 0.95330604035419386,
    0.95143502096900834, 0.94952818059303667, 0.94758559101774109,
    0.94560732538052128, 0.94359345816196039, 0.94154406518302081,
    0.93945922360218992, 0.937339011912575, 0.93518350993894761,
    0.932992798834739, 0.93076696107898371, 0.92850608047321559,
    0.92621024213831138, 0.92387953251128674, 0.9215140393420419,
    0.91911385169005777, 0.9166790599210427, 0.91420975570353069,
    0.91170603200542988, 0.90916798309052238, 0.90659570451491533,
    0.90398929312344334, 0.901348847046022, 0.89867446569395382,
    0.89596624975618522, 0.89322430119551532, 0.89044872324475788,
    0.88763962040285393, 0.88479709843093779, 0.881921264348355,
    0.87901222642863353, 0.8760700941954066, 0.87309497841829009,
    0.87008699110871146, 0.86704624551569265, 0.8639728561215867,
    0.86086693863776731, 0.85772861000027212, 0.85455798836540053,
    0.8513551931052652, 0.84812034480329723, 0.84485356524970712,
    0.84155497743689844, 0.83822470555483808, 0.83486287498638,
    0.83146961230254524, 0.8280450452577558, 0.82458930278502529,
    0.82110251499110465, 0.81758481315158371, 0.81403632970594841,
    0.81045719825259477, 0.80684755354379933, 0.80320753148064494,
    0.799537269107905, 0.79583690460888357, 0.79210657730021239,
    0.78834642762660634, 0.78455659715557524, 0.78073722857209449,
    0.77688846567323244, 0.773010453362737, 0.7691033376455797,
    0.765167265622459, 0.76120238548426178, 0.75720884650648457,
    0.75318679904361252, 0.74913639452345937, 0.745057785441466,
    0.74095112535495922, 0.73681656887736979, 0.73265427167241282,
    0.7284643904482252, 0.724247082951467, 0.72000250796138165,
    0.71573082528381859, 0.71143219574521643, 0.70710678118654757,
    0.7027547444572253, 0.69837624940897292, 0.69397146088965389,
    0.68954054473706683, 0.68508366777270036, 0.680600997795453,
    0.67609270357531592, 0.67155895484701833, 0.66699992230363747,
    0.66241577759017178, 0.65780669329707864, 0.65317284295377676,
    0.64851440102211244, 0.64383154288979139, 0.63912444486377573,
    0.63439328416364549, 0.629638238914927, 0.62485948814238634,
    0.6200572117632891, 0.61523159058062682, 0.61038280627630948,
    0.60551104140432555, 0.600616479383869, 0.59569930449243336,
    0.59075970185887416, 0.58579785745643886, 0.58081395809576453,
    0.57580819141784534, 0.57078074588696726, 0.56573181078361312,
    0.560661576197336, 0.55557023301960218, 0.55045797293660481,
    0.54532498842204646, 0.54017147272989285, 0.53499761988709715,
    0.52980362468629461, 0.524589682678469, 0.51935599016558964,
    0.51410274419322166, 0.508830142543107, 0.50353838372571758,
    0.49822766697278181, 0.49289819222978404, 0.487550160148436,
    0.48218377207912272, 0.47679923006332209, 0.47139673682599764,
    0.46597649576796618, 0.46053871095824, 0.45508358712634384,
    0.44961132965460654, 0.4441221445704292, 0.43861623853852766,
    0.43309381885315196, 0.42755509343028208, 0.42200027079979968,
    0.41642956009763715, 0.41084317105790391, 0.40524131400498986,
    0.39962419984564679, 0.3939920400610481, 0.38834504669882625,
    0.38268343236508978, 0.37700741021641826, 0.37131719395183749,
    0.36561299780477385, 0.35989503653498811, 0.35416352542049034,
    0.34841868024943456, 0.34266071731199438, 0.33688985339222005,
    0.33110630575987643, 0.32531029216226293, 0.31950203081601569,
    0.31368174039889152, 0.30784964004153487, 0.30200594931922808,
    0.29615088824362379, 0.29028467725446233, 0.28440753721127188,
    0.27851968938505306, 0.272621355449949, 0.26671275747489837,
    0.26079411791527551, 0.25486565960451457, 0.24892760574572015,
    0.24298017990326387, 0.2370236059943672, 0.23105810828067111,
    0.22508391135979283, 0.2191012401568698, 0.21311031991609136,
    0.20711137619221856, 0.2011046348420919, 0.19509032201612825,
    0.18906866414980619, 0.18303988795514095, 0.17700422041214875,
    0.17096188876030122, 0.16491312048996992, 0.15885814333386145,
    0.15279718525844344, 0.14673047445536175, 0.14065823933284921,
    0.13458070850712617, 0.12849811079379317, 0.1224106751992162,
    0.11631863091190475, 0.11022220729388306, 0.10412163387205459,
    0.0980171403295606, 0.091908956497132724, 0.0857973123444399,
    0.079682437971430126, 0.073564563599667426, 0.067443919563664051,
    0.061320736302208578, 0.055195244349689941, 0.049067674327418015,
    0.04293825693494082, 0.036807222941358832, 0.030674803176636626,
    0.024541228522912288, 0.01840672990580482, 0.012271538285719925,
    0.0061358846491544753, 0.0 };

  sx_idx_1 = (short)x->size[1];
  i5 = y->size[0] * y->size[1];
  y->size[0] = 1024;
  y->size[1] = (short)x->size[1];
  emxEnsureCapacity_creal_T(y, i5);
  ub_loop = sx_idx_1 - 1;

#pragma omp parallel for \
 num_threads(omp_get_max_threads()) \
 private(xoff,rwork,xoff_tmp)

  for (k = 0; k <= ub_loop; k++) {
    xoff_tmp = k << 10;
    r2br_r2dit_trig_impl(x, xoff_tmp, dv0, dv5, rwork);
    for (xoff = 0; xoff < 1024; xoff++) {
      y->data[xoff + xoff_tmp] = rwork[xoff];
    }
  }

  i5 = y->size[0] * y->size[1];
  ub_loop = y->size[0] * y->size[1];
  y->size[0] = 1024;
  emxEnsureCapacity_creal_T(y, ub_loop);
  ub_loop = i5 - 1;
  for (i5 = 0; i5 <= ub_loop; i5++) {
    y->data[i5].re *= 0.0009765625;
    y->data[i5].im *= 0.0009765625;
  }
}

//
// Arguments    : const emxArray_creal_T *x
//                int n
//                emxArray_creal_T *y
// Return Type  : void
//
void c_fft(const emxArray_creal_T *x, int n, emxArray_creal_T *y)
{
  boolean_T useRadix2;
  int N2blue;
  int nRows;
  double costab_data[1073];
  int costab_size[2];
  double sintab_data[1073];
  int sintab_size[2];
  double sintabinv_data[1073];
  int sintabinv_size[2];
  useRadix2 = ((n & (n - 1)) == 0);
  get_algo_sizes(n, useRadix2, &N2blue, &nRows);
  generate_twiddle_tables(nRows, useRadix2, costab_data, costab_size,
    sintab_data, sintab_size, sintabinv_data, sintabinv_size);
  if (useRadix2) {
    c_r2br_r2dit_trig(x, n, costab_data, sintab_data, y);
  } else {
    dobluesteinfft(x, N2blue, n, costab_data, sintab_data, sintabinv_data, y);
  }
}

//
// Arguments    : const emxArray_creal_T *x
//                emxArray_creal_T *y
// Return Type  : void
//
void r2br_r2dit_trig(const emxArray_creal_T *x, emxArray_creal_T *y)
{
  short sx_idx_1;
  int ub_loop;
  int k;
  int xoff;
  creal_T rwork[1024];
  int xoff_tmp;
  static const double dv4[513] = { 0.0, -0.0061358846491544753,
    -0.012271538285719925, -0.01840672990580482, -0.024541228522912288,
    -0.030674803176636626, -0.036807222941358832, -0.04293825693494082,
    -0.049067674327418015, -0.055195244349689941, -0.061320736302208578,
    -0.067443919563664051, -0.073564563599667426, -0.079682437971430126,
    -0.0857973123444399, -0.091908956497132724, -0.0980171403295606,
    -0.10412163387205459, -0.11022220729388306, -0.11631863091190475,
    -0.1224106751992162, -0.12849811079379317, -0.13458070850712617,
    -0.14065823933284921, -0.14673047445536175, -0.15279718525844344,
    -0.15885814333386145, -0.16491312048996992, -0.17096188876030122,
    -0.17700422041214875, -0.18303988795514095, -0.18906866414980619,
    -0.19509032201612825, -0.2011046348420919, -0.20711137619221856,
    -0.21311031991609136, -0.2191012401568698, -0.22508391135979283,
    -0.23105810828067111, -0.2370236059943672, -0.24298017990326387,
    -0.24892760574572015, -0.25486565960451457, -0.26079411791527551,
    -0.26671275747489837, -0.272621355449949, -0.27851968938505306,
    -0.28440753721127188, -0.29028467725446233, -0.29615088824362379,
    -0.30200594931922808, -0.30784964004153487, -0.31368174039889152,
    -0.31950203081601569, -0.32531029216226293, -0.33110630575987643,
    -0.33688985339222005, -0.34266071731199438, -0.34841868024943456,
    -0.35416352542049034, -0.35989503653498811, -0.36561299780477385,
    -0.37131719395183749, -0.37700741021641826, -0.38268343236508978,
    -0.38834504669882625, -0.3939920400610481, -0.39962419984564679,
    -0.40524131400498986, -0.41084317105790391, -0.41642956009763715,
    -0.42200027079979968, -0.42755509343028208, -0.43309381885315196,
    -0.43861623853852766, -0.4441221445704292, -0.44961132965460654,
    -0.45508358712634384, -0.46053871095824, -0.46597649576796618,
    -0.47139673682599764, -0.47679923006332209, -0.48218377207912272,
    -0.487550160148436, -0.49289819222978404, -0.49822766697278181,
    -0.50353838372571758, -0.508830142543107, -0.51410274419322166,
    -0.51935599016558964, -0.524589682678469, -0.52980362468629461,
    -0.53499761988709715, -0.54017147272989285, -0.54532498842204646,
    -0.55045797293660481, -0.55557023301960218, -0.560661576197336,
    -0.56573181078361312, -0.57078074588696726, -0.57580819141784534,
    -0.58081395809576453, -0.58579785745643886, -0.59075970185887416,
    -0.59569930449243336, -0.600616479383869, -0.60551104140432555,
    -0.61038280627630948, -0.61523159058062682, -0.6200572117632891,
    -0.62485948814238634, -0.629638238914927, -0.63439328416364549,
    -0.63912444486377573, -0.64383154288979139, -0.64851440102211244,
    -0.65317284295377676, -0.65780669329707864, -0.66241577759017178,
    -0.66699992230363747, -0.67155895484701833, -0.67609270357531592,
    -0.680600997795453, -0.68508366777270036, -0.68954054473706683,
    -0.69397146088965389, -0.69837624940897292, -0.7027547444572253,
    -0.70710678118654757, -0.71143219574521643, -0.71573082528381859,
    -0.72000250796138165, -0.724247082951467, -0.7284643904482252,
    -0.73265427167241282, -0.73681656887736979, -0.74095112535495922,
    -0.745057785441466, -0.74913639452345937, -0.75318679904361252,
    -0.75720884650648457, -0.76120238548426178, -0.765167265622459,
    -0.7691033376455797, -0.773010453362737, -0.77688846567323244,
    -0.78073722857209449, -0.78455659715557524, -0.78834642762660634,
    -0.79210657730021239, -0.79583690460888357, -0.799537269107905,
    -0.80320753148064494, -0.80684755354379933, -0.81045719825259477,
    -0.81403632970594841, -0.81758481315158371, -0.82110251499110465,
    -0.82458930278502529, -0.8280450452577558, -0.83146961230254524,
    -0.83486287498638, -0.83822470555483808, -0.84155497743689844,
    -0.84485356524970712, -0.84812034480329723, -0.8513551931052652,
    -0.85455798836540053, -0.85772861000027212, -0.86086693863776731,
    -0.8639728561215867, -0.86704624551569265, -0.87008699110871146,
    -0.87309497841829009, -0.8760700941954066, -0.87901222642863353,
    -0.881921264348355, -0.88479709843093779, -0.88763962040285393,
    -0.89044872324475788, -0.89322430119551532, -0.89596624975618522,
    -0.89867446569395382, -0.901348847046022, -0.90398929312344334,
    -0.90659570451491533, -0.90916798309052238, -0.91170603200542988,
    -0.91420975570353069, -0.9166790599210427, -0.91911385169005777,
    -0.9215140393420419, -0.92387953251128674, -0.92621024213831138,
    -0.92850608047321559, -0.93076696107898371, -0.932992798834739,
    -0.93518350993894761, -0.937339011912575, -0.93945922360218992,
    -0.94154406518302081, -0.94359345816196039, -0.94560732538052128,
    -0.94758559101774109, -0.94952818059303667, -0.95143502096900834,
    -0.95330604035419386, -0.95514116830577078, -0.95694033573220882,
    -0.9587034748958716, -0.96043051941556579, -0.96212140426904158,
    -0.96377606579543984, -0.9653944416976894, -0.96697647104485207,
    -0.96852209427441727, -0.970031253194544, -0.97150389098625178,
    -0.97293995220556018, -0.97433938278557586, -0.97570213003852857,
    -0.97702814265775439, -0.97831737071962765, -0.97956976568544052,
    -0.98078528040323043, -0.98196386910955524, -0.98310548743121629,
    -0.984210092386929, -0.98527764238894122, -0.98630809724459867,
    -0.98730141815785843, -0.98825756773074946, -0.989176509964781,
    -0.99005821026229712, -0.99090263542778, -0.99170975366909953,
    -0.99247953459871, -0.9932119492347945, -0.99390697000235606,
    -0.99456457073425542, -0.99518472667219693, -0.99576741446765982,
    -0.996312612182778, -0.99682029929116567, -0.99729045667869021,
    -0.99772306664419164, -0.99811811290014918, -0.99847558057329477,
    -0.99879545620517241, -0.99907772775264536, -0.99932238458834954,
    -0.99952941750109314, -0.99969881869620425, -0.9998305817958234,
    -0.9999247018391445, -0.99998117528260111, -1.0, -0.99998117528260111,
    -0.9999247018391445, -0.9998305817958234, -0.99969881869620425,
    -0.99952941750109314, -0.99932238458834954, -0.99907772775264536,
    -0.99879545620517241, -0.99847558057329477, -0.99811811290014918,
    -0.99772306664419164, -0.99729045667869021, -0.99682029929116567,
    -0.996312612182778, -0.99576741446765982, -0.99518472667219693,
    -0.99456457073425542, -0.99390697000235606, -0.9932119492347945,
    -0.99247953459871, -0.99170975366909953, -0.99090263542778,
    -0.99005821026229712, -0.989176509964781, -0.98825756773074946,
    -0.98730141815785843, -0.98630809724459867, -0.98527764238894122,
    -0.984210092386929, -0.98310548743121629, -0.98196386910955524,
    -0.98078528040323043, -0.97956976568544052, -0.97831737071962765,
    -0.97702814265775439, -0.97570213003852857, -0.97433938278557586,
    -0.97293995220556018, -0.97150389098625178, -0.970031253194544,
    -0.96852209427441727, -0.96697647104485207, -0.9653944416976894,
    -0.96377606579543984, -0.96212140426904158, -0.96043051941556579,
    -0.9587034748958716, -0.95694033573220882, -0.95514116830577078,
    -0.95330604035419386, -0.95143502096900834, -0.94952818059303667,
    -0.94758559101774109, -0.94560732538052128, -0.94359345816196039,
    -0.94154406518302081, -0.93945922360218992, -0.937339011912575,
    -0.93518350993894761, -0.932992798834739, -0.93076696107898371,
    -0.92850608047321559, -0.92621024213831138, -0.92387953251128674,
    -0.9215140393420419, -0.91911385169005777, -0.9166790599210427,
    -0.91420975570353069, -0.91170603200542988, -0.90916798309052238,
    -0.90659570451491533, -0.90398929312344334, -0.901348847046022,
    -0.89867446569395382, -0.89596624975618522, -0.89322430119551532,
    -0.89044872324475788, -0.88763962040285393, -0.88479709843093779,
    -0.881921264348355, -0.87901222642863353, -0.8760700941954066,
    -0.87309497841829009, -0.87008699110871146, -0.86704624551569265,
    -0.8639728561215867, -0.86086693863776731, -0.85772861000027212,
    -0.85455798836540053, -0.8513551931052652, -0.84812034480329723,
    -0.84485356524970712, -0.84155497743689844, -0.83822470555483808,
    -0.83486287498638, -0.83146961230254524, -0.8280450452577558,
    -0.82458930278502529, -0.82110251499110465, -0.81758481315158371,
    -0.81403632970594841, -0.81045719825259477, -0.80684755354379933,
    -0.80320753148064494, -0.799537269107905, -0.79583690460888357,
    -0.79210657730021239, -0.78834642762660634, -0.78455659715557524,
    -0.78073722857209449, -0.77688846567323244, -0.773010453362737,
    -0.7691033376455797, -0.765167265622459, -0.76120238548426178,
    -0.75720884650648457, -0.75318679904361252, -0.74913639452345937,
    -0.745057785441466, -0.74095112535495922, -0.73681656887736979,
    -0.73265427167241282, -0.7284643904482252, -0.724247082951467,
    -0.72000250796138165, -0.71573082528381859, -0.71143219574521643,
    -0.70710678118654757, -0.7027547444572253, -0.69837624940897292,
    -0.69397146088965389, -0.68954054473706683, -0.68508366777270036,
    -0.680600997795453, -0.67609270357531592, -0.67155895484701833,
    -0.66699992230363747, -0.66241577759017178, -0.65780669329707864,
    -0.65317284295377676, -0.64851440102211244, -0.64383154288979139,
    -0.63912444486377573, -0.63439328416364549, -0.629638238914927,
    -0.62485948814238634, -0.6200572117632891, -0.61523159058062682,
    -0.61038280627630948, -0.60551104140432555, -0.600616479383869,
    -0.59569930449243336, -0.59075970185887416, -0.58579785745643886,
    -0.58081395809576453, -0.57580819141784534, -0.57078074588696726,
    -0.56573181078361312, -0.560661576197336, -0.55557023301960218,
    -0.55045797293660481, -0.54532498842204646, -0.54017147272989285,
    -0.53499761988709715, -0.52980362468629461, -0.524589682678469,
    -0.51935599016558964, -0.51410274419322166, -0.508830142543107,
    -0.50353838372571758, -0.49822766697278181, -0.49289819222978404,
    -0.487550160148436, -0.48218377207912272, -0.47679923006332209,
    -0.47139673682599764, -0.46597649576796618, -0.46053871095824,
    -0.45508358712634384, -0.44961132965460654, -0.4441221445704292,
    -0.43861623853852766, -0.43309381885315196, -0.42755509343028208,
    -0.42200027079979968, -0.41642956009763715, -0.41084317105790391,
    -0.40524131400498986, -0.39962419984564679, -0.3939920400610481,
    -0.38834504669882625, -0.38268343236508978, -0.37700741021641826,
    -0.37131719395183749, -0.36561299780477385, -0.35989503653498811,
    -0.35416352542049034, -0.34841868024943456, -0.34266071731199438,
    -0.33688985339222005, -0.33110630575987643, -0.32531029216226293,
    -0.31950203081601569, -0.31368174039889152, -0.30784964004153487,
    -0.30200594931922808, -0.29615088824362379, -0.29028467725446233,
    -0.28440753721127188, -0.27851968938505306, -0.272621355449949,
    -0.26671275747489837, -0.26079411791527551, -0.25486565960451457,
    -0.24892760574572015, -0.24298017990326387, -0.2370236059943672,
    -0.23105810828067111, -0.22508391135979283, -0.2191012401568698,
    -0.21311031991609136, -0.20711137619221856, -0.2011046348420919,
    -0.19509032201612825, -0.18906866414980619, -0.18303988795514095,
    -0.17700422041214875, -0.17096188876030122, -0.16491312048996992,
    -0.15885814333386145, -0.15279718525844344, -0.14673047445536175,
    -0.14065823933284921, -0.13458070850712617, -0.12849811079379317,
    -0.1224106751992162, -0.11631863091190475, -0.11022220729388306,
    -0.10412163387205459, -0.0980171403295606, -0.091908956497132724,
    -0.0857973123444399, -0.079682437971430126, -0.073564563599667426,
    -0.067443919563664051, -0.061320736302208578, -0.055195244349689941,
    -0.049067674327418015, -0.04293825693494082, -0.036807222941358832,
    -0.030674803176636626, -0.024541228522912288, -0.01840672990580482,
    -0.012271538285719925, -0.0061358846491544753, -0.0 };

  sx_idx_1 = (short)x->size[1];
  ub_loop = y->size[0] * y->size[1];
  y->size[0] = 1024;
  y->size[1] = (short)x->size[1];
  emxEnsureCapacity_creal_T(y, ub_loop);
  ub_loop = sx_idx_1 - 1;

#pragma omp parallel for \
 num_threads(omp_get_max_threads()) \
 private(xoff,rwork,xoff_tmp)

  for (k = 0; k <= ub_loop; k++) {
    xoff_tmp = k << 10;
    r2br_r2dit_trig_impl(x, xoff_tmp, dv0, dv4, rwork);
    for (xoff = 0; xoff < 1024; xoff++) {
      y->data[xoff + xoff_tmp] = rwork[xoff];
    }
  }
}

//
// File trailer for fft1.cpp
//
// [EOF]
//

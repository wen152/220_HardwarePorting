#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <opencv2/opencv.hpp>
#include "lsd.hpp"
#define pi 3.14159265358979323846
#include "fusion_measure.hpp"
typedef struct  {
      double xx1;
      double yy1;
      double xx2;
      double yy2;
      double k;
  } resultt; 
  
typedef struct {
    int idx;
    double len;
    double deg;
} IterLine;

typedef struct {
    double k;
    double b;
    double x1;
    double y1;
    double x2;
    double y2;
    double len;
} Line;


void sortdescend(double* a, int length, int* b)
{
    double t1, t;
    for (int j = 0; j < length; j++)
        for (int i = 0; i < length - 1 - j; i++)
            if (a[i] < a[i + 1])
            {
                t = a[i];
                a[i] = a[i + 1];
                a[i + 1] = t;


                t1 = b[i];
                b[i] = b[i + 1];
                b[i + 1] = t1;
            }
}

void sortdascend(double* a, int length, int* b)
{
    double t1, t;
    for (int j = 0; j < length; j++)
        for (int i = 0; i < length - 1 - j; i++)
            if (a[i] > a[i + 1])
            {
                t = a[i];
                a[i] = a[i + 1];
                a[i + 1] = t;


                t1 = b[i];
                b[i] = b[i + 1];
                b[i + 1] = t1;
            }
}

void cross(double v1[3], double v2[3], double result[3]) {
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
}


double calculateFai(double x, double y) {
    double fai;

    if (x >= 0 && y > 0)
        fai = atan(x / y);
    else if (x > 0 && y < 0)
        fai = atan(fabs(y) / x) + pi / 2;
    else if (x < 0 && y < 0)
        fai = atan(fabs(x / y)) + pi;
    else if (x < 0 && y > 0)
        fai = atan(y / fabs(x)) + 3.0 / 2.0 * pi;

    return fai;
}

void matrixMultiply(double a[3], double b[3][3], double result[3]) {
    int i, j;

    for (i = 0; i < 3; i++) {
        result[i] = 0;
        for (j = 0; j < 3; j++)
            result[i] += a[j] * b[j][i];
    }
}

Line* linefusion(double out[],double verThre,double latThre,int len1, int len2){
	//static resultt linesInfo[28];
	resultt *linesInfo = (resultt *)malloc(len1 * sizeof(resultt));     
  for (int i = 0; i < len1; i++) 
  {
      linesInfo[i].xx1 = out[7 * i + 0];
      linesInfo[i].yy1 = out[7 * i + 1];
      linesInfo[i].xx2 = out[7 * i + 2];
      linesInfo[i].yy2 = out[7 * i + 3];
      linesInfo[i].k = out[7 * i + 5];
  };
double toleAng = 22.5;
double accLenThre = 15;
int binNum = 60;
int binWid = 180 / binNum;
int binShif = floor(toleAng / binWid);
double degLen[60] = { 0 };  
static IterLine iterLine[22];
//static Line priLinesInfo[36];
Line *priLinesInfo = (Line *)malloc(len1 * sizeof(Line)); // ��̬�����ڴ�
int linesInfoCnt = 0;
float firBinLen = 0;
int firBin = 0;

for (int i = 0; i < len1; i++)     //outopt1=36  opt2=28 
{
    iterLine[i].deg = atan(linesInfo[i].k) * 180 / 3.1415926;
    if (iterLine[i].deg <= 0) {
        iterLine[i].deg += 180;
    }
    iterLine[i].len = sqrt(pow((linesInfo[i].xx2 - linesInfo[i].xx1), 2) + pow((linesInfo[i].yy2 - linesInfo[i].yy1), 2));
    iterLine[i].idx = ceil(iterLine[i].deg/ binWid)-1;
    degLen[iterLine[i].idx] += iterLine[i].len;

};

int priCnt = 0;
while (1) {

    
    for (int i = 0; i < binNum; i++) {
        if (degLen[i] > firBinLen) {
            firBinLen = degLen[i];
            firBin = i;
        } 
    }
    
    if (firBinLen < accLenThre) {
        break;
    }
    int nearFirBin[15]; //int nearFirBin[2 * binShif + 1];
    for (int i = 0; i < 2 * binShif + 1; i++) {
        nearFirBin[i] = (firBin - binShif + i) % (binNum);
    }
 
    double iniDeno = 0;
    double iniNume = 0;
    double degThre = 0.0001;
    double mainOri;

    for (int i = 0; i < len1; i++)  
    {
        if (iterLine[i].idx == firBin) {
            iniDeno += iterLine[i].len;
            iniNume += iterLine[i].len * iterLine[i].deg;
        }
    }
    double aveDeg = iniNume / iniDeno;  
    double curDeg = INFINITY;

    while (fabs(curDeg - aveDeg) > degThre) {
        curDeg = aveDeg;
        iniDeno = 0;
        iniNume = 0;

        for (int i = 0; i < len1; i++) 
        {
            if (iterLine[i].idx == firBin) 
            {
                iniDeno += iterLine[i].len * cos((iterLine[i].deg - aveDeg) * 3.1415926 / 180.0);
                iniNume += iterLine[i].len * cos((iterLine[i].deg - aveDeg) * 3.1415926 / 180.0) * iterLine[i].deg;
            }
        }

        mainOri = iniNume / iniDeno;

    }
    degLen[nearFirBin[0]] = 0;  

    int nearBinNum = binShif * 2 + 1;

    typedef struct {
        Line pri[10];  
    } LinesInfoOut;
    LinesInfoOut linesInfoOut;

    int binIdx;



    for (int i = 0; i < len1; i++)
    {
        double curDeg = atan(linesInfo[i].k) * 180 / 3.1415926;
        if (curDeg <= 0) {
            curDeg += 180;
        }

        double x1 = linesInfo[i].xx1;
        double y1 = linesInfo[i].yy1;
        double x2 = linesInfo[i].xx2;
        double y2 = linesInfo[i].yy2;
        double cenX = (x1 + x2) / 2;
        double cenY = (y1 + y2) / 2;
        x1 = x1 - cenX;
        y1 = y1 - cenY;
        x2 = x2 - cenX;
        y2 = y2 - cenY;
        binIdx = ceil(curDeg / binWid)-1;


        for (int j = 0; j < nearBinNum; j++) {
            
            if (binIdx == nearFirBin[j]) {

                double angDif = mainOri - curDeg;  
                double X1 = x1 * cos(angDif * 3.1415926 / 180) - y1 * sin(angDif * 3.1415926 / 180);
                double Y1 = x1 * sin(angDif * 3.1415926 / 180) + y1 * cos(angDif * 3.1415926 / 180);
                double X2 = x2 * cos(angDif * 3.1415926 / 180) - y2 * sin(angDif * 3.1415926 / 180);
                double Y2 = x2 * sin(angDif * 3.1415926 / 180) + y2 * cos(angDif * 3.1415926 / 180);

 
                X1 = X1 + cenX;
                Y1 = Y1 + cenY;
                X2 = X2 + cenX;
                Y2 = Y2 + cenY;
                double k = (Y2 - Y1) / (X2 - X1);


                priLinesInfo[priCnt].k = k;
                priLinesInfo[priCnt].b = (Y1 + Y2) / 2 - k * (X1 + X2) / 2;
                priLinesInfo[priCnt].x1 = X1;
                priLinesInfo[priCnt].y1 = Y1;
                priLinesInfo[priCnt].x2 = X2;
                priLinesInfo[priCnt].y2 = Y2;
                priLinesInfo[priCnt].len = sqrt(pow((X2 - X1), 2) + pow((Y2 - Y1), 2));
                priCnt++;
               
               
            } 
        }
    };break;

}




int iter1 = 0;
int numLines =priCnt;  
Line tempLinesInfo[priCnt]; 

Line line1;
Line line2;
Line minLine;
Line maxLine;
double ang;
double angDif;
double lenSum;
double maxLineY;
double minLineY;
double curLatDist;
//double verThre=30;   //�Ӻ��� ������ֵ��Ҫ�޸ģ����״�30 
//double latThre=30;//�Ӻ��� ������ֵ��Ҫ�޸ģ���  �״�30 
double maxLineXLeft;
double maxLineXRigh;
double minLineXLeft;
double minLineXRigh;
int coaFlag;
for (int i = 0; i < priCnt; i++)   
{
    ang = atan(priLinesInfo[i].k) * 180 / 3.1415926;
    if (ang <= 0) {
        ang += 180;
    }
    angDif = 0 - ang;
    tempLinesInfo[i].x1 = priLinesInfo[i].x1 * cos(angDif* 3.1415926/180) - priLinesInfo[i].y1 * sin(angDif * 3.1415926 / 180);
    tempLinesInfo[i].y1 = priLinesInfo[i].x1 * sin(angDif * 3.1415926 / 180) + priLinesInfo[i].y1 * cos(angDif * 3.1415926 / 180);
    tempLinesInfo[i].x2 = priLinesInfo[i].x2 * cos(angDif * 3.1415926 / 180) - priLinesInfo[i].y2 * sin(angDif * 3.1415926 / 180);
    tempLinesInfo[i].y2 = priLinesInfo[i].x2 * sin(angDif * 3.1415926 / 180) + priLinesInfo[i].y2 * cos(angDif * 3.1415926 / 180);
    tempLinesInfo[i].len= sqrt(pow((tempLinesInfo[i].x2 - tempLinesInfo[i].x1), 2) + pow((tempLinesInfo[i].y2 - tempLinesInfo[i].y1), 2));
    tempLinesInfo[i].k=priLinesInfo[i].k;
    
};



while (iter1 < numLines) {
    int exLen = numLines; 
    int iter2 = iter1 + 1;
    while (iter2 < numLines) {
        line1 = tempLinesInfo[iter1];
        if (iter1 == iter2) {
            iter2 = iter2 + 1;
            continue;
        }
        line2 = tempLinesInfo[iter2];

        if (line1.len < line2.len) {
            minLine = line1;
            maxLine = line2;
        }
        else {
            minLine = line2;
            maxLine = line1;
        }
        lenSum = maxLine.len + minLine.len;
        maxLineY = (maxLine.y1 + maxLine.y2) / 2;
        minLineY = (minLine.y1 + minLine.y2) / 2;

        curLatDist = fabs(maxLineY - minLineY);

        if (curLatDist > verThre) {
            iter2 = iter2 + 1;
            continue;
        }
        coaFlag = 0;

        maxLineXLeft = maxLine.x1;
        maxLineXRigh = maxLine.x2;
        minLineXLeft = minLine.x1;
        minLineXRigh = minLine.x2;

        if (maxLineXLeft > maxLineXRigh) {
            double temp = maxLineXLeft;
            maxLineXLeft = maxLineXRigh;
            maxLineXRigh = temp;
        }
        if (minLineXLeft > minLineXRigh) {
            double temp = minLineXLeft;
            minLineXLeft = minLineXRigh;
            minLineXRigh = temp;
        }
        if (maxLineXLeft < minLineXLeft) {
            if (maxLineXRigh < minLineXLeft) {
                if ((minLineXLeft - maxLineXRigh) < latThre) {
                    coaFlag = 1;
                }
            }else if (maxLineXRigh < minLineXRigh) {
                coaFlag = 1;
            }else {
                coaFlag = 1;
            }
        }
        else {
            if (minLineXRigh < maxLineXLeft) {
                if ((maxLineXLeft - minLineXRigh) < latThre) {
                    coaFlag = 1;
                }
            }
            else if (minLineXRigh < maxLineXRigh) {
                coaFlag = 1;
            }
            else {
                coaFlag = 1;
            }
        }

        if (coaFlag == 1) {

            double P1 = maxLine.len / lenSum;
            double P2 = minLine.len / lenSum;
            double newX[4] = { maxLine.x1, maxLine.x2, minLine.x1, minLine.x2 };
            double newY[4] = { P1 * maxLineY + P2 * minLineY ,P1 * maxLineY + P2 * minLineY ,P1 * maxLineY + P2 * minLineY ,P1 * maxLineY + P2 * minLineY };
            double minX = INFINITY;
            double maxX = -INFINITY;
            double minY, maxY;
           
            for (int m = 0; m < 4; m++) {
                if (newX[m] > maxX) {
                    maxX = newX[m];
                    maxY = newY[m];
                }
                if (newX[m] < minX) {
                    minX = newX[m];
                    minY = newY[m];
                }
            }
            tempLinesInfo[iter1].x1 = minX;
            tempLinesInfo[iter1].y1 = minY;
            tempLinesInfo[iter1].x2 = maxX;
            tempLinesInfo[iter1].y2 = maxY;
            tempLinesInfo[iter1].len = maxX - minX;
            for (int j = iter2; j < numLines - 1; j++) {
                tempLinesInfo[j] = tempLinesInfo[j + 1];
            }
            numLines--; 
        }
        else{
            iter2 = iter2 + 1;
        }
    }
    int afLen = numLines;
    if (exLen == afLen) {
        iter1++;
    }
}
  

int newnumLines = priCnt;
Line curLine;
for (int i = 0; i < newnumLines; i++) {
    curLine = tempLinesInfo[i];
    
    double angg = atan(curLine.k) * 180 / 3.1415926;
    if (angg < 0) {
        angg = angg + 180;
    }
    double X1 = curLine.x1 * cos(angg* 3.1415926/180) - curLine.y1 * sin(angg * 3.1415926 / 180);
    double Y1 = curLine.x1 * sin(angg * 3.1415926 / 180) + curLine.y1 * cos(angg * 3.1415926 / 180);
    double X2 = curLine.x2 * cos(angg * 3.1415926 / 180) - curLine.y2 * sin(angg * 3.1415926 / 180);
    double Y2 = curLine.x2 * sin(angg * 3.1415926 / 180) + curLine.y2 * cos(angg * 3.1415926 / 180);
    priLinesInfo[i].x1 = X1;
    priLinesInfo[i].y1 = Y1;
    priLinesInfo[i].x2 = X2;
    priLinesInfo[i].y2 = Y2;
    priLinesInfo[i].b = (Y1 + Y2) / 2 - curLine.k * (X1 + X2) / 2;
    priLinesInfo[i].len = sqrt(pow((X2 - X1), 2) + pow((Y2 - Y1), 2));

}
Line linesInfoOut;

double X1 = 0;
double Y1 = 0;
double X2 = 0;
double Y2 = 0;
double K = 0;
double lineOrd[14] = { 0 };  //opt1=14
for (int i = 0; i < priCnt; i++) {
    X1 = priLinesInfo[i].x1;
    X2 = priLinesInfo[i].x2;
    Y1 = priLinesInfo[i].y1;
    Y2 = priLinesInfo[i].y2;
    K = priLinesInfo[i].k;
    lineOrd[i] = priLinesInfo[i].len;
    if (fabs(K) < 1)
    {
        if (X1 < X2)
        {
            priLinesInfo[i].x1 = X1;
            priLinesInfo[i].y1 = Y1;
            priLinesInfo[i].x2 = X2;
            priLinesInfo[i].y2 = Y2;
        }
        else
        {
            priLinesInfo[i].x1 = X2;
            priLinesInfo[i].y1 = Y2;
            priLinesInfo[i].x2 = X1;
            priLinesInfo[i].y2 = Y1;
        }
    }
    else
    {
        if (Y1 < Y2)
        {
            priLinesInfo[i].x1 = X1;
            priLinesInfo[i].y1 = Y1;
            priLinesInfo[i].x2 = X2;
            priLinesInfo[i].y2 = Y2;
        }
        else
        {
            priLinesInfo[i].x1 = X2;
            priLinesInfo[i].y1 = Y2;
            priLinesInfo[i].x2 = X1;
            priLinesInfo[i].y2 = Y1;
        }
    }

}


int b[14] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13}; //opt1=14
sortdescend(lineOrd, priCnt, b);   //opt1=14

//Line priLines[4];
Line* priLines =  new Line[4];
for (int i = 0; i < 4; i++)
{
    priLines[i] = priLinesInfo[b[i]];
}


for (int i = 0; i < 4; i++)
{
    printf("k_%d=%f\t", i, priLines[i].k);
    printf("b_%d=%f\t", i, priLines[i].b);
    printf("x1_%d=%f\t", i, priLines[i].x1);
    printf("y1_%d=%f\t", i, priLines[i].y1);
    printf("x2_%d=%f\t", i, priLines[i].x2);
    printf("y2_%d=%f\t", i, priLines[i].y2);
    printf("len_%d=%f\n", i, priLines[i].len);
}

  return priLines;
}

#define population_size 2000
#define dimension 3 
#define max_epoch 300
#define velocity_max 5
#define velocity_min -5
#define c0 0.9
#define c1 1.49445 
#define c2 1.49445



double population[population_size][dimension];
double velocity[population_size][dimension];
double fitness[population_size];
double pbest[population_size][dimension];
double fitnesspbest[population_size];
double gbest[dimension];
double fitnessgbest;
 
double result[max_epoch];
double fitnessepoch[population_size][dimension];

double maxx(double x1, double x2) {
	if (x1 > x2)
		return x1;
	else
		return x2;
}

double minn(double x1, double x2) {
	if (x1 < x2)
		return x1;
	else
		return x2;
}


double fitting(double x, double a,double b, double r, double u1, double v1, double u2, double v2, double kradarR1[3], double kopticalu1[3], double kopticalv[3], double kopticalu2[3]) {

	double fitness = pow((x * (kradarR1[0]*cos(a)*sin(b) + kradarR1[1]*cos(a)*cos(b) + kradarR1[2]*sin(a))) - r, 2) + pow((x * (kopticalu1[0]*cos(a)*sin(b) + kopticalu1[1]*cos(a)*cos(b) + kopticalu1[2]*sin(a))) - u1, 2) + pow((x * (kopticalv[0]*cos(a)*sin(b) + kopticalv[1]*cos(a)*cos(b) + kopticalv[2]*sin(a))) - v1, 2) + pow((x * (kopticalu2[0]*cos(a)*sin(b) + kopticalu2[1]*cos(a)*cos(b) + kopticalu2[2]*sin(a))) - u2, 2) + pow((x * (kopticalv[0]*cos(a)*sin(b) + kopticalv[1]*cos(a)*cos(b) + kopticalv[2]*sin(a))) - v2, 2);
	return fitness;
}


double* initialization(double r, double u1, double v1, double u2, double v2, double kradarR1[3], double kopticalu1[3], double kopticalv[3], double kopticalu2[3]) {
	int i,j;
	double population_max[3] = {5,3.1415926/2,3.1415926};
	double population_min[3] = {0,-3.1415926 / 2 ,0};
	for (i = 0; i < population_size; i++) {
		for ( j = 0; j < dimension; j++) {

			population[i][j] = ((double)rand() / (RAND_MAX)) * (population_max[j] - population_min[j]) + population_min[j];
			velocity[i][j] = ((double)rand() / (RAND_MAX)) * (velocity_max - velocity_min) + velocity_min;
		}

		fitness[i] = fitting(population[i][0], population[i][1],population[i][2], r, u1, v1, u2, v2,kradarR1,kopticalu1, kopticalv, kopticalu2);
	}
}


double* max_in_community(double fit[], int size) {
	static double result[2];
	int i;
	result[0] = *fit;
	result[1] = 0;
	for ( i = 0; i < size; i++)
		if (fit[i] <result[0]) {
			result[0] = fit[i];
			result[1] = i;
		}

	return result;
}

double* PSO_search(double r, double u1, double v1, double u2, double v2, double kradarR1[3], double kopticalu1[3], double kopticalv[3], double kopticalu2[3]) {


	int i,j,k;
	double population_max[3] = { 5,3.1415926 / 2,3.1415926 };
	double population_min[3] = { 0,-3.1415926 / 2 ,0 };
	//��������������������Step1��������������������// 

	initialization(r,u1,v1,u2,v2, kradarR1, kopticalu1, kopticalv, kopticalu2);
	double* best_fitness;
	best_fitness = max_in_community(fitness, population_size);
	int b = (int)best_fitness[1];
	for ( i = 0; i < dimension; i++)
		gbest[i] = population[b][i];

	//��������������������Step2��������������������// 
	for ( i = 0; i < population_size; i++)
		for ( j = 0; j < dimension; j++)
			pbest[i][j] = population[i][j];
	for ( i = 0; i < population_size; i++)
		fitnesspbest[i] = fitness[i];
	fitnessgbest = best_fitness[0];

	//��������������������Step3��������������������// 
	for ( i = 0; i < max_epoch; i++) {
		for ( j = 0; j < population_size; j++) {
			for ( k = 0; k < dimension; k++) {

				double r1 = (double)rand() / RAND_MAX;
				double r2 = (double)rand() / RAND_MAX;
				velocity[j][k] = c0*velocity[j][k] + c1 * r1 * (pbest[j][k] - population[j][k]) + c2 * r2 * (gbest[k] - population[j][k]);

				velocity[j][k] = minn(velocity[j][k], velocity_max);
				velocity[j][k] = maxx(velocity[j][k], velocity_min);

				population[j][k] += velocity[j][k];
				population[j][k] = minn(population[j][k], population_max[k]);
				population[j][k] = maxx(population[j][k], population_min[k]);
			}

			fitness[j] = fitting(population[j][0], population[j][1],population[j][2], r, u1, v1, u2, v2, kradarR1, kopticalu1, kopticalv, kopticalu2);
		}
		for ( j = 0; j < population_size; j++) {
			if (fitness[j] < fitnesspbest[j]) {
				for ( k = 0; k < dimension; k++)
					pbest[j][k] = population[j][k];
				fitnesspbest[j] = fitness[j];
			}
			if (fitness[j] < fitnessgbest) {
				for ( k = 0; k < dimension; k++)
					gbest[k] = population[j][k];
				fitnessgbest = fitness[j];
			}
		}
		for ( k = 0; k < dimension; k++)
			fitnessepoch[i][k] = gbest[k];
		result[i] = fitnessgbest;
	}
	static double final_position[3];
	for (int k = 0; k < dimension; k++)
		final_position[k] = fitnessepoch[max_epoch - 1][k];
	return final_position;
}

int Fusion_Measure() {

    clock_t start, end;
    // cv::Mat image = cv::imread("test.png");
    // int row = image.rows;
    // int col = image.cols;
    // double *img = (double *) malloc(row*col*sizeof(double));
    // for(int x = 0; x<row; x++)
    //     for(int y = 0; y<col; y++)
    //         img[x *col + y ] = image.at<int>(x,y);
    //         //img[x *col + y ] = image.at<int>(x)(y);
    // if( img == NULL )
    // {
    //   fprintf(stderr,"error: not enough memory\n");
    //   exit(EXIT_FAILURE);
    // }
    // double* out;
    // int n;
    // start = clock();
    // out = lsd(&n, img, col, row);
    // end = clock();   

//out = lsd(&n,image,X,Y);   //����ͼƬ������ 
double outradar[154] = {
705.408107467036,	478.507925245044,	705.769510688481,	448.497224337414,	30.0128769240091, - 83.0393840642253,	30.0107009076304,
642.712631991841,	423.668999639062,	654.804845043193,	480.006526496162,	57.6206434262392,	4.65899224714736,- 56.3375268571006,
554.674089285567,	98.4447534281120,	555.386637133555,	113.587680223148,	15.1596819345096,	21.2518034231613, - 15.1429267950362,
640.974217402480,	410.976716675194,	640.790943476587,	421.018997457025,	10.0439530381737, - 54.7938324174127, - 10.0422807818307,
615.559588384646,	130.773876988326,	595.975696741718,	60.6576553667362,	72.7998169391232,	3.58030073388958,	70.1162216215898,
556.175258494676,	114.306098235530,	578.504171389670,	185.998700806035,	75.0893442134406,	3.21075203739892, - 71.6926025705047,
618.550131415706,	151.197314255013,	615.249060088149,	131.123489481436,	20.3434390639800,	6.08100303862048,	20.0738247735763,
678.725375299319,	403.448434405244,	674.734030675600,	386.003653762027,	17.8955637965178,	4.37065257145380,	17.4447806432167,
657.204180142301,	482.256642745236,	660.653658592298,	506.050210931495,	24.0423124638860,	6.89772918751855, - 23.7935681862586,
571.268828082279,	72.4401708245048,	553.645415650003,	97.3528680256263,	30.5160146086096, - 1.41361369694190, - 24.9126972011216,
694.486581143664,	424.920128056676,	679.247471583236,	401.324556047951,	28.0888141190335,	1.54835634688240,	23.5955720087244,
589.742087381002,	193.864987098091,	619.262099126453,	151.906734051777,	51.3023010414741, - 1.42134946991610,	41.9582530463136,
704.149968853213,	448.193849798420,	693.719947013543,	427.750061711215,	22.9506824937858,	1.96009063082204,	20.4437880872059,
592.348037654438,	61.6741531838177,	583.423220870699,	62.9720304686425,	9.01869392261333, - 0.145423409384652, - 1.29787728482476,
673.207173315073,	508.193014966732,	704.582887385238,	478.264369128292,	43.3608034419971, - 0.953879353040738,	29.9286458384400,
582.927277852115,	62.9508807753198,	572.758996156081,	72.7767345143430,	14.1400620348911, - 0.966323911232329, - 9.82585373902323,
663.757338215567,	387.694422635884,	652.621904096044,	394.142277705608,	12.8675066749707, - 0.579039398061688, - 6.44785506972437,
671.660638483831,	384.588035730602,	665.234777127487,	388.489719456941,	7.51763460613398, - 0.607184548494314, - 3.90168372633917,
579.378373290432,	186.175003847732,	582.722932190848,	193.277290084421,	7.85038496038263,	2.12353450728779, - 7.10228623668846,
653.545773486771,	396.077034056143,	639.995153689989,	409.988789734496,	19.4205108823419, - 1.02665087553091, - 13.9117556783535,
663.446681073225,	508.051480680377,	672.266468409843,	507.774615065876,	8.82413186844249, - 0.0313914161344921,	0.276865614501503,
584.244043622600,	194.144293486131,	589.849762330693,	192.615900332864,	5.81034145857333, - 0.272648920300051,	1.52839315326730,
  };
  double outopt1[252] = {
488.915630842442,	449.047877185073,	482.818228897135,	425.850640829380,	23.9852055447631,	3.80444598597427,	23.1972363556935,
465.076366284101,	430.816770389340,	462.548530367719,	443.559222180942,	12.9907671860234,	-5.04085400046196,	-12.7424517916025,
425.810115932087,	586.392362580084,	450.961002853077,	580.823864040628,	25.7599551415827,	-0.221403664886617,	5.56849853945607,
337.246249209459,	584.324356205257,	419.738853979585,	584.699694535605,	82.4934586535349,	0.00454996337422283,	-0.375338330348313,
463.213968460010,	444.217445132174,	468.086811238167,	453.037694523395,	10.0767750825334,	1.81008290083942,	-8.82024939122073,
572.236378670064,	479.690611181610,	510.967002115986,	480.327985184871,	61.2726917063827,	-0.0104028152253049,	-0.637374003261698,
453.504175715632,	519.507847464585,	374.745169697400,	519.834161935897,	78.7596820087158,	-0.00414320200076389,	-0.326314471312912,
623.495421710712,	586.639291210475,	712.258773057567,	587.274970684882,	88.7656275295759,	0.00716150826620321,	-0.635679474406402,
597.327297401119,	586.701959371194,	621.089534472941,	585.760977095877,	23.7808611765828,	-0.0395999026721526,	0.940982275316742,
450.733298108219,	579.243016592960,	465.108237579406,	571.680986952600,	16.2426345486798,	-0.526056450917027,	7.56202964035970,
475.074551027988,	603.738177596216,	490.899252452798,	608.814091210917,	16.6188469578455,	0.320758886909761,	-5.07591361470134,
505.976161151325,	621.407597669836,	562.250475880430,	622.216910386385,	56.2801340199499,	0.0143815650256193,	-0.809312716548789,
748.504651798173,	518.460137343748,	660.996334804832,	518.796521499810,	87.5089635255010,	-0.00384402497522630,	-0.336384156062422,
476.494494505172,	423.178700655638,	465.136700155014,	429.226254151949,	12.8674937649574,	-0.532458443062656,	-6.04755349631085,
471.744642066237,	453.802532491207,	483.163209000539,	447.712124810833,	12.9412803287112,	-0.533377587171420,	6.09040768037340,
483.001255711822,	447.048263706398,	506.036977805953,	485.477131547860,	44.8042673857592,	1.66822935632017,	-38.4288678414623,
511.228043300371,	482.577545451158,	490.112226545827,	448.743800986353,	39.8823266838992,	1.60229390404822,	33.8337444648056,
609.210274033655,	531.818226088256,	580.031617168639,	483.818933531480,	56.1722894530512,	1.64501377766724,	47.9992925567765,
646.062787520599,	517.467227565271,	623.630384906592,	523.951095942903,	23.3506581529375,	-0.289040299837679,	-6.48386837763201,
474.785216652319,	519.034002878335,	457.032683732810,	520.204754582052,	17.7910956551297,	-0.0659484316421116,	-1.17075170371652,
473.492153073857,	593.500977907198,	474.719323674832,	603.348044269040,	9.92323856502619,	8.02420328030930,	-9.84706636184228,
572.370110506506,	627.818518637796,	597.720551510840,	585.787733153641,	49.0839259588037,	-1.65799030782025,	42.0307854841553,
712.756013302097,	588.063168456830,	751.070784576682,	522.807624054907,	75.6723712659727,	-1.70314325862121,	65.2555444019235,
490.891642915106,	609.411261708488,	494.947045780196,	615.456380429689,	7.27940607134900,	1.49063334083019,	-6.04511872120099,
623.698069153996,	525.123049308981,	610.322823873578,	532.224586433960,	15.1436130381355,	-0.530946309850224,	-7.10153712497925,
654.804012390260,	518.951514640261,	647.224170296846,	518.827395432624,	7.58085824552809,	0.0163749067735419,	0.124119207637818,
374.087654100486,	518.883717367091,	333.718155030598,	580.708653368295,	73.8377895572897,	-1.53147642219124,	-61.8249360012038,
466.108932964214,	570.518399150100,	475.788472472962,	584.892443490962,	17.3293576284428,	1.48499257923071,	-14.3740443408615,
500.665861174060,	479.538749728620,	476.024048094283,	518.515203759782,	46.1127197278794,	-1.58172022103146,	-38.9764540311617,
579.102916490940,	476.075097687918,	571.946884964634,	479.331076063126,	7.86194520365800,	-0.454997768419408,	-3.25597837520849,
464.730614691129,	593.114157455196,	480.939699581382,	592.299788689109,	16.2295295516110,	-0.0502415017011463,	0.814368766087682,
476.296469474593,	519.122818036964,	452.103841672682,	555.903864780205,	44.0241824397710,	-1.52034111566560,	-36.7810467432404,
488.913447048388,	597.501454191248,	496.825696369769,	584.491942948681,	15.2266566288918,	-1.64422412820216,	13.0095112425668,
579.852701177818,	560.927428271974,	595.279588871072,	582.759064079169,	26.7321751063821,	1.41516787062246,	-21.8316358071947,
590.983349972408,	582.261411828249,	577.262008240419,	562.241769624616,	24.2705849308893,	1.45901491229246,	20.0196422036330,
503.369506104458,	592.471081023810,	516.849860825361,	600.427910472581,	15.6534692090347,	0.590253714646860,	-7.95682944877046,

  };
  double outopt2[196] = {
493.013597269959,	529.116233241830,	481.001055247584,	528.931413236703,	12.0139637203394,	0.0153855865629672,	0.184820005126539,
477.000436939254,	528.971041760214,	423.000022034311,	528.948110896650,	54.0004197736506,	0.000424642358853151,	0.0229308635642838,
518.000837007997,	583.227260444125,	556.004004539105,	583.087293476850,	38.0034252818003, -0.00368303424079278,	0.139966967275655,
604.023960888665,	528.970376236513,	592.981308999190,	529.243045478126,	11.0460178013371, -0.0246923695813408, -0.272669241613016,
413.998454025158,	564.163160527587,	467.000120534166,	564.065245400867,	53.0017569529931, -0.00184739713238363,	0.0979151267202951,
471.000157427471,	564.048819292113,	482.000043043215,	564.013347983584,	10.9999428077235, -0.00322469794398564,	0.0354713085291678,
661.000035211224,	529.074321812167,	606.999573564888,	529.099905449927,	54.0004677066771, -0.000473767019389420, -0.0255836377598371,
540.046751779812,	480.030270837538,	535.376254859437,	487.243617457320,	8.59336435514900, -1.54444949707909, -7.21334661978182,
561.820636336482,	584.939854345467,	573.482672942160,	550.161853741511,	36.6812271305214, -2.98215498543561,	34.7780006039554,
651.132883982586,	563.047560890661,	662.330916960406,	531.760526553538,	33.2306253354439, -2.79397590622339,	31.2870343371229,
573.240260753171,	540.919223266891,	563.464936038393,	511.843686437381,	30.6748075006771,	2.97438066538632,	29.0755368295107,
553.971481030498,	508.664391435234,	547.776461753198,	490.731762940906,	18.9725440719400,	2.89468485756569,	17.9326284943280,
545.502050619381,	492.152386562697,	550.127547376209,	507.266995123662,	15.8065370084218,	3.26767250212748, -15.1146085609647,
581.993364427954,	563.681551687052,	594.001962119566,	563.798467126734,	12.0091668211840,	0.00973597772903113, -0.116915439682430,
597.999343739041,	563.804720788350,	650.002117057626,	563.629958766223,	52.0030669729063 ,-0.00336062888523891,	0.174762022126970,
421.930199817271,	529.978654244491,	412.125903299179,	562.038502779463,	33.5254846094733, -3.26997949070710, -32.0598485349722,
592.320020758072,	529.620746382282,	573.619380424940,	539.261709691370,	21.0395371240991, -0.515541881847078, -9.64096330908819,
481.692520267055,	564.404255808524,	503.573389314296,	553.110945915605,	24.6233888528774, -0.516127118558987,	11.2933098929190,
572.318207550597,	555.833378197160,	581.521449780426,	563.362614123549,	11.8907132324404,	0.818106895196778 ,-7.52923592638911,
504.196225111726,	553.938615347629,	509.201001090865,	569.937121289011,	16.7630538672943,	3.19664776366964, -15.9985059413823,
509.390179828812,	570.191125850176,	512.512317131822,	580.152846375375,	10.4395218837370,	3.19067342605196, -9.96172052519933,
511.228570293804,	543.722291687092,	492.647158341983,	528.428695522681,	24.0658046564694,	0.823058882934455,	15.2935961644106,
514.172644122480,	508.056059894443,	504.795694860616,	536.933659342794,	30.3618663352393 ,-3.07963695247824, -28.8775994483503,
549.001688223594,	508.464749793586,	518.998832616163,	508.370118295804,	30.0030048448600,	0.00315408303196729,	0.0946314977819611,
516.764306805934,	511.077419107082,	526.632726362220,	541.120639873394,	31.6224733149100,	3.04438016593817, -30.0432207663113,
517.671093427856,	570.886201555258,	514.153449183772,	581.053091911014,	10.7582285035172 ,-2.89025542388349, -10.1668903557555,
526.107826989837,	545.035587505231,	517.881422605187,	569.960864402645,	26.2477267113278 ,-3.02991145729749, -24.9252768974139,
525.338156840146,	541.889749343806,	515.926885743011,	513.023837739928,	30.3613714510180,	3.06716396817699,	28.8659116038783,

  };
// project_3platform
//�״�
int q1;
int q2;
int q3;
int q4;
int m1;
int m2;
int m3;
int m4;
int n1;
int n2;
int n3;
int n4;

Line* radar=linefusion(outradar,30,30,22,12);
double x1_rad[4] = { 0 };
double y1_rad[4] = { 0 };
double x2_rad[4] = { 0 };
double y2_rad[4] = { 0 };
double deltx_rad[6] = { 0 };
double delty_rad[6] = { 0 };
for (int i = 0; i < 4; i++)
{
    x1_rad[i] = radar[i].x1;
    y1_rad[i] = radar[i].y1;
    x2_rad[i] = radar[i].x2;
    y2_rad[i] = radar[i].y2;
    deltx_rad[i] = radar[i].x1- radar[i].x2; 
    delty_rad[i] = radar[i].y1 - radar[i].y2; 

}

if (fabs(radar[0].k) < 1)
{
    int b[4] = { 0,1,2,3 };
    int bb[4] = { 0,1,2,3 };
    sortdescend(x2_rad, 4, b);
    q1 = b[0];
    q2 = b[1];
    sortdascend(x1_rad, 4, bb);
    q3 = bb[0];
    q4 = bb[1];
}
else
{
    int b[4] = { 0,1,2,3 };
    int bb[4] = { 0,1,2,3 };
    sortdescend(y2_rad, 4, b);
    q1 = b[0];
    q2 = b[1];
    sortdascend(y1_rad, 4, bb);
    q3 = bb[0];
    q4 = bb[1];
};

for (int i = 0; i < 4; i++)
{
    x1_rad[i] = radar[i].x1;
    y1_rad[i] = radar[i].y1;
    x2_rad[i] = radar[i].x2;
    y2_rad[i] = radar[i].y2;
}

deltx_rad[4] = x1_rad[q3] - x1_rad[q4]; 
deltx_rad[5] = x2_rad[q1] - x2_rad[q2];
delty_rad[4] = y1_rad[q3] - y1_rad[q4]; 
delty_rad[5] = y2_rad[q1] - y2_rad[q2]; 

   
//��ѧI
Line* opt1=linefusion(outopt1,0,0,36,14);
double x1_opt1[4] = { 0 };
double y1_opt1[4] = { 0 };
double x2_opt1[4] = { 0 };
double y2_opt1[4] = { 0 };
double deltx_opt1[6] = { 0 };
double delty_opt1[6] = { 0 };
for (int i = 0; i < 4; i++)
{
    x1_opt1[i] = opt1[i].x1;
    y1_opt1[i] = opt1[i].y1;
    x2_opt1[i] = opt1[i].x2;
    y2_opt1[i] = opt1[i].y2;
    deltx_opt1[i] = opt1[i].x1 - opt1[i].x2;  
    delty_opt1[i] = opt1[i].y1 - opt1[i].y2; 

}

if (fabs(opt1[1].k) < 1)
{
    int b[4] = { 0,1,2,3 };
    int bb[4] = { 0,1,2,3 };
    sortdescend(x2_opt1, 4, b);
    m1 = b[0];
    m2 = b[1];
    sortdascend(x1_opt1, 4, bb);
    m3 = bb[0];
    m4 = bb[1];
}
else
{
    int b[4] = { 0,1,2,3 };
    int bb[4] = { 0,1,2,3 };
    sortdescend(y2_opt1, 4, b);
    m1 = b[0];
    m2 = b[1];
    sortdascend(y1_opt1, 4, bb);
    m3 = bb[0];
    m4 = bb[1];
}
for (int i = 0; i < 4; i++)
{
    x1_opt1[i] = opt1[i].x1;
    y1_opt1[i] = opt1[i].y1;
    x2_opt1[i] = opt1[i].x2;
    y2_opt1[i] = opt1[i].y2;
}
deltx_opt1[4] = x1_opt1[m3] - x1_opt1[m4];
deltx_opt1[5] = x2_opt1[m1] - x2_opt1[m2];
delty_opt1[4] = y1_opt1[m3] - y1_opt1[m4];
delty_opt1[5] = y2_opt1[m1] - y2_opt1[m2];

//��ѧ2
Line* opt2=linefusion(outopt2,0,0,28,14);
start = clock();
double x1_opt2[4] = { 0 };
double y1_opt2[4] = { 0 };
double x2_opt2[4] = { 0 };
double y2_opt2[4] = { 0 };
double deltx_opt2[6] = { 0 };
double delty_opt2[6] = { 0 };
for (int i = 0; i < 4; i++)
{
    x1_opt2[i] = opt2[i].x1;
    y1_opt2[i] = opt2[i].y1;
    x2_opt2[i] = opt2[i].x2;
    y2_opt2[i] = opt2[i].y2;
    deltx_opt2[i] = opt2[i].x1 - opt2[i].x2;  
    delty_opt2[i] = opt2[i].y1 - opt2[i].y2; 
}

if (fabs(opt2[1].k) < 1)
{
    int b[4] = { 0,1,2,3 };
    int bb[4] = { 0,1,2,3 };
    sortdescend(x2_opt2, 4, b);
    n1 = b[0];
    n2 = b[1];
    sortdascend(x1_opt2, 4, bb);
    n3 = bb[0];
    n4 = bb[1];
}
else
{
    int b[4] = { 0,1,2,3 };
    int bb[4] = { 0,1,2,3 };
    sortdescend(y2_opt2, 4, b);
    n1 = b[0];
    n2 = b[1];
    sortdascend(y1_opt2, 4, bb);
    n3 = bb[0];
    n4 = bb[1];
}
for (int i = 0; i < 4; i++)
{
    x1_opt2[i] = opt2[i].x1;
    y1_opt2[i] = opt2[i].y1;
    x2_opt2[i] = opt2[i].x2;
    y2_opt2[i] = opt2[i].y2;
}
deltx_opt2[4]= x1_opt2[n3] - x1_opt2[n4];
deltx_opt2[5]= x2_opt2[n1] - x2_opt2[n2];
delty_opt2[4]= y1_opt2[n3] - y1_opt2[n4];
delty_opt2[5]= y2_opt2[n1] - y2_opt2[n2];
/**/

double	c_long[2][6];
double	c_wide[2][6];
c_long[0][0] = (deltx_opt1[0] + deltx_opt1[1]) / 2;     
c_long[1][0] = -((deltx_opt1[0] + deltx_opt1[1]) / 2);   

c_long[0][1] = (delty_opt1[0] + delty_opt1[1]) / 2;     
c_long[1][1] = -((delty_opt1[0] + delty_opt1[1]) / 2);

c_long[0][2] = (delty_rad[0] + delty_rad[1]) / 2;      
c_long[1][2] = -((delty_rad[0] + delty_rad[1]) / 2);

c_long[0][3] = (deltx_rad[0] + deltx_rad[1]) / 2;              
c_long[1][3] = -((deltx_rad[0] + deltx_rad[1]) / 2);

c_long[0][4] = (deltx_opt2[0] + deltx_opt2[1]) / 2;           
c_long[1][4] = -((deltx_opt2[0] + deltx_opt2[1]) / 2);

c_long[0][5] = (delty_opt2[0] + delty_opt2[1]) / 2;
c_long[1][5] = -((delty_opt2[0] + delty_opt2[1]) / 2);              

c_wide[0][0] = (fabs(deltx_opt1[4]) + fabs(deltx_opt1[5])) / 2;
c_wide[1][0] = -((fabs(deltx_opt1[4]) + fabs(deltx_opt1[5])) / 2);

c_wide[0][1] = (fabs(delty_opt1[4]) + fabs(delty_opt1[5])) / 2;
c_wide[1][1] = -((fabs(delty_opt1[4]) + fabs(delty_opt1[5])) / 2);

c_wide[0][2] = (fabs(delty_rad[4]) + fabs(delty_rad[5])) / 2;
c_wide[1][2] = -((fabs(delty_rad[4]) + fabs(delty_rad[5])) / 2);

c_wide[0][3] = (fabs(deltx_rad[4]) + fabs(deltx_rad[5])) / 2;
c_wide[1][3] = -((fabs(deltx_rad[4]) + fabs(deltx_rad[5])) / 2);

c_wide[0][4] = (fabs(deltx_opt2[4]) + fabs(deltx_opt2[5])) / 2;
c_wide[1][4] = -((fabs(deltx_opt2[4]) + fabs(deltx_opt2[5])) / 2);

c_wide[0][5] = (fabs(delty_opt2[4]) + fabs(delty_opt2[5])) / 2;
c_wide[1][5] = -((fabs(delty_opt2[4]) + fabs(delty_opt2[5])) / 2); 


double T_X[3] = {1, 0, 0};
double T_Y[3] = {0, 1, 0};
double T_Z[3] = {0, 0, 1};
double V1[3] = {0, 0.707, -0.707};
double V2[3] = {-1, 0, 0};

// ������
double sun[3];
cross(V1, V2, sun);

double tar_x1 = 3.481467; // Get from table
double tar_y1 = 2.849464; // Get from table
double opt1_x1 = 3.640551;
double opt1_y1 = -4.595559;
double opt2_x1 = 3.687914;
double opt2_y1 = 10.495424;
double wuju1 = sqrt(pow(opt1_x1, 2) + pow(opt1_y1, 2)) * 10000; 
double wuju2 = sqrt(pow(opt2_x1, 2) + pow(opt2_y1, 2)) * 10000; 
double jiaoju = 150; // mm
double W_u = 1080;
double W_v = 1080;
double w_u = 36;
double w_v = 36;
double lamda = 0.03;
double fbl_r = 0.0417/4;
double fbl_fa = 1 / 102.4;

double fai1 = calculateFai(tar_x1, tar_y1);

double fai2 = calculateFai(opt1_x1, opt1_y1);

double fai3 = calculateFai(opt2_x1, opt2_y1);
    
double TX[3] = {1, 0, 0};
double TY[3] = {0, 1, 0};
double TZ[3] = {0, 0, 1};

double M1[3][3] = {
        {cos(fai1), -sin(fai1), 0},
        {sin(fai1), cos(fai1), 0},
        {0, 0, 1}
    };


    double M2[3][3] = {
        {cos(fai2), -sin(fai2), 0},
        {sin(fai2), cos(fai2), 0},
        {0, 0, 1}
    };

    double M3[3][3] = {
        {cos(fai3), -sin(fai3), 0},
        {sin(fai3), cos(fai3), 0},
        {0, 0, 1}
    };
double kradarR1[3], kopticalu1[3], kopticalu2[3];  
matrixMultiply(TY, M1, kradarR1);
matrixMultiply(TX, M2, kopticalu1);
matrixMultiply(TX, M3, kopticalu2);
double kopticalv[3] = { 0,0,1 };

double u1_long[2], v1_long[2], u2_long[2], v2_long[2], r1[2];
double u1_wide[2], v1_wide[2], u2_wide[2], v2_wide[2], r2[2];
 for (int i = 0; i < 2; i++) {
        u1_long[i] = c_long[i][0] * w_u / W_u * wuju1 / jiaoju * 1e-3;
        v1_long[i] = c_long[i][1] * w_v / W_v * wuju1 / jiaoju * 1e-3;
        u2_long[i] = c_long[i][4] * w_u / W_u * wuju2 / jiaoju * 1e-3;
        v2_long[i] = c_long[i][5] * w_v / W_v * wuju2 / jiaoju * 1e-3;
        r1[i]=c_long[i][2]*fbl_r;
        u1_wide[i] = c_wide[i][0] * w_u / W_u * wuju1 / jiaoju * 1e-3;
        v1_wide[i] = c_wide[i][1] * w_v / W_v * wuju1 / jiaoju * 1e-3;
        u2_wide[i] = c_wide[i][4] * w_u / W_u * wuju2 / jiaoju * 1e-3;
        v2_wide[i] = c_wide[i][5] * w_v / W_v * wuju2 / jiaoju * 1e-3;
        r2[i]=c_wide[i][2]*fbl_r;
    }
 
double comb_long[8][5]={
		   {u1_long[0],v1_long[0],u2_long[0],v2_long[0],r1[0]},
           {u1_long[0],v1_long[0],u2_long[0],v2_long[0],r1[1]},
           {u1_long[0],v1_long[0],u2_long[1],v2_long[1],r1[0]},
           {u1_long[0],v1_long[0],u2_long[1],v2_long[1],r1[1]},
           {u1_long[1],v1_long[1],u2_long[0],v2_long[0],r1[0]},
           {u1_long[1],v1_long[1],u2_long[0],v2_long[0],r1[1]},
           {u1_long[1],v1_long[1],u2_long[1],v2_long[1],r1[0]},
           {u1_long[1],v1_long[1],u2_long[1],v2_long[1],r1[1]}}; 
double comb_wide[8][5] = {
    {u1_wide[0], v1_wide[0], u2_wide[0], v2_wide[0], r2[0]},
    {u1_wide[0], v1_wide[0], u2_wide[0], v2_wide[0], r2[1]},
    {u1_wide[0], v1_wide[0], u2_wide[1], v2_wide[1], r2[0]},
    {u1_wide[0], v1_wide[0], u2_wide[1], v2_wide[1], r2[1]},
    {u1_wide[1], v1_wide[1], u2_wide[0], v2_wide[0], r2[0]},
    {u1_wide[1], v1_wide[1], u2_wide[0], v2_wide[0], r2[1]},
    {u1_wide[1], v1_wide[1], u2_wide[1], v2_wide[1], r2[0]},
    {u1_wide[1], v1_wide[1], u2_wide[1], v2_wide[1], r2[1]}
};



    double panel[8][3];
    double body[8][3];
    double l1[8][3];
    double l2[8][3];
    double anglee[8][8];
    double faxiang[8][3][3];
    double len_long[8];
    double len_wide[8];
    
for (int ii = 0; ii < 8; ii++) {
        double r1 = comb_long[ii][4];
        double u1 = comb_long[ii][0];
        double v1 = comb_long[ii][1];
        double u2 = comb_long[ii][2];
        double v2 = comb_long[ii][3];
		srand((unsigned)time(NULL)); 

		double* panel=PSO_search(r1, u1, v1, u2, v2, kradarR1, kopticalu1, kopticalv, kopticalu2);
        l1[ii][0] = cos(panel[1]) * sin(panel[2]);
        l1[ii][1] = cos(panel[1]) * cos(panel[2]);
        l1[ii][2] = sin(panel[1]);
        len_long[ii]=panel[0];

        
        double r2 = comb_wide[ii][4];
        u1 = comb_wide[ii][0];
        v1 = comb_wide[ii][1];
        u2 = comb_wide[ii][2];
        v2 = comb_wide[ii][3];

        double* body=PSO_search(r2, u1, v1, u2, v2, kradarR1, kopticalu1, kopticalv, kopticalu2);

        l2[ii][0] = cos(body[1]) * sin(body[2]);
        l2[ii][1] = cos(body[1]) * cos(body[2]);
        l2[ii][2] = sin(body[1]);
        len_wide[ii]=body[0];

 }

double norm_sun = sqrt(sun[0] * sun[0] + sun[1] * sun[1] + sun[2] * sun[2]);
double min_angle=10;
int mm;
int nn;


    for (int ii = 0; ii < 8; ii++) {
        for (int tt = 0; tt < 8; tt++) {
        	faxiang[tt][0][ii] = l2[ii][1] * l1[tt][2] - l2[ii][2] * l1[tt][1];
    		faxiang[tt][1][ii] = l2[ii][2] * l1[tt][0] - l2[ii][0] * l1[tt][2];
    		faxiang[tt][2][ii] = l2[ii][0] * l1[tt][1] - l2[ii][1] * l1[tt][0];
            double norm_faxiang = sqrt(faxiang[tt][0][ii] * faxiang[tt][0][ii] + faxiang[tt][1][ii] * faxiang[tt][1][ii] + faxiang[tt][2][ii] * faxiang[tt][2][ii]);            
            anglee[ii][tt] = acos((faxiang[tt][0][ii] * sun[0] + faxiang[tt][1][ii] * sun[1] + faxiang[tt][2][ii] * sun[2]) / (norm_faxiang * norm_sun));


			if (anglee[ii][tt] < min_angle) {
            min_angle = anglee[ii][tt];
            mm = ii;
            nn = tt;
        }
		}

}


double l1best[3];
double l2best[3];


    for (int c = 0; c < 3; c++) {
        l1best[c] = l1[nn][c];
        l2best[c] = l2[mm][c];

    }
    double length_long = len_long[nn];
    double length_wide = len_wide[mm];


printf("l1best[0] = %f\t", l1best[0]);
printf("l1best[1] = %f\n",l1best[1]);
printf("l1best[2] = %f\t", l1best[2]);
printf("len_long = %f\n", length_long);
printf("l2best[0] = %f\t", l2best[0]);
printf("l2best[1] = %f\n",l2best[1]);
printf("l2best[2] = %f\t", l2best[2]);
printf("len_wide = %f\n", length_wide);
end = clock();
std::cout<<"execute time = "<<double(end-start)/CLOCKS_PER_SEC + 0.27<<"s"<<std::endl;  //输出时间（单位：ｓ
}

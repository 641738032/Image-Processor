#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include<QImage>
#include<QRgb>
#include<vector>
#include<iostream>
#include"matrix.h"
#include"fft.h"
#include"mainwindow.h"
using namespace std;
#include<iterator>
class imageprocess
{
public:
    imageprocess(QImage img);
    static QImage rgb2gray(const QImage&);//不用复制，更有效率
    static QImage inverse(const QImage&);

    static QImage logTransformation(const QImage&,double c);
    static QImage histEq(const QImage&);
    static QImage colorHistEq(const QImage&);

/**********************空间滤波*****************************/
    static QImage linearFilter(const QImage&,vector<double>&,int);//线性滤波RGB
    static QImage linearFilterGray(const QImage&,vector<double>&,int);//线性滤波Gray

    static void kernelNormalization(std::vector<double>&);//归一化

    static QImage gaussBlur(const QImage&,int,double);//GaussBlur RGB
    static QImage gaussBlurGray(const QImage&,int,double);//GaussBlur Gray
    static std::vector<double> createGaussKernel(int,double);

    static QImage bilateralFilter(const QImage&,int,double,double);
    static std::vector<double> creatBilaterColorKernel(double);

    static QImage MedianFilter(const QImage&,int);//中值滤波Gray
    static QImage MedianFilterRGB(const QImage&,int);//中值滤波RGB
//    static std::vector<double> creatMedianKernel(double);

     static QImage MaximumFilter(const QImage&,int);//最大值滤波Gray
     static QImage MaximumFilterRGB(const QImage&,int);//最大值滤波RGB

      static QImage MinimumFilter(const QImage&,int);//最小值滤波Gary
      static QImage MinimumFilterRGB(const QImage&,int);//最小值滤波RGB

      static QImage LowPassFilterS(const QImage&,Matrix<double>&);//低通滤波时域卷积
      static QImage LowpassFilterF(const QImage&,Matrix<double>&);//低通滤波频域点乘

      static QImage HighPassFilterS(const QImage&,Matrix<double>&);//高通滤波时域卷积
      static QImage HighpassFilterF(const QImage&,Matrix<double>&);//高通滤波频域点乘

      static Matrix<double> Normalization(Matrix<double>&);//归一化
/**********************空间滤波*****************************/

/**********************频域滤波*****************************/
      static QImage imgfft2d(const QImage&);
      static QImage imgIfft2d(const QImage&);
/**********************频域滤波*****************************/

/**********************仿射变换*****************************/
      static QImage Transform(const QImage&,const QMatrix&);//QMatrix自带三乘三函数
/**********************仿射变换*****************************/
//--------------------------------------------------------//

private:
    QImage img;

    static QImage zeroPadding(const QImage & img,int nCol,int nRow);
    static Matrix<double> zeroPadding_M(Matrix<int> & ,int ,int);

    static QImage filter(const QImage&,std::vector<double>,int,int);//
    static Matrix<double> conv2(Matrix<double>&,Matrix<double>&,int,int);

    static QImage crop(const QImage&,int,int);//裁剪
    static Matrix<double> crop_M(const QImage&,Matrix<double>&,int,int);

    static std::vector<int> getPatch(const QImage&,int,int,int,int,char);
    static std::vector<int> getPatch_M_R(Matrix<double>&,int,int,int,int,char);
    static std::vector<int> getPatch_M_G(Matrix<double>&,int,int,int,int,char);
    static std::vector<int> getPatch_M_B(Matrix<double>&,int,int,int,int,char);
    static std::vector<int> getPatch_M_Y(Matrix<double>&,int,int,int,int,char);

    static QImage Duplicate(const QImage&,int,int);
    static Matrix<double> Duplicate_M(Matrix<int> &,int,int);
    static QImage Mirror(const QImage&,int,int);
    static Matrix<double> Mirror_M(Matrix<int> &,int,int);



/**********************仿射变换*****************************/
     static void getRange(const QImage&,const QMatrix& , double &minX, double& maxX,
                     double& minY, double& maxY);
     static void getRange_M(Matrix<int>&,const QMatrix& , double &minX, double& maxX,
                     double& minY, double& maxY);
     static QRgb getNewRgb(const QImage&,const QMatrix& , int i, int j,
                     double minX, double minY) ;
     static double getNewRgb_M(Matrix<int>&,const QMatrix& , int i, int j,
                     double minX, double minY) ;
     static QRgb bilinear(const QImage&,double x, double y) ;
     static double bilinear_M(Matrix<int>&,double x, double y) ;
     static double linear_inter(double v1, double v2,
                               double c1, double c2, double c3);
/**********************仿射变换*****************************/
//--------------------------------------------------------//
};

#endif // IMAGEPROCESS_H

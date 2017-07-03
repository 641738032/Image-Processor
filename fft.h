//
//  fft.h
//  fft-1
//
//  Created by 朱小琳 on 2017/5/4.
//  Copyright © 2017年 朱小琳. All rights reserved.
//

#ifndef fft_h
#define fft_h


#include <complex>
#include <vector>
#include "matrix.h"


const double PI = 3.1415926;

size_t calcN(size_t length);//输入一个数 返回2的整数次幂
std::complex<double> pow(std::complex<double> base, int exponent);

// different function with different input 返回值相同 类型不同int double complex-double  "> >"空格
std::vector<std::complex<double> > fft(std::vector<int> data, size_t N=0);
std::vector<std::complex<double> > fft(std::vector<double> data, size_t N=0);
std::vector<std::complex<double> > fft(std::vector<std::complex<double> > data, size_t N=0) ;

//fft2d
Matrix<std::complex<double> > fft2d(const Matrix<int>& data,size_t M=0 ,size_t N=0);
Matrix<std::complex<double> > fft2d(const Matrix<double>& data,size_t M=0 ,size_t N=0);
Matrix<std::complex<double> > fft2d(const Matrix<std::complex<double> >& data,size_t M=0 ,size_t N=0);
/*
//fftRow
Matrix<std::complex<double> > fftRow(const Matrix<std::complex<double> >& data);
Matrix<std::complex<double> > ifftRow(const Matrix<std::complex<double> >& data);
*/
//ifft
std::vector<std::complex<double> > ifft(std::vector<std::complex<double> > data, size_t N=0) ;
Matrix<std::complex<double> > ifft2d(const Matrix<int>& data,size_t M=0 ,size_t N=0);
Matrix<std::complex<double> > ifft2d(const Matrix<double>& data,size_t M=0 ,size_t N=0);
Matrix<std::complex<double> > ifft2d(const Matrix<std::complex<double> >& data,size_t M=0 ,size_t N=0);


#endif /* fft_h */

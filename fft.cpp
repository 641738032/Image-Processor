//
//  fft.cpp
//  fft-1
//
//  Created by 朱小琳 on 2017/5/4.
//  Copyright © 2017年 朱小琳. All rights reserved.
//

#include <stdio.h>
#include "fft.h"
#include <iostream>
using namespace std;

#include "matrix.h"

#define PI 3.1415926

using std::complex;
using std::vector;


size_t calcN(size_t length) {
    // check if length is power of 2
    // if it is, just return length
    // if not, get the correct N and return
    if( 0==(length&(length-1)))
    {
        return  length;
    }
    size_t temp  = length;
    while(temp>>=1)
    {
        length |=temp ;
    }
    return length+1;
}

complex<double> pow(complex<double> base, int exponent) {
    // return base^{exponent}
    complex<double> ret = complex<double>(1,0);
    
    while (exponent--) {
        ret= ret*base;
    }
    return ret;
}
//将int型的向量 转为 complex<double>型的向量
vector<complex<double> >
fft(vector<int> data, size_t N)
{
    // convert data to vector<complex<double> >
    // call:
    // vector<complex<double> >
    // fft(vector<complex<double> > data, size_t N)
    
    vector<complex<double> > temp;
    temp.assign(data.begin(), data.end());
    return fft(temp,N);
}
//将double型的向量 转为 complex<double>型的向量
vector<complex<double> >
fft(vector<double> data, size_t N)
{
    
    // convert data to vector<complex<double> >
    // call:
    // vector<complex<double> >
    // fft(vector<complex<double> > data, size_t N)
    
    vector<complex<double> > temp;
    temp.assign(data.begin(), data.end());
   /*
    for(size_t i=0;i<temp.size();++i)
    {
        cout<<temp.at(i)<<endl;
    }//测试输出
    */
    
    return fft(temp,N);
}
//进行fft计算,基2时间抽取法
vector<complex<double> >
fft(vector<complex<double> > data, size_t N) {
    
    // change length to make it beign just the power of 2
    if (0 == N)
    {
        N = calcN(data.size());
    }
    
    if (0 != (N & N - 1)){
        cout << "error N" << endl;
    }
    // append 0 if necessary
    size_t delta = N - data.size();
    while (delta--){
        data.push_back(complex<double>(0, 0));
    }

    // start fft
    // check if N is 0, 1, 2
    
    // if N is 0 or 1, just return data
    // if N is 2, do dft on them
    // if N > 2, do fft
    // 1. split input into two part
    // 2. do fft on them seperately
    // 3. construct result from output
    vector<complex<double> > ret;
    // when N=0, just return data
    if (0 == N)
    {
        return data;
    }
    
    // when N=1, just return data
    if (1 == N)
    {
        return data;
    }
    // when N=2, do dft on them，当N=2时，终止递归
    if (2 == N)
    {
        ret.push_back(data[0] + data[1]);
        ret.push_back(data[0] - data[1]);
        return ret;
    }
    // when N>2
    if (N >2)
    {
        vector<complex<double> >odd, even, oddRet, evenRet;
        for (size_t i = 0; i < N; i += 2)
        {
            // 1. split input into two part
            even.push_back(data[i]);
            odd.push_back(data[i + 1]);
        }
        // 2. do fft on them seperately
        evenRet = fft(even);//fft递归
        oddRet = fft(odd);
        
        // 3. construct result from output
        complex<double>w_N((cos(-2 * PI / N)), sin(-2 * PI / N));//W_N
        for (size_t i = 0; i<N / 2; ++i)
        {
            ret.push_back(evenRet[i] + oddRet[i] * pow(w_N, i));
            //even+odd*W_N的i次幂
        }
        for (size_t i = 0; i<N / 2; ++i)
        {
            ret.push_back(evenRet[i] - oddRet[i] * pow(w_N, i));
            //even-odd*W_N的i次幂
        }
        return ret;
    }
    return ret;
}
/**************************************************************/
//将int型的矩阵 转为 complex<double>型的矩阵，再做fft变换
Matrix<complex<double> >
fft2d(const Matrix<int>& data, size_t M , size_t N )
{
    Matrix<complex<double> > temp(M,N,complex<double>(0,0));
    for(size_t i=0;i<M;++i)
    {
        for(size_t j=0;j<N;++j)
        {
            temp(i,j) = complex<double>(data(i,j),0);
        }
    }
    return fft2d(temp,M,N);
}
//将double型的矩阵 转为 complex<double>型的矩阵，再做fft变换
Matrix<complex<double> >
fft2d(const Matrix<double>& data, size_t M , size_t N)
{
    Matrix<complex<double> > temp(M,N,complex<double>(0.0,0.0));
    for(size_t i=0;i<M;++i)
    {
        for(size_t j=0;j<N;++j)
        {
            temp(i,j) = complex<double>(data(i,j),0.0);
        }
    }
    return fft2d(temp,M,N);
}

//fft2d
Matrix<complex<double> >
fft2d(const Matrix<complex<double> >& data, size_t M , size_t N)
{
/*
 *
    // dertermin M and N

    M=calcN(data.getNRow());
    N=calcN(data.getNCol());
    cout<<" M=" <<M<<"\t N="<<N<<endl;//输出看M、N对不对
    
    // append 0  对矩阵进行补零操作
    Matrix<complex<double> > temp(M,N,complex<double>(0,0));//temp为补零后的矩阵
    for(size_t i=0;i<M;++i)
    {
        for(size_t j=0;j<N;++j)
        {
            try
            {
                temp(i,j)=data(i,j);
            }
            catch(range_error e)
            {
                continue;
            }
        }
    }
    
    cout << "*******  补零  temp:\n" <<temp<< endl;//输出看补零结果正确与否

    return ((fftRow((fftRow(temp)).transpose())).transpose()).transpose();
    */

        size_t newrow = calcN(M);
        size_t newcol = calcN(N);
        Matrix<complex<double> > m2(newrow,newcol,complex<double>(0,0));//m2是m补零后的结果
        for(size_t i=0;i<M;i++)
        {
            for(size_t j=0;j<N;j++)
            {
                m2(i,j) = data(i,j);
            }
        }//补零
        Matrix<complex<double> > res_m(newrow,newcol,complex<double>(0,0));
        res_m = m2;
        for(size_t i=0;i<newrow;i++)
        {
            vector<complex<double> > fftRow = fft(res_m.getRow(i),res_m.getNCol());
            if(res_m.setRow(i,fftRow))
            {
                cout<<"第"<<i<<"行"<<"fft变换成功"<<endl;
            }
        }
        res_m.transpose();
        for(size_t i=0;i<newrow;i++)
        {
            vector<complex<double> > fftRow = fft(res_m.getRow(i),res_m.getNCol());
            if(res_m.setRow(i,fftRow))
            {
                cout<<"第"<<i<<"列"<<"fft变换成功"<<endl;
            }
        }
        res_m.transpose();
        return res_m;

    /*
        // dertermin M and N
            if(0==M) M=calcN(data.getNRow());
            if(0==N) N=calcN(data.getNCol());
            if(0!=(M&(M-1)||0!=(N&(N-1)))){
            cout<<"error!!"<<endl;
            return data;
            }
            // append 0
            Matrix<complex<double> > temp(M,N,0);
            for(size_t i=0;i<M;++i)
            {
             for(size_t j=0;j<N;++j)
             {
                 try{
                 temp(i,j)=data(i,j);
                 }catch(range_error e){
                     continue;
                 }
             }
            }
            return fftRow(fftRow(data).transpose()).transpose();
*/
}

/*
 *
Matrix<complex<double> >
fftRow(const Matrix<complex<double> >& data)
{
    //Matrix<complex<double> > ret(r,c,complex<double>(0,0) ) ;//新建矩阵，用于存储做完fft的矩阵
    size_t M = data.getNRow();
    size_t N = data.getNCol();
    size_t newrow = calcN(M);
    size_t newcol = calcN(N);
    Matrix<complex<double> > m2(newrow,newcol,complex<double>(0,0));//m2是m补零后的结果
    for(size_t i=0;i<M;i++)
    {
        for(size_t j=0;j<N;j++)
        {
            m2(i,j) = data(i,j);
        }
    }
    Matrix<complex<double> > res_m(newrow,newcol,complex<double>(0,0));
    for(size_t i=0;i<newrow;i++)
    {
        vector<complex<double> > fftRow = fft(res_m.getRow(i),res_m.getNCol());
        if(res_m.setRow(i,fftRow))
        {
            cout<<"第"<<i<<"行"<<"fft变换成功"<<endl;
        }
    }
    return res_m;
    */

/*
    for (size_t i=0; i<ret.getNRow(); ++i)
    {
    ret.setRow(i, fft(data.getRow(i)) );//取出data的第i行，做完fft，再将值赋给新的矩阵ret第i行
    }
     cout << "******* 一维fft  ret:\n" <<ret<< endl;//test
    return ret;
    */
//}

/*************************ifft*************************************/
vector<complex<double> >
ifft(vector<complex<double> > data, size_t N) {
    // change length to make it beign just the power of 2
    // determin N
    if (0 == N)
    {
        N = calcN(data.size());
    }
    if (0 != (N & N - 1)){
        cout << "error N" << endl;
    }
    
    // append 0 if necessary
    size_t delta = N - data.size();
    while (delta--){
        data.push_back(complex<double>(0, 0));
    }
    
    vector<complex<double> > ret;
    //取共轭
    for (size_t i = 0; i<data.size(); ++i) {
        data[i]=complex<double>(data[i].real(), -data[i].imag());
    }
    //除以N
    ret=fft(data);//ifft
    for (size_t i = 0; i<ret.size(); ++i) {
        ret[i] = complex<double>(ret[i].real(), -ret[i].imag())/(double)N;
        // IFFT的公式
    }
    
    return ret;
}

/*
Matrix<complex<double> >
ifftRow(const Matrix<complex<double> >& data)
{
    size_t r = data.getNRow();
    size_t c = data.getNCol();
    Matrix<complex<double> > ret(r,c,complex<double>(0,0) ) ;
    
    for (size_t i=0; i<ret.getNRow(); ++i)
    {
        ret.setRow(i, ifft(data.getRow(i)) );//与fftRow不同之处，只需调用ifft即可
    }
   // cout << "*******    ret:\n" <<ret<< endl;//test
    return ret;

}
*/

/**************************************************************/
//将int型的矩阵 转为 complex<double>型的矩阵，再做fft变换
Matrix<complex<double> >
ifft2d(const Matrix<int>& data, size_t M , size_t N )
{
    Matrix<complex<double> > temp(M,N,complex<double>(0,0));
    for(size_t i=0;i<M;++i)
    {
        for(size_t j=0;j<N;++j)
        {
            temp(i,j) = complex<double>(data(i,j),0);
        }
    }
    return ifft2d(temp,M,N);
}
//将double型的矩阵 转为 complex<double>型的矩阵，再做fft变换
Matrix<complex<double> >
ifft2d(const Matrix<double>& data, size_t M , size_t N)
{
    Matrix<complex<double> > temp(M,N,complex<double>(0.0,0.0));
    for(size_t i=0;i<M;++i)
    {
        for(size_t j=0;j<N;++j)
        {
            temp(i,j) = complex<double>(data(i,j),0.0);
        }
    }
    return ifft2d(temp,M,N);
}
Matrix<complex<double> >
ifft2d(const Matrix<complex<double> >& data, size_t M , size_t N)
{
    size_t newrow = calcN(M);
    size_t newcol = calcN(N);
//    cout<<"newrow="<<newrow<<",newcol= "<< newcol<<endl;
    Matrix<complex<double> > m2(newrow,newcol,complex<double>(0,0));//m2是m补零后的结果
    for(size_t i=0;i<M;i++)
    {
        for(size_t j=0;j<N;j++)
        {
            m2(i,j) = data(i,j);
        }
    }//补零
    Matrix<complex<double> > res_m(newrow,newcol,complex<double>(0,0));
    res_m = m2;
    for(size_t i=0;i<newrow;i++)
    {
        vector<complex<double> > ifftRow = ifft(res_m.getRow(i),res_m.getNCol());
        if(res_m.setRow(i,ifftRow))
        {
            cout<<"第"<<i<<"行"<<"ifft变换成功"<<endl;
        }
    }
    res_m.transpose();
    for(size_t i=0;i<newrow;i++)
    {
        vector<complex<double> > ifftRow = ifft(res_m.getRow(i),res_m.getNCol());
        if(res_m.setRow(i,ifftRow))
        {
            cout<<"第"<<i<<"列"<<"ifft变换成功"<<endl;
        }
    }
    res_m.transpose();
    return res_m;
}

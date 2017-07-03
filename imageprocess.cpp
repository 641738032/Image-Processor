#include "imageprocess.h"
#include<cmath>
#include<algorithm>
#include<cstdarg>
#include"mainwindow.h"
#include"matrix.h"
#include<iostream>
using namespace std;
#include<fft.h>

imageprocess::imageprocess(QImage image):
    img(image)
{
}
QImage imageprocess::rgb2gray(const QImage& img)
{
    Matrix<int> matimg(img.height(),img.width(),0);
    Matrix<int> matimgr(img.height(),img.width(),0);
    Matrix<int> matimgg(img.height(),img.width(),0);
    Matrix<int> matimgb(img.height(),img.width(),0);
    matimgr= matimgr.fromQImage(img,'r');
    matimgg = matimgg.fromQImage(img,'g');
    matimgb= matimgb.fromQImage(img,'b');
    QImage ret(img);//调用复制构造函数
    int height =img.height();
    int width = img.width();
    for(int i=0;i<height;++i)
    {
    for(int j=0;j<width;++j)
    {
//        QRgb rgb =img.pixel(i,j);//依次读取每个像素的rgb值
//        int r = qRed(rgb);//1~8/red/9~16/green
//        int g = qGreen(rgb);
//        int b = qBlue(rgb);
        int gray = (matimgr(i,j)+matimgg(i,j)+matimgb(i,j))/3;
//        ret.setPixel(i,j,qRgb(gray,gray,gray));//将灰度值赋给对应的像素
        matimg(i,j) = gray;
    }
    }

    ret = matimg.toQImage(matimg);
    return ret;
}

QImage imageprocess::inverse(const QImage& img)
{
    Matrix<int> matimg(img.height(),img.width(),0);
    Matrix<int> matimgr(img.height(),img.width(),0);
    Matrix<int> matimgg(img.height(),img.width(),0);
    Matrix<int> matimgb(img.height(),img.width(),0);
    matimgr= matimgr.fromQImage(img,'r');
    matimgg = matimgg.fromQImage(img,'g');
    matimgb= matimgb.fromQImage(img,'b');
    QImage ret(img);//调用复制构造函数
    int height =img.height();
    int width = img.width();
    for(int i=0;i<height;++i)
    {
    for(int j=0;j<width;++j)
    {
//        QRgb rgb =img.pixel(i,j);
//        int r = qRed(rgb);//1~8 red//9~16 green//17~24 blue
//        int g = qGreen(rgb);
//        int b = qBlue(rgb);
        matimgr(i,j) = 255 -matimgr(i,j);
        matimgg(i,j) = 255 -matimgg(i,j);
        matimgb(i,j) = 255 -matimgb(i,j);
//        ret.setPixel(i,j,qRgb(255-r,255-g,255-b));
    }
    }
    ret= matimg.toQImage(matimgr,matimgg,matimgb);
    return ret;
}

QImage imageprocess::logTransformation(const QImage& img,double c)
{
    Matrix<int> matimg(img.height(),img.width(),0);
    Matrix<int> matimgy(img.height(),img.width(),0);
    matimgy= matimgy.fromQImage(img,'y');

    cout<<"logTransformation"<<endl;
    QImage ret(img);
    int width =img.width();
    int height=img.height();
    for(int i=0;i<height;++i)
    {
        for(int j=0;j<width;++j)
        {
//            QRgb rgb = ret.pixel(i,j);
//            int g =qRed(rgb);
            int ng = ( c * log(1+matimgy(i,j)/255.0))*255;
            if(ng > 255)  ng=255;
//            ret.setPixel(i,j,qRgb(ng,ng,ng));
            matimg(i,j) = ng;
        }
}
    ret = matimg.toQImage(matimg);
    return ret;
}

QImage imageprocess::histEq(const QImage& img)
{
    Matrix<int> matimg(img.height(),img.width(),0);
    Matrix<int> matimgy(img.height(),img.width(),0);
    matimgy= matimgy.fromQImage(img,'y');

    QImage ret(img);
    int width = img.width();
    int height = img.height();
    int N = width*height;

    //count histogram
    int hist[256];
    std::fill(hist,hist+256,0);
    for(int i=0;i<height;++i)
        for(int j=0;j<width;++j)
            hist[ matimgy(i,j)]++;

    //calculate map function
    int map[256];
    double sum=0;
    for(int i=0;i<256;++i )
    {
        sum+=hist[i];
        map[i] = round(sum/N *255);
    }

    //map the pixel
    for(int i=0;i<height;++i)
    {
        for(int j=0;j<width;++j)
        {
            int g = map[ matimgy(i,j) ];
//            ret.setPixel(i,j, qRgb(g , g , g));
            matimg(i,j) = g ;

        }
    }
    ret = matimg.toQImage(matimg);
    return ret;
}

QImage imageprocess::colorHistEq(const QImage &img)
{
    Matrix<int> matimg(img.height(),img.width(),0);
    Matrix<int> matimgr(img.height(),img.width(),0);
    Matrix<int> matimgg(img.height(),img.width(),0);
    Matrix<int> matimgb(img.height(),img.width(),0);
    matimgr= matimgr.fromQImage(img,'r');
    matimgg = matimgg.fromQImage(img,'g');
    matimgb= matimgb.fromQImage(img,'b');

    QImage ret(img);
    int width =img.width();
    int height=img.height();
    int N=width*height;

    //count histogram
    int hist[3][256];
    std::fill(hist[0],hist[0]+3*256,0);
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j)
        {
            hist[0][matimgr(i,j)]++;
            hist[1][matimgg(i,j)]++;
            hist[2][matimgb(i,j)]++;
        }
    }

    //calculate map function
    int map[3][256];
    double sumR=0;
    double sumG=0;
    double sumB=0;
    for(int i=0;i<256;++i )
    {
        sumR+=hist[0][i];
        map[0][i] = round(sumR/N *255);
        sumG+=hist[1][i];
        map[1][i] = round(sumG/N *255);
        sumB+=hist[2][i];
        map[2][i] = round(sumB/N *255);
    }
    //map the pixel
    for(int i=0;i<height;++i)
    {
        for(int j=0;j<width;++j)
        {
            int r = map[0][matimgr(i,j)];
            int g = map[1][matimgg(i,j)];
            int b = map[2][matimgb(i,j)];
//            ret.setPixel(i,j,qRgb(r,g,b));
            matimgr(i,j) = r;
            matimgg(i,j) = g;
            matimgb(i,j) = b;
        }
    }
    ret = matimg.toQImage(matimgr,matimgg,matimgb);
    return ret;
}

//----------------------->
QImage imageprocess::linearFilter(const QImage & img,vector<double>& kernel,int nCol)
{
    cout<<"linearFilter"<<endl;

    int nRow = kernel.size()/nCol;
    cout<<"*********nRow**********"<<nRow<<endl;  //

    if (0 == nRow%2 || 0 == nCol%2) return img;//

//*****************获取滤波器的矩阵************************
    Matrix<double> filtermat(nCol,nCol,0);
    for(int i=0;i<nCol;++i)
    {
        for(int j=0;j<nCol;++j)
        {
//                cout<<"i="<<i<<",j="<<j<<"matrix="<<filtermat(i,j)<<endl;
                filtermat(i,j) = kernel[nCol*i+j];
                cout<<"i="<<i<<",j="<<j<<",matrix="<<filtermat(i,j)<<endl;
        }
    }
    cout<<"******filtermat*****"<<'\t'<<endl;
    cout<<filtermat<<endl;
//*****************获取滤波器的矩阵************************

    Matrix<int> matimg(img.height(),img.width(),0);
    Matrix<int> matimgr(img.height(),img.width(),0);
    Matrix<int> matimgg(img.height(),img.width(),0);
    Matrix<int> matimgb(img.height(),img.width(),0);
    matimgr = matimg.fromQImage(img,'r');
    matimgg = matimg.fromQImage(img,'g');
    matimgb = matimg.fromQImage(img,'b');
//    cout<<matimg.getNRow()<<'\n'<<matimg.getNCol()<<endl;

    cout<<"图像转矩阵："<<'\t'<<endl;
    cout<<matimgr<<endl;

//***************int转double***************************
    Matrix<double> paddedr(img.height(),img.width(),0);
    Matrix<double> paddedg(img.height(),img.width(),0);
    Matrix<double> paddedb(img.height(),img.width(),0);
    for(int i=0;i<img.height();++i)
    {
        for(int j =0;j<img.width();++j)
        {
            paddedr(i,j) = (double)matimgr(i,j);
            paddedg(i,j) = (double)matimgg(i,j);
            paddedb(i,j) = (double)matimgb(i,j);
        }
    }
//********************filter*******************
//    QImage filtered = filter(padded,kernel,nCol,nRow);//滤波
    paddedr = paddedr.conv(paddedr,filtermat);
    paddedg = paddedg.conv(paddedg,filtermat);
    paddedb = paddedb.conv(paddedb,filtermat);

    cout<<"filter"<<endl;
    cout<<"卷积后的矩阵："<<'\t'<<endl;
    cout<<paddedr<<endl;

//*************归一化******************
    paddedr =Normalization(paddedr);
    paddedg =Normalization(paddedg);
    paddedb =Normalization(paddedb);

//*************矩阵转图像******************
    QImage res = paddedr.toQImage(paddedr,paddedg,paddedb);
    return res;
}

QImage imageprocess::linearFilterGray(const QImage & aa,vector<double>& kernel,int nCol)
{
    cout<<"linearFilterGray"<<endl;
    QImage img(rgb2gray(aa));

    int nRow = kernel.size()/nCol;
    cout<<"*********nRow**********"<<nRow<<endl;  //

    if (0 == nRow%2 || 0 == nCol%2) return img;//
//**************获取滤波器矩阵******************
    Matrix<double> filtermat(nCol,nCol,0);
    for(int i=0;i<nCol;++i)
    {
        for(int j=0;j<nCol;++j)
        {
//                cout<<"i="<<i<<",j="<<j<<"matrix="<<filtermat(i,j)<<endl;
                filtermat(i,j) = kernel[nCol*i+j];
                cout<<"i="<<i<<",j="<<j<<",matrix="<<filtermat(i,j)<<endl;
        }
    }
    cout<<"******filtermat*****"<<'\t'<<endl;
    cout<<filtermat<<endl;

    Matrix<int> matimg(img.height(),img.width(),0);
    matimg = matimg.fromQImage(img,'y');

    cout<<"图像转矩阵："<<'\t'<<endl;
    cout<<matimg<<endl;

    Matrix<double> padded(img.height(),img.width(),0);
    for(int i=0;i<img.height();++i)
    {
        for(int j =0;j<img.width();++j)
        {
            padded(i,j) = (double)matimg(i,j);
        }
    }

    padded = padded.conv(padded,filtermat);
    padded = Normalization(padded);
    QImage res = padded.toQImage(padded);

    return res;
}


Matrix<double> imageprocess::zeroPadding_M(Matrix<int>& matimg,int nCol,int nRow)
{
    Matrix<double> padded(matimg.getNRow()+nRow-1,matimg.getNCol()+nCol-1,0.0f);
    int widthp = padded.getNCol();
    int heightp = padded.getNRow();    //扩展边界
//    cout<<"补零后的行："<<heightp<<"补零后的列："<<widthp<<endl;

    for (int i = nRow/2;i<heightp-nRow/2;i++)
    {
        for (int j=nCol/2;j<widthp-nCol/2;j++)
        {
//                ret.setPixel(i,j,img.pixel(i-nCol/2,j-nRow/2));
                padded(i,j) = matimg(i-nRow/2,j-nCol/2);
            }
    }
//    cout<<"padded"<<endl;
//    cout<<"补零后的矩阵："<<'\t'<<endl;
//    cout<<padded<<endl;
    return padded;
}
Matrix<double> imageprocess::Duplicate_M(Matrix<int>& matimg,int nCol,int nRow)
{
    Matrix<double> padded(matimg.getNRow()+nRow-1,matimg.getNCol()+nCol-1,0.0f);
    int width = padded.getNCol();
    int height = padded.getNRow();    //扩展边界
    cout<<"补零后的行："<<height<<"补零后的列："<<width<<endl;

    for (int i = nRow/2 ; i<height-nRow/2 ; i++)//1111原图像的区域
    {
        for (int j = nCol/2 ; j<width-nCol/2 ; j++)
        {
                padded(i,j) = matimg(i-nRow/2,j-nCol/2);
        }
    }

    for (int i=nRow/2;i<height-nRow/2;i++)//222原图左边区域
    {
        for (int j = 0 ; j < nCol/2;j++)
        {
//                ret.setPixel(i,j,ret.pixel(nCol/2,j));
            padded(i,j) = matimg(i,nCol/2);
        }
    }

    for (int i=nRow/2;i<height-nRow/2;i++)//333原图右边区域
    {
        for (int j = width-nCol/2 ; j<width ; j++)
        {
//                ret.setPixel(i,j,ret.pixel(width-nCol/2-1,j));
            padded(i,j) = matimg(i,width-nCol/2-1);
        }
    }

    for (int i = 0 ; i < nRow/2 ; i++)//444原图上边区域
    {
        for (int j = 0 ; j<width ; j++)
        {
//                ret.setPixel(i,j,ret.pixel(i,nRow/2));
            padded(i,j) = matimg(nRow/2,j);
        }
    }

    for (int i = height - nRow/2 ; i < height ; i++)//555原图下边区域
    {
        for (int j = 0 ; j<width ; j++)
        {
//                ret.setPixel(i,j,ret.pixel(i,height- nRow/2 - 1));
            padded(i,j) = matimg(height- nRow/2 - 1,j);
        }
    }

    return padded;
    cout<<"Duplicate"<<endl;
}
Matrix<double> imageprocess::Mirror_M(Matrix<int>& matimg,int nCol,int nRow)
{
    Matrix<double> padded(matimg.getNRow()+nRow-1,matimg.getNCol()+nCol-1,0.0f);
    int width = padded.getNCol();
    int height = padded.getNRow();    //扩展边界
//    QImage ret(img.width()+nCol-1,img.height()+nRow-1,img.format());
//    int width = ret.width();
//    int height = ret.height();    //扩展边界

    for (int i = nRow/2 ; i<height-nRow/2 ; i++)//1111原图像的区域
    {
        for (int j = nCol/2 ; j<width-nCol/2 ; j++)
        {
//                ret.setPixel(i,j,img.pixel(i-nCol/2,j-nRow/2));
            padded(i,j) = matimg(i-nRow/2,j-nCol/2);
        }
    }

    for (int i=nRow/2;i<height-nRow/2;i++)//222原图左边区域
    {
        for (int j = 0 ; j < nCol/2;j++)
        {
//                ret.setPixel(i,j,ret.pixel(nCol/2,j));
            padded(i,j) = matimg(i,nCol-2-j);
        }
    }


    for (int i=nRow/2;i<height-nRow/2;i++)//333原图右边区域
    {
        for (int j = width-nCol/2 ; j<width ; j++)
        {
//                ret.setPixel(i,j,ret.pixel(width-nCol/2-1,j));
            padded(i,j) = matimg(i,2*width-nCol-j);
        }
    }

    for (int i = 0 ; i < nRow/2 ; i++)//444原图上边区域
    {
        for (int j = 0 ; j<width ; j++)
        {
//                ret.setPixel(i,j,ret.pixel(i,nRow/2));
            padded(i,j) = matimg(nRow-2-i,j);
        }
    }

    for (int i = height - nRow/2 ; i < height ; i++)//555原图下边区域
    {
        for (int j = 0 ; j<width ; j++)
        {
//                ret.setPixel(i,j,ret.pixel(i,height- nRow/2 - 1));
            padded(i,j) = matimg(2*height - nRow  -i,j);
        }
    }
    return padded;
    cout<<"Mirror"<<endl;
}
//----------------------------------->
QImage imageprocess::zeroPadding(const QImage & img,int nCol,int nRow)
{
    QImage ret(img.width()+nCol-1,img.height()+nRow-1,img.format());
    int width = ret.width();
    int height = ret.height();    //扩展边界
    for (int i = 0;i<width;i++)
    {
        for (int j=0;j<height;j++)
        {
            if (i<nCol/2 || i>=width-nCol/2 ||
                    j<nRow/2 || j>=height-nRow/2)//padding之后需要填充的边界，补零
                ret.setPixel(i,j,qRgb(0,0,0));
            else
                ret.setPixel(i,j,img.pixel(i-nCol/2,j-nRow/2));
        }
    }
    return ret;
    cout<<"zeroPadding"<<endl;
}
QImage imageprocess::Duplicate(const QImage & img,int nCol,int nRow)
{
    QImage ret(img.width()+nCol-1,img.height()+nRow-1,img.format());
    int width = ret.width();
    int height = ret.height();    //扩展边界

    for (int i = nCol/2 ; i<width-nCol/2 ; i++)//1111原图像的区域
    {
        for (int j = nRow/2 ; j<height-nRow/2 ; j++)
        {
                ret.setPixel(i,j,img.pixel(i-nCol/2,j-nRow/2));
        }
    }

    for (int i = 0 ; i < nCol/2;i++)//222原图左边区域
    {
        for (int j=nRow/2;j<height-nRow/2;j++)
        {
                ret.setPixel(i,j,ret.pixel(nCol/2,j));
        }
    }

    for (int i = width-nCol/2 ; i<width ; i++)//333原图右边区域
    {
        for (int j=nRow/2;j<height-nRow/2;j++)
        {
                ret.setPixel(i,j,ret.pixel(width-nCol/2-1,j));
        }
    }

    for (int i = 0 ; i<width ; i++)//444原图上边区域
    {
        for (int j = 0 ; j < nRow/2 ; j++)
        {
                ret.setPixel(i,j,ret.pixel(i,nRow/2));
        }
    }

    for (int i = 0 ; i<width ; i++)//555原图下边区域
    {
        for (int j = height - nRow/2 ; j < height ; j++)
        {
                ret.setPixel(i,j,ret.pixel(i,height- nRow/2 - 1));
        }
    }

    return ret;
    cout<<"Duplicate"<<endl;
}
QImage imageprocess::Mirror(const QImage & img,int nCol,int nRow)
{
    QImage ret(img.width()+nCol-1,img.height()+nRow-1,img.format());
    int width = ret.width();
    int height = ret.height();    //扩展边界

    for (int i = nCol/2 ; i<width-nCol/2 ; i++)//1111原图像的区域
    {
        for (int j = nRow/2 ; j<height-nRow/2 ; j++)
        {
                ret.setPixel(i,j,img.pixel(i-nCol/2,j-nRow/2));
        }
    }

    for (int i = 0 ; i < nCol/2;i++)//222原图左边区域
    {
        for (int j=nRow/2;j<height-nRow/2;j++)
        {
                //ret.setPixel(i,j,img.pixel(nCol-1-i,j));
            ret.setPixel(i,j,ret.pixel(nCol-2-i,j));
        }
    }

    for (int i = width-nCol/2 ; i<width ; i++)//333原图右边区域
    {
        for (int j=nRow/2;j<height-nRow/2;j++)
        {
                ret.setPixel(i,j,ret.pixel(2*width-nCol-i,j));
        }
    }

    for (int i = 0 ; i<width ; i++)//444原图上边区域
    {
        for (int j = 0 ; j < nRow/2 ; j++)
        {
                ret.setPixel(i,j,ret.pixel(i,nRow-2-j));
        }
    }

    for (int i = 0 ; i<width ; i++)//555原图下边区域
    {
        for (int j = height - nRow/2 ; j < height ; j++)
        {
                ret.setPixel(i,j,ret.pixel(i,2*height - nRow  -j ));
        }
    }
    return ret;
    cout<<"Mirror"<<endl;
}

QImage imageprocess:: filter(const QImage& img,std::vector<double> kernel,int nCol,int nRow)
{
    QImage ret(img);
    int width = img.width();
    int height = img.height();
    for (int i = nCol/2;i<width-nCol/2;i++)
    {
        for (int j=nRow/2;j<height-nRow/2;j++)
        {
            std::vector<int> patchR = getPatch(img,i,j,nCol,nRow,'r');
            std::vector<int> patchG = getPatch(img,i,j,nCol,nRow,'g');
            std::vector<int> patchB = getPatch(img,i,j,nCol,nRow,'b');
            double sumR = 0, sumG = 0, sumB = 0;
            for (int k=0;k<kernel.size();k++)
            {
                sumR += kernel[k]*patchR[k];
                sumG += kernel[k]*patchG[k];
                sumB += kernel[k]*patchB[k];      //从原图中对应位置获取和滤波器大小相同的图像块，相乘 相加
            }
            int r = (int) sumR;
            int g = (int) sumG;
            int b = (int) sumB;
            if (r>255) r =  255;   //RGB的范围应该是0～255
            if (g>255) g = 255;
            if (b>255) b = 255;
            if (r<0) r = 0;
            if (g<0) g = 0;
            if (b<0) b = 0;
            ret.setPixel(i,j,qRgb(r,g,b));
            //cout<<"setPixel"<<endl;
        }
    }
    cout<<"filter"<<endl;
    return ret;
}
Matrix<double> imageprocess:: conv2(Matrix<double>& m1,Matrix<double>& m2,int nCol,int nRow)
{
    cout<<"--------------------filter-----------"<<endl;
    cout<<m2<<endl;
    //----------------变换滤波器----------------
    Matrix<double> mfil1(m2.getNRow(), m2.getNCol(), 0);//第一次变换后的滤波器矩阵,滤波器一般为正方形
    Matrix<double> mfil2(m2.getNRow(), m2.getNCol(), 0);//第二次变换后的滤波器矩阵
    for(size_t i=0;i<mfil1.getNRow();i++)
    {
        mfil1.setRow(mfil1.getNRow()-1-i, m2.getRow(i));//第2行 设为输入filter的 第0行
    }
    for(size_t j=0;j<mfil2.getNCol();j++)
    {
        mfil2.setCol(mfil2.getNCol()-1-j, mfil1.getCol(j));//第二列 设为输入的 第0列
    }
    cout<<"---------------filter after transform------------"<<endl;
    cout<<mfil2<<endl;
//------把滤波器里的值按照行列顺序放入vector中，方便调用---------
            vector<double> filter;
            for(size_t k1=0;k1<mfil2.getNRow();k1++)
            {
                for(size_t k2=0;k2<mfil2.getNCol();k2++)
                {
                    filter.push_back(mfil2(k1,k2));
//                    cout<<"vector    :"<<filter[nCol*k1+k2]<<'\n';
//                    cout<<endl;
                }
            }
    int width = m1.getNCol();
    int height = m1.getNRow();
    cout<<"width ="<<width<<", height ="<<height<<endl;
    Matrix<double> resimg(m1.getNRow(),m1.getNCol(),0);//卷积后的结果矩阵
    double sum = 0,seq = 0;
    for (int i=nRow/2;i<height-nRow/2;i++)
    {
        for (int j = nCol/2;j<width-nCol/2;j++)
        {
            for(int m= i-nRow/2 ; m <= i+nRow/2 ; m++)//滤波器的所在的行的范围
            {
                for(int n=j - nCol/2;n <= j+nCol/2; n++)//滤波器所在的列的范围
                {
                   sum += filter[seq]*m1(m,n);
//                   cout<<sum<<",";
                //从原图中对应位置获取和滤波器大小相同的图像块，相乘 相加
                }
            }
//            cout<<endl;
            resimg(i-nRow/2,j-nCol/2) = sum;
            //cout<<"setPixel"<<endl;
        }
    }
    return resimg;
}
QImage imageprocess::crop(const QImage& img,int nCol,int nRow)  //裁剪
{
    QImage ret(img.width() - nCol+1 ,img.height()-nRow +1,img.format());//裁剪图像为原来的大小
    int width = ret.width();
    int height = ret.height();
    for(int  i=0;i<width;++i)
    {
        for(int j=0;j<height;++j)
        {
                ret.setPixel(i,j,img.pixel(i+nCol/2,j+nRow/2));//将图像往左上角移动(nCol/2,nRow/2)
        }
    }
    cout<<"crop"<<endl;
    return ret;
}

Matrix<double> imageprocess::crop_M(const QImage& img,Matrix<double> & mat, int nCol, int nRow)
{
    int width = img.width();
    int height = img.height();
    Matrix<double>  ret(height,width ,0);
    for(int  i=0;i<height;++i)
    {
        for(int j=0;j<width;++j)
        {
            ret(i,j) = mat(i+nRow/2,j+nCol/2);
        }
    }
    return ret;
    cout<<"crop_M"<<endl;
}

//----------------------------------->

std::vector<int> imageprocess::getPatch(const QImage& img,int w,int h,int nCol,int nRow,char c)
{
    std::vector<int> ret;
    for (int i=w-nCol/2;i<= w+nCol/2;i++)
    {
        for (int j=h-nRow/2;j<=h+nRow/2;j++)
        {
            switch (c) {
            case 'r':
                ret.push_back(qRed(img.pixel(i,j)));
                break;
            case 'g':
                ret.push_back(qGreen(img.pixel(i,j)));
                break;
            case 'b':
                ret.push_back(qBlue(img.pixel(i,j)));
                break;
            case 'y':
                ret.push_back(qGray(img.pixel(i,j)));
                break;
            default:
                break;
        }
       }
    }
 return ret;
}
std::vector<int> imageprocess::getPatch_M_R(Matrix<double>& padded,int w,int h,int nCol,int nRow,char c)
{
    std::vector<int> ret;
    for (int i=h-nRow/2;i<=h+nRow/2;i++)
    {
        for (int j=w-nCol/2;j<= w+nCol/2;j++)
        {
//            cout<<"i=="<<i<<",,j=="<<j<<endl;
                ret.push_back(padded(i,j));
        }
     }
//    cout<<"getPatch_M_R"<<endl;
    return ret;
}
std::vector<int> imageprocess::getPatch_M_G(Matrix<double>& padded,int w,int h,int nCol,int nRow,char c)
{
    std::vector<int> ret;
    for (int i=h-nRow/2;i<=h+nRow/2;i++)
    {
        for (int j=w-nCol/2;j<= w+nCol/2;j++)
        {
//            cout<<"i=="<<i<<",,j=="<<j<<endl;
                ret.push_back(padded(i,j));
        }
    }
//    cout<<"getPatch_M"<<endl;
    return ret;
}
std::vector<int> imageprocess::getPatch_M_B(Matrix<double>& padded,int w,int h,int nCol,int nRow,char c)
{
    std::vector<int> ret;
    for (int i=h-nRow/2;i<=h+nRow/2;i++)
    {
        for (int j=w-nCol/2;j<= w+nCol/2;j++)
        {
//            cout<<"i=="<<i<<",,j=="<<j<<endl;
                ret.push_back(padded(i,j));
        }
    }
//    cout<<"getPatch_M"<<endl;
    return ret;
}
std::vector<int> imageprocess::getPatch_M_Y(Matrix<double>& padded,int w,int h,int nCol,int nRow,char c)
{
    std::vector<int> ret;
    for (int i=h-nRow/2;i<=h+nRow/2;i++)
    {
        for (int j=w-nCol/2;j<= w+nCol/2;j++)
        {
//            cout<<"i=="<<i<<",,j=="<<j<<endl;
                ret.push_back(padded(i,j));
        }
    }
//    cout<<"getPatch_M_Y"<<endl;
    return ret;
}

void imageprocess::kernelNormalization(std::vector<double>&kernel)//滤波器归一化函数，方便输入
{
    double sum =0 ;
    for(int i =0 ;i<kernel.size();++i)
    {
        sum = sum+kernel[i]; //kernel 系数的和
    }
    if(sum !=0)
    {
        for(int i=0;i<kernel.size();++i)
        {
            kernel[i]  = kernel [i] /sum;  //归一化
        }
    }
    //cout<<"kernelNormalization"<<endl;
}

QImage imageprocess::gaussBlur(const QImage & img, int size, double sigma)
{
    std::vector<double> kernel = createGaussKernel(size,sigma);
//    kernelNormalization(kernel);
    return linearFilter(img,kernel,size);
}
QImage imageprocess::gaussBlurGray(const QImage & img, int size, double sigma)
{
    std::vector<double> kernel = createGaussKernel(size,sigma);
//    kernelNormalization(kernel);
    return linearFilterGray(img,kernel,size);
//    QImage linear = linearFilterGray(img,kernel,size);

//    Matrix<int> linearmat(linear.height(),linear.width(),0);
//    linearmat = linearmat.fromQImage(linear,'y');
//    Matrix<double> linearmatd(linear.height(),linear.width(),0);
//    for(int i = 0;i<linear.height();++i)
//    {
//        for(int j=0;j<linear.width();++j)
//        {
//            linearmatd(i,j) =(double) linearmat(i,j);
//        }
//    }

//    linearmatd =Normalization(linearmatd);

//    QImage res = linearmatd.toQImage(linearmatd);
//    return res;
}
std::vector<double> imageprocess::createGaussKernel(int size, double sigma)
{
    cout<<"size="<<size<<",sigma="<<sigma<<endl;
    double const PI=3.1415926;
    std::vector<double> ret;
    for (int i=0;i<size;i++)
    {
        for (int j=0;j<size;j++)
        {
            int deltaX = abs(i-size/2);
            int deltaY = abs(j-size/2);
            ret.push_back(exp((-deltaX*deltaX-deltaY*deltaY)/(2*sigma*sigma)));
        }
    }
    cout<< "createGaussKernel"<<endl;
    return ret;
}

QImage imageprocess::bilateralFilter(const QImage& img,int size,double sigmaSpace,double sigmaColor)
{
//*************图像转矩阵***************************
    Matrix<int> matimg(img.height(),img.width(),0);
    matimg = matimg.fromQImage(img,'y');

    cout<<'matimg.getNRow()'<<matimg.getNRow()<<'\n'<<'matimg.getNCol()'<<matimg.getNCol()<<endl;
    cout<<"图像转矩阵："<<'\t'<<endl;
    cout<<matimg<<endl;

//***************padding****************************
    Matrix<double> padded = zeroPadding_M(matimg,size,size);

//**************pre-calculate********************
    std::vector<double> kernelSpace = createGaussKernel(size,sigmaSpace);
    std::vector<double> kernelColor = creatBilaterColorKernel(sigmaColor);
//**************filter************
    int width = padded.getNCol();
    int height = padded.getNRow();
    Matrix<double> filtered(padded);

    for(int i=size/2;i<height-size/2;++i)
       {
            for(int j=size/2; j<width-size/2;++j)
            {
                //std::cout<< i<<"*\t"<<i<<std::endl;
                std::vector<int> patch = getPatch_M_Y(padded,j,i,size,size,'y');
                std::vector<double> kernel;
                for(int k=0;k<kernelSpace.size();++k)
                {
                    int delta = abs(patch[k] - padded(i,j)) ;
                    kernel.push_back(kernelColor[delta]*kernelSpace[k]);
                }
                kernelNormalization(kernel);
                //cout<<"kernelNormalization"<<endl;
                double sum =0;
                for(int k =0;k<kernelSpace.size();++k)
                {
                    sum +=kernel[k]*patch[k];
                }
                int y = (int )sum;
                if(y>255) y=255;
//                filtered.setPixel(i,j, qRgb(y,y,y));
                filtered(i,j) = y;
            }
        }

    Matrix<double> croped  = crop_M(img,filtered,size,size);
//*****************归一化********
    QImage res = croped.toQImage(croped);

    return res;
}
//预先将颜色空间中所有可能的值计算出来,为每一个中心点计算一次Kernel
std::vector<double> imageprocess::creatBilaterColorKernel(double sigma)
{
    cout<<"Bilater Color::sigma="<<sigma<<endl;
    std::vector<double> ret;
    for(int i =0;i<256;++i)
    {
        ret.push_back(exp( (-i*i) / (2*sigma*sigma)) );
    }
    cout<<"creatBilaterColorKernel"<<endl;
    return ret;
}

//------------------------------->
QImage imageprocess::MedianFilter(const QImage& img,int size)
{
    if(0==size) return img;

    Matrix<int> matimg(img.height(),img.width(),0);
    matimg = matimg.fromQImage(img,'y');
    cout<<matimg.getNRow()<<'\n'<<matimg.getNCol()<<endl;

    cout<<"图像转矩阵："<<'\t'<<endl;
    cout<<matimg<<endl;

    //padding
    Matrix<double> padded = zeroPadding_M(matimg,size,size);

    int width = padded.getNCol();
    int height = padded.getNRow();
    for(int i=size/2; i<height-size/2;++i)
    {
        for(int j=size/2;j<width-size/2;++j)
        {
            std::vector<int> patch = getPatch_M_Y(padded,j,i,size,size,'y');
            std::sort(patch.begin() , patch.end());//排序
            int y = patch.at(patch.size()/2);//取中值
            padded(i,j) = y;
        }
    }

    Matrix<double> croped = crop_M(img,padded,size,size);
    QImage ret = croped.toQImage(croped);

    return ret;
}
QImage imageprocess::MedianFilterRGB(const QImage& img,int size)
{
    if(0==size) return img;

       Matrix<int> matimgr(img.height(),img.width(),0);
       Matrix<int> matimgg(img.height(),img.width(),0);
       Matrix<int> matimgb(img.height(),img.width(),0);
       matimgr = matimgr.fromQImage(img,'r');
       matimgg = matimgg.fromQImage(img,'g');
       matimgb = matimgb.fromQImage(img,'b');
       cout<<"-----------原图-------------"<<endl;
//       cout<<matimgr<<endl;
//       四周补零
       Matrix<double> paddedr = zeroPadding_M(matimgr,size,size);
       Matrix<double> paddedg = zeroPadding_M(matimgg,size,size);
       Matrix<double> paddedb = zeroPadding_M(matimgb,size,size);
       cout<<"------------补零--------------------"<<endl;
//       cout<<paddedr<<endl;

       int width = paddedr.getNCol();
       int height = paddedr.getNRow();
       for(int i=size/2; i<height-size/2;++i)
       {
           for(int j=size/2;j<width-size/2;++j)
           {
               std::vector<int> filterR = getPatch_M_R(paddedr,j,i,size,size,'r');
               std::vector<int> filterG = getPatch_M_G(paddedg,j,i,size,size,'g');
               std::vector<int> filterB = getPatch_M_B(paddedb,j,i,size,size,'b');
               std::sort(filterR.begin() , filterR.end());//排序
               std::sort(filterG.begin() , filterG.end());//排序
               std::sort(filterB.begin() , filterB.end());//排序
               int r = filterR.at(filterR.size()/2);//取中值
               int g = filterG.at(filterG.size()/2);//取中值
               int b = filterB.at(filterB.size()/2);//取中值
               paddedr(i,j) = r;
               paddedg(i,j) = g;
               paddedb(i,j) = b;
           }
       }
//       cout<<paddedr<<endl;
       Matrix<double> cropedr = crop_M(img,paddedr,size,size);
       Matrix<double> cropedg = crop_M(img,paddedg,size,size);
       Matrix<double> cropedb = crop_M(img,paddedb,size,size);
       QImage ret = cropedr.toQImage(cropedr,cropedg,cropedb);

       return ret;

}
//------------------------------->
QImage imageprocess::MaximumFilter(const QImage& img,int size)
{
    if(size == 0)
            return img;
        Matrix<int> imgmatY(img.height(),img.width(),0);
        imgmatY = imgmatY.fromQImage(img,'y');
//        cout<<"----------- 原图-------------"<<endl;
//        cout<<imgmatY<<endl;
        //四周补零
        int half = size/2;
        Matrix<double> imgmatY2=zeroPadding_M(imgmatY,size,size);

//        cout<<"------------补零--------------------"<<endl;
//        cout<<imgmatY2<<endl;
        //找范围内的中值
        Matrix<int> resimgY(img.height(),img.width(),0);//存放结果

        for(size_t i=half;i<imgmatY2.getNRow()-half;i++)
        {
            for(size_t j=half;j<imgmatY2.getNCol()-half;j++)
            {
                std::vector<int> filterY=getPatch_M_Y(imgmatY2,j,i,size,size,'y');

                std::sort(filterY.begin(),filterY.end());

                resimgY(i-half,j-half) = filterY.at(filterY.size()-1);//取最大值
            }
        }
//        cout<<"-----------最大值-------------"<<endl;
//        cout<<resimgY<<endl;
        return resimgY.toQImage(resimgY);
}
QImage imageprocess::MaximumFilterRGB(const QImage& img,int size)
{
    if(size == 0)
            return img;
        Matrix<int> imgmatR(img.height(),img.width(),0);
        Matrix<int> imgmatG(img.height(),img.width(),0);
        Matrix<int> imgmatB(img.height(),img.width(),0);
        imgmatR = imgmatR.fromQImage(img,'r');
        imgmatG = imgmatG.fromQImage(img,'g');
        imgmatB = imgmatB.fromQImage(img,'b');
//        cout<<"-----------原图-------------"<<endl;
//        cout<<imgmatR<<endl;
        //四周补零
        int half = size/2;
        Matrix<double> imgmatR2=zeroPadding_M(imgmatR,size,size);
        Matrix<double> imgmatG2=zeroPadding_M(imgmatG,size,size);
        Matrix<double> imgmatB2=zeroPadding_M(imgmatB,size,size);

//        cout<<"------------补零--------------------"<<endl;
//        cout<<imgmatR2<<endl;
        //找范围内的中值
        Matrix<int> resimgR(img.height(),img.width(),0);//存放结果
        Matrix<int> resimgG(img.height(),img.width(),0);//存放结果
        Matrix<int> resimgB(img.height(),img.width(),0);//存放结果

        for(size_t i=half;i<imgmatR2.getNRow()-half;i++)
        {
            for(size_t j=half;j<imgmatR2.getNCol()-half;j++)
            {
                std::vector<int> filterR=getPatch_M_G(imgmatR2,j,i,size,size,'r');
                std::vector<int> filterG=getPatch_M_G(imgmatG2,j,i,size,size,'g');
                std::vector<int> filterB=getPatch_M_B(imgmatB2,j,i,size,size,'b');

                std::sort(filterR.begin(),filterR.end());
                std::sort(filterG.begin(),filterG.end());
                std::sort(filterB.begin(),filterB.end());

                resimgR(i-half,j-half) = filterR.at(filterR.size()-1);//取最大值
                resimgG(i-half,j-half) = filterG.at(filterG.size()-1);//取最大值
                resimgB(i-half,j-half) = filterB.at(filterB.size()-1);//取最大值
            }
        }
//        cout<<"-----------最大值-------------"<<endl;
//        cout<<resimgR<<endl;
        return resimgR.toQImage(resimgR,resimgG,resimgB);
}

//------------------------------->
QImage imageprocess::MinimumFilter(const QImage& img,int size)
{
    if(size == 0)
            return img;
        Matrix<int> imgmatY(img.height(),img.width(),0);
        imgmatY = imgmatY.fromQImage(img,'y');
//        cout<<"----------- 原图-------------"<<endl;
//        cout<<imgmatY<<endl;
        //四周补零
        int half = size/2;
        Matrix<double> imgmatY2=zeroPadding_M(imgmatY,size,size);

//        cout<<"------------补零--------------------"<<endl;
//        cout<<imgmatY2<<endl;
        //找范围内的小值
        Matrix<int> resimgY(img.height(),img.width(),0);//存放结果

        for(size_t i=half;i<imgmatY2.getNRow()-half;i++)
        {
            for(size_t j=half;j<imgmatY2.getNCol()-half;j++)
            {
                std::vector<int> filterY=getPatch_M_Y(imgmatY2,j,i,size,size,'y');

                std::sort(filterY.begin(),filterY.end());

                resimgY(i-half,j-half) = filterY.at(0);//取最小值
            }
        }
//        cout<<"-----------最小值-------------"<<endl;
//        cout<<resimgY<<endl;
        return resimgY.toQImage(resimgY);
}
QImage imageprocess::MinimumFilterRGB(const QImage& img,int size)
{
    if(size == 0)
            return img;
        Matrix<int> imgmatR(img.height(),img.width(),0);
        Matrix<int> imgmatG(img.height(),img.width(),0);
        Matrix<int> imgmatB(img.height(),img.width(),0);
        imgmatR = imgmatR.fromQImage(img,'r');
        imgmatG = imgmatG.fromQImage(img,'g');
        imgmatB = imgmatB.fromQImage(img,'b');
//        cout<<"-----------原图-------------"<<endl;
//        cout<<imgmatR<<endl;
        //四周补零
        int half = size/2;
        Matrix<double> imgmatR2=zeroPadding_M(imgmatR,size,size);
        Matrix<double> imgmatG2=zeroPadding_M(imgmatG,size,size);
        Matrix<double> imgmatB2=zeroPadding_M(imgmatB,size,size);

//        cout<<"------------补零--------------------"<<endl;
//        cout<<imgmatR2<<endl;
        //找范围内的最小值
        Matrix<int> resimgR(img.height(),img.width(),0);//存放结果
        Matrix<int> resimgG(img.height(),img.width(),0);//存放结果
        Matrix<int> resimgB(img.height(),img.width(),0);//存放结果

        for(size_t i=half;i<imgmatR2.getNRow()-half;i++)
        {
            for(size_t j=half;j<imgmatR2.getNCol()-half;j++)
            {
                std::vector<int> filterR=getPatch_M_G(imgmatR2,j,i,size,size,'r');
                std::vector<int> filterG=getPatch_M_G(imgmatG2,j,i,size,size,'g');
                std::vector<int> filterB=getPatch_M_B(imgmatB2,j,i,size,size,'b');

                std::sort(filterR.begin(),filterR.end());
                std::sort(filterG.begin(),filterG.end());
                std::sort(filterB.begin(),filterB.end());

                resimgR(i-half,j-half) = filterR.at(0);//取最小值
                resimgG(i-half,j-half) = filterG.at(0);//取最小值
                resimgB(i-half,j-half) = filterB.at(0);//取最小值
            }
        }
//        cout<<"-----------最小值-------------"<<endl;
//        cout<<resimgR<<endl;
        return resimgR.toQImage(resimgR,resimgG,resimgB);
}

QImage imageprocess::imgfft2d(const QImage& img)
{

    int h = img.height();
    int w = img.width();
    Matrix<int> mat (h,w,0);

    mat = mat.fromQImage(img,'y');
    cout<<"Image to Matrix \t"<< mat <<endl;

    Matrix<complex<double> > matfft= fft2d(mat,mat.getNRow(),mat.getNCol());//FFT
    cout<<"after fft2"<<'\n'<<matfft<<endl;

    int height = matfft.getNRow();
    int width =matfft.getNCol();

    Matrix<double> matlog(height,width,0.0);
    for(int i=0;i<height;++i)
    {
         for(int j=0;j<width;++j)
         {
            std::complex<double> temp=matfft(i,j);
            double gray=sqrt(pow(temp.real(),2)+pow(temp.imag(),2));//取模

            //cout<<gray<<'\t'<<endl;
            gray = log(1+qAbs(gray));         //对数
            //double gray = abs(matfft(i,j));
            matlog(i,j) = gray;
         }
    }

    cout<<"log之后的矩阵"<<endl;
    cout<<matlog<<endl;
//---------------fftshift------------------------
    matlog._startR = matlog.getNRow() / 2;
    matlog._startC = matlog.getNCol() / 2;

//    cout<<"fftshift "<<endl;
   // matlog = matlog.fftshift();
   // cout<<matlog<<endl;

//----------归一化-----------------
   double min = 100000.0;
    double max = -1.0;
    for(int i=0;i<height;i++)
      {
            for(int j=0;j<width;j++)
            {
                if(matlog(i,j) > max)
                    max = matlog(i,j);
                if(matlog(i,j) < min)
                    min = matlog(i,j);
            }
      }
    for(int i=0;i<height;i++)
       {
            for(int j=0;j<width;j++)
            {
                matlog(i,j) = (matlog(i,j) - min) / (max - min) * 255;//归一化
            }
       }


    cout<<"result "<<'\n'<<matlog<<endl;
    QImage res = matlog.toQImage(matlog);
    return res;
}


QImage imageprocess::imgIfft2d(const QImage& img)
{

    int h = img.height();
    int w = img.width();
    Matrix<int> mat (h,w,0);
    int height =calcN(mat.getNRow());
    int width =calcN(mat.getNCol());

    mat = mat.fromQImage(img,'y');
    cout<<"Image to Matrix \t"<< mat <<endl;

    Matrix<complex<double> > matfft=
            ifft2d(mat,h,w);
    cout<<"after fft2"<<'\n'<<matfft<<endl;

//    int height = matfft.getNRow();
//    int width =matfft.getNCol();
//    matfft = ifft2d(matfft,height,width);   //IFFT
    Matrix<double> matlog(height,width,0.0);
    for(int i=0;i<height;++i)
    {
         for(int j=0;j<width;++j)
         {
            std::complex<double> temp=matfft(i,j);
            double gray=sqrt(pow(temp.real(),2)+pow(temp.imag(),2));//取模

            //cout<<gray<<'\t'<<endl;
            //gray = log(1+qAbs(gray));         //对数
            //double gray = abs(matfft(i,j));
            matlog(i,j) = gray;
         }
    }

    cout<<"log之后的矩阵"<<endl;
    cout<<matlog<<endl;
//---------------fftshift------------------------
    matlog._startR = 0;
    matlog._startC = 0;

    //cout<<"fftshift "<<endl;
   // matlog = matlog.fftshift();
    //cout<<matlog<<endl;

//----------归一化-----------------
   double min = 100000.0;
    double max = -1.0;
    for(int i=0;i<height;i++)
      {
            for(int j=0;j<width;j++)
            {
                if(matlog(i,j) > max)
                    max = matlog(i,j);
                if(matlog(i,j) < min)
                    min = matlog(i,j);
            }
      }
    for(int i=0;i<height;i++)
       {
            for(int j=0;j<width;j++)
            {
                matlog(i,j) = (matlog(i,j) - min) / (max - min) * 255;//归一化
            }
       }


    cout<<"result "<<'\n'<<matlog<<endl;

    QImage res = matlog.toQImage(matlog);
    return res;
}

/**********************仿射变换*****************************/
QImage imageprocess::Transform(const QImage& img,const QMatrix & mat)
{
     int r = img.height();
     int c = img.width();
    Matrix<int> matimgr(r,c,0);
    Matrix<int> matimgg(r,c,0);
    Matrix<int> matimgb(r,c,0);
//    Matrix<int> matimgy(r,c,0);
    matimgr = matimgr.fromQImage(img,'r');
    matimgg = matimgr.fromQImage(img,'g');
    matimgb = matimgr.fromQImage(img,'b');
//    matimgy = matimgr.fromQImage(img,'y');

    //判断 QMatrix 是否可逆
        bool invert = true;
        QMatrix inv_mat = mat.inverted(&invert);  //确认变换矩阵可逆
        cout<<"**确认变换矩阵是否可逆 Invert  = *****"<<invert<<endl;

        if (!invert)    return img;//无法求逆，直接返回原图像

        // 获取新图像的大小
        double minX, maxX, minY, maxY;
        getRange_M(matimgr,mat, minX, maxX, minY, maxY);//通过变换矩阵确定图像大小 minX maxX minY maxY


        //构建新图像(这一步默认做了一个平移 (minX, minY))，
        int newHeight = ceil(maxX-minX);
        int newWidth = ceil(maxY-minY);
        cout<<"*******newWidth = "<<newWidth <<endl;
        cout<<"*******newHeight = "<<newHeight <<endl;
//        QImage newImg( newWidth, newHeight, img.format());

        Matrix<int> newImgr(newHeight,newWidth,0);
        Matrix<int> newImgg(newHeight,newWidth,0);
        Matrix<int> newImgb(newHeight,newWidth,0);

        //遍历新图像，对每个像素坐标求出其在原图中的坐标
        for(int i=0;i<newHeight;++i)
        {
            for(int j=0;j<newWidth;++j)
            {
                double r = getNewRgb_M(matimgr,inv_mat, i, j, minX, minY);
                double g = getNewRgb_M(matimgg,inv_mat, i, j, minX, minY);
                double b = getNewRgb_M(matimgb,inv_mat, i, j, minX, minY);
                newImgr(i,j)=(int)r;
                newImgg(i,j)=(int)g;
                newImgb(i,j)=(int)b;
            }
        }
        return newImgr.toQImage(newImgr,newImgg,newImgb);
    }

void imageprocess::getRange(const QImage& img,const QMatrix &mat, double& minX,
                                  double &maxX, double& minY, double& maxY)
{

        cout<<"*********getRange******"<<endl;

        //四个顶点（将原图四个角映射到新图像坐标系中）
        vector<qreal> vx(4), vy(4);//
         mat.map(0,0,&vx[0],&vy[0]);//对输入的坐标做映射 映射回原图像
         mat.map(0,img.height()-1,&vx[1],&vy[1]);
        //mat.map(0,img.width()-1,&vx[2],&vy[2]);
         mat.map(img.width()-1,0,&vx[2],&vy[2]);
         mat.map(img.width()-1,img.height()-1,&vx[3],&vy[3]);

        //求出minX maxX minY maxY

         //maxX
         if(vx[0]>=vx[1]&&vx[0]>=vx[2]&&vx[0]>=vx[3])
         {
             maxX = vx[0];
         }
         else if(vx[1]>=vx[0]&&vx[1]>=vx[2]&&vx[1]>=vx[3])
         {
             maxX = vx[1];
         }
         else if(vx[2]>=vx[0]&&vx[2]>=vx[1]&&vx[2]>=vx[3])
         {
             maxX = vx[2];
         }
         else
         {
             maxX = vx[3];
         }
    //minX
         if(vx[0]<=vx[1]&&vx[0]<=vx[2]&&vx[0]<=vx[3])
         {
             minX = vx[0];
         }
         else if(vx[1]<=vx[0]&&vx[1]<=vx[2]&&vx[1]<=vx[3])
         {
             minX = vx[1];
         }
         else if(vx[2]<=vx[0]&&vx[2]<=vx[1]&&vx[2]<=vx[3])
         {
             minX = vx[2];
         }
         else
         {
             minX = vx[3];
         }
    //maxY
         if(vy[0]>=vy[1]&&vy[0]>=vy[2]&&vy[0]>=vy[3])
         {
             maxY = vy[0];
         }
         else if(vy[1]>=vy[0]&&vy[1]>=vy[2]&&vy[1]>=vy[3])
         {
             maxY = vy[1];
         }
         else if(vy[2]>=vy[0]&&vy[2]>=vy[1]&&vy[2]>=vy[3])
         {
             maxY = vy[2];
         }
         else
         {
             maxY = vy[3];
         }
     //minY
         if(vy[0]<=vy[1]&&vy[0]<=vy[2]&&vy[0]<=vy[3])
         {
             minY = vy[0];
         }
         else if(vy[1]<=vy[0]&&vy[1]<=vy[2]&&vy[1]<=vy[3])
         {
             minY = vy[1];
         }
         else if(vy[2]<=vy[0]&&vy[2]<=vy[1]&&vy[2]<=vy[3])
         {
             minY = vy[2];
         }
         else
         {
             minY = vy[3];
         }
         cout<<"*******minX = "<<minX <<"********minY = "<<minY<<endl;
         cout<<"*******maxX = "<<maxX <<"********maxY = "<<maxY<<endl;
}

void imageprocess::getRange_M(Matrix<int>& img,const QMatrix &mat, double& minX,
                                  double &maxX, double& minY, double& maxY)
{

        cout<<"*********getRange******"<<endl;

        //四个顶点（将原图四个角映射到新图像坐标系中）
        vector<qreal> vx(4), vy(4);//
         mat.map(0,0,&vx[0],&vy[0]);//对输入的坐标做映射 映射回原图像
         mat.map(img.getNRow()-1,0,&vx[1],&vy[1]);
        //mat.map(0,img.width()-1,&vx[2],&vy[2]);
         mat.map(0,img.getNCol()-1,&vx[2],&vy[2]);
         mat.map(img.getNRow()-1,img.getNCol()-1,&vx[3],&vy[3]);

        //求出minX maxX minY maxY

         //maxX
         if(vx[0]>=vx[1]&&vx[0]>=vx[2]&&vx[0]>=vx[3])
         {
             maxX = vx[0];
         }
         else if(vx[1]>=vx[0]&&vx[1]>=vx[2]&&vx[1]>=vx[3])
         {
             maxX = vx[1];
         }
         else if(vx[2]>=vx[0]&&vx[2]>=vx[1]&&vx[2]>=vx[3])
         {
             maxX = vx[2];
         }
         else
         {
             maxX = vx[3];
         }
    //minX
         if(vx[0]<=vx[1]&&vx[0]<=vx[2]&&vx[0]<=vx[3])
         {
             minX = vx[0];
         }
         else if(vx[1]<=vx[0]&&vx[1]<=vx[2]&&vx[1]<=vx[3])
         {
             minX = vx[1];
         }
         else if(vx[2]<=vx[0]&&vx[2]<=vx[1]&&vx[2]<=vx[3])
         {
             minX = vx[2];
         }
         else
         {
             minX = vx[3];
         }
    //maxY
         if(vy[0]>=vy[1]&&vy[0]>=vy[2]&&vy[0]>=vy[3])
         {
             maxY = vy[0];
         }
         else if(vy[1]>=vy[0]&&vy[1]>=vy[2]&&vy[1]>=vy[3])
         {
             maxY = vy[1];
         }
         else if(vy[2]>=vy[0]&&vy[2]>=vy[1]&&vy[2]>=vy[3])
         {
             maxY = vy[2];
         }
         else
         {
             maxY = vy[3];
         }
     //minY
         if(vy[0]<=vy[1]&&vy[0]<=vy[2]&&vy[0]<=vy[3])
         {
             minY = vy[0];
         }
         else if(vy[1]<=vy[0]&&vy[1]<=vy[2]&&vy[1]<=vy[3])
         {
             minY = vy[1];
         }
         else if(vy[2]<=vy[0]&&vy[2]<=vy[1]&&vy[2]<=vy[3])
         {
             minY = vy[2];
         }
         else
         {
             minY = vy[3];
         }
         cout<<"*******minX = "<<minX <<"********minY = "<<minY<<endl;
         cout<<"*******maxX = "<<maxX <<"********maxY = "<<maxY<<endl;
}
QRgb imageprocess::getNewRgb(const QImage& img,const QMatrix &mat, int i, int j
                                   ,double minX, double minY)
    {
         //cout<<"*********getNewRgb******"<<endl;

        // get the position in origal image获取原图的位置
         double srcX,srcY;
         mat.map(i+minX,j+minY,&srcX,&srcY);//平移回去

        //超出原图 用0填充
       if(srcX<0||srcX>img.width()-1||srcY<0||srcY>img.height()-1)
       {
           //cout<<"************超出原图范围********"<<endl;
           return qRgb(0,0,0);
       }

        //通过双线性插值确定像素值
        else
       {
           return bilinear(img,srcX,srcY);//通过双线性插值确定像素值
       }

    }
double imageprocess::getNewRgb_M(Matrix<int>& img,const QMatrix &mat, int i, int j
                                   ,double minX, double minY)
    {
         //cout<<"*********getNewRgb******"<<endl;

        // get the position in origal image获取原图的位置
         double srcX,srcY;
         mat.map(i+minX,j+minY,&srcX,&srcY);//平移回去

        //超出原图 用0填充
       if(srcX<0||srcX>img.getNRow()-1||srcY<0||srcY>img.getNCol()-1)
       {
           //cout<<"************超出原图范围********"<<endl;
           return qRgb(0,0,0);
       }

        //通过双线性插值确定像素值
        else
       {
           return bilinear_M(img,srcX,srcY);//通过双线性插值确定像素值
       }

    }

//双线性插值  通过双线性插值计算目标图像像素点的值。
QRgb imageprocess::bilinear(const QImage& img,double x, double y)
{
        // cout<<"*********bilinear******"<<endl;

        //获取周围的四个像素
        int x1=ceil(x);
        int x2=floor(x);
        int y1=ceil(y);
        int y2=floor(y);
        QRgb rgb1=img.pixel(x1,y1);
        QRgb rgb2=img.pixel(x1,y2);
        QRgb rgb3=img.pixel(x2,y1);
        QRgb rgb4=img.pixel(x2,y2);

        //双线性插值red
        double tmp1,tmp2;
        tmp1=linear_inter(qRed(rgb1),qRed(rgb2),y1,y2,y);
        tmp2=linear_inter(qRed(rgb3),qRed(rgb4),y1,y2,y);
        double r=linear_inter(tmp1,tmp2,x1,x2,x);


        //双线性插值greed
        tmp1=linear_inter(qGreen(rgb1),qGreen(rgb2),y1,y2,y);
        tmp2=linear_inter(qGreen(rgb3),qGreen(rgb4),y1,y2,y);
        double g=linear_inter(tmp1,tmp2,x1,x2,x);

        //双线性差值blue
        tmp1=linear_inter(qBlue(rgb1),qBlue(rgb2),y1,y2,y);
        tmp2=linear_inter(qBlue(rgb3),qBlue(rgb4),y1,y2,y);
        double b=linear_inter(tmp1,tmp2,x1,x2,x);


        return qRgb(r,g,b);

}

double imageprocess::bilinear_M(Matrix<int>& img,double x, double y)
{
        // cout<<"*********bilinear******"<<endl;

        //获取周围的四个像素
        int x1=ceil(x);
        int x2=floor(x);
        int y1=ceil(y);
        int y2=floor(y);
        double rgb1=img(x1,y1);
        double rgb2=img(x1,y2);
        double rgb3=img(x2,y1);
        double rgb4=img(x2,y2);

        //双线性插值red
        double tmp1,tmp2;
        tmp1=linear_inter(rgb1,rgb2,y1,y2,y);
        tmp2=linear_inter(rgb3,rgb4,y1,y2,y);
        double r=linear_inter(tmp1,tmp2,x1,x2,x);


//        //双线性插值greed
//        tmp1=linear_inter(rgb1,rgb2,y1,y2,y);
//        tmp2=linear_inter(rgb3,rgb4,y1,y2,y);
//        double g=linear_inter(tmp1,tmp2,x1,x2,x);

//        //双线性差值blue
//        tmp1=linear_inter(rgb1,rgb2,y1,y2,y);
//        tmp2=linear_inter(rgb3,rgb4,y1,y2,y);
//        double b=linear_inter(tmp1,tmp2,x1,x2,x);


        return r;

}

double imageprocess::linear_inter(double v1, double v2, double c1,
                                        double c2, double c3)
{
    // cout<<"*********linear_inter******"<<endl;
    if(c1==c2)return v1;
    //根据公式计算
    return (c2-c3)/(c2-c1)*v1+(c3-c1)/(c2-c1)*v2;
}

//--------------------时域-----低通滤波器----------------------------//
QImage imageprocess::LowPassFilterS(const QImage& img ,Matrix<double>& mat)
{
    int h = img.height();
    int w = img.width();
    Matrix<int> mati (h,w,0);
//***********************图像转矩阵**************************
    mati = mati.fromQImage(img,'y');
    cout<<"Image to Matrix******"<<mati<<endl;

    Matrix<double> matimg(h,w,0);
    for(int i=0;i<h;++i)
    {
        for(int j=0;j<w;++j)
        {
            matimg(i,j) = (double)mati(i,j);
        }
    }
    Matrix<double> res = matimg.conv(matimg,mat);

    cout<<"***********res**********"<<'\t'<<endl;
    cout<<res<<endl;
    res = Normalization(res);

//***********************图像与滤波器卷积**************************
//    Matrix<double> res (mati);
//    Matrix<double> padded = zeroPadding_M(mati,mat.getNCol(),mat.getNRow());
//    Matrix<double> res = conv2(padded,mat,mat.getNCol(),mat.getNRow());
//    Matrix<double> croped = crop_M(img,res,mat.getNCol(),mat.getNRow());

//***********************结果归一化**************************
//    res = Normalization(croped);
    cout<<"********归一化***res**********"<<'\t'<<endl;
    cout<<res<<endl;
//***********************矩阵转图像**************************
    QImage resimg = res.toQImage(res);

    return resimg;
}

//--------------------频域-----低通滤波器----------------------------//
QImage imageprocess::LowpassFilterF(const QImage& img,Matrix<double>& mat)
//LowPassFilterF(const QImage& img ,Matrix<int>& mat)
{
    int h = img.height();//A
    int w = img.width();//B

    cout<<"h="<<h<<endl;
    cout<<"w="<<w<<endl;

    Matrix<int> mati (h,w,0);

    int r = mat.getNRow();//C
    int c = mat.getNCol();//D
    cout<<"r="<<r<<endl;
    cout<<"c="<<c<<endl;

    int p = h+r-1;
    int q = w+c-1;

    cout<<"p="<<p<<endl;
    cout<<"q="<<q<<endl;

//***********************图像转矩阵**************************
    mati = mati.fromQImage(img,'y');
    cout<<"********* 图像转矩阵******"<<'\t'<<endl;
    cout<<mati<<endl;


//***********************图像补零**************************
    Matrix<double> resi (p,q,0);
//    cout<<"********* 图像转矩阵,补零******"<<'\t'<<endl;
//    cout<<resi<<endl;

    for(int i= 0; i<h; ++i)
    {
        for(int j=0 ; j<w; ++j)
        {
//            cout<<"i"<<i<<"j"<<j<<endl;
            resi(i,j)= mati(i,j);
        }
    }
    cout<<"********* 补零后的图像******"<<'\t'<<endl;
    cout<<resi<<endl;

//***********************滤波器补零**************************
    Matrix<double> resm (p,q,0);
    cout<<"********* 矩阵,补零******"<<'\t'<<endl;
    cout<<resm<<endl;
    for(int i= 0;i<r;++i)
    {
        for(int j=0;j<c;++j)
        {
            resm(i,j)= mat(i,j);
        }
    }
    cout<<"********* 补零后的矩阵******"<<'\t'<<endl;
    cout<<resm<<endl;
//***********************图像和滤波器都做fft2d**************************
    Matrix<complex<double> > resifft(p,q,complex<double>(0,0));
    Matrix<complex<double> > resmfft(p,q,complex<double>(0,0));
    resifft = fft2d(resi,p,q);
    resmfft = fft2d(resm,p,q);

    cout<<"********* 补零后的fft图像******"<<'\t'<<endl;
    cout<<resifft<<endl;
    cout<<"********* 补零后的fft矩阵******"<<'\t'<<endl;
    cout<<resmfft<<endl;
//***********************频域：图像与滤波器点乘：**************************
    Matrix<complex<double> > res(p,q,complex<double>(0,0));
    res = res.dotmul(resifft,resmfft);

    cout<<"********* 点乘后******"<<'\t'<<endl;
    cout<<res<<endl;
//***********************转回时域：对点乘做ifft2d**************************
    p = calcN(p);
    q = calcN(q);
    cout<<"p="<<p<<",q="<<q<<endl;
    res = ifft2d(res,p,q);
    cout<<"********* ifft******"<<'\t'<<endl;
    cout<<res<<endl;
//***********************取实部**************************
    Matrix<double> resa(h,w,0);
    for(int i =0;i<h;++i)
    {
        for(int j=0;j<w;++j)
        {
            resa(i,j) = res(i,j).real();
        }
    }
    cout<<"*********点乘、ifft后、取实部的结果********"<<'\t'<<endl;
    cout<<resa<<endl;
//***********************归一化**************************
    resa = Normalization(resa);
    cout<<"********归一化***resa**********"<<'\t'<<endl;
    cout<<resa<<endl;
//***********************矩阵转图像**************************
    QImage resimg = resa.toQImage(resa);

    return resimg;
}

//--------------------时域-----高通滤波器----------------------------//
QImage imageprocess::HighPassFilterS(const QImage& img ,Matrix<double>& mat)
{
    int h = img.height();
    int w = img.width();
    Matrix<int> mati (h,w,0);
//***********************图像转矩阵**************************
    mati = mati.fromQImage(img,'y');
    cout<<"Image to Matrix******"<<mati<<endl;

    Matrix<double> matimg(h,w,0);
    for(int i=0;i<h;++i)
    {
        for(int j=0;j<w;++j)
        {
            matimg(i,j) = (double)mati(i,j);
        }
    }

//***********************图像与滤波器卷积**************************
    Matrix<double> res (h,w,0);
    res = res.conv(matimg,mat);

    cout<<"***********res**********"<<'\t'<<endl;
    cout<<res<<endl;

//***********************结果归一化**************************
    res = Normalization(res);
    cout<<"********归一化***res**********"<<'\t'<<endl;
    cout<<res<<endl;
//***********************矩阵转图像**************************
    QImage resimg = res.toQImage(res);

    return resimg;
}

//--------------------频域-----高通滤波器----------------------------//
QImage imageprocess::HighpassFilterF(const QImage& img,Matrix<double>& mat)
//LowPassFilterF(const QImage& img ,Matrix<int>& mat)
{
    int h = img.height();//A
    int w = img.width();//B

    cout<<"h="<<h<<endl;
    cout<<"w="<<w<<endl;

    Matrix<int> mati (h,w,0);

    int r = mat.getNRow();//C
    int c = mat.getNCol();//D
    cout<<"r="<<r<<endl;
    cout<<"c="<<c<<endl;

    int p = h+r-1;
    int q = w+c-1;

    cout<<"p="<<p<<endl;
    cout<<"q="<<q<<endl;

//***********************图像转矩阵**************************
    mati = mati.fromQImage(img,'y');
    cout<<"********* 图像转矩阵******"<<'\t'<<endl;
    cout<<mati<<endl;


//***********************图像补零**************************
    Matrix<double> resi (p,q,0);
//    cout<<"********* 图像转矩阵,补零******"<<'\t'<<endl;
//    cout<<resi<<endl;

    for(int i= 0; i<h; ++i)
    {
        for(int j=0 ; j<w; ++j)
        {
//            cout<<"i"<<i<<"j"<<j<<endl;
            resi(i,j)= mati(i,j);
        }
    }
    cout<<"********* 补零后的图像******"<<'\t'<<endl;
    cout<<resi<<endl;

//***********************滤波器补零**************************
    Matrix<double> resm (p,q,0);
    cout<<"********* 矩阵,补零******"<<'\t'<<endl;
    cout<<resm<<endl;
    for(int i= 0;i<r;++i)
    {
        for(int j=0;j<c;++j)
        {
            resm(i,j)= mat(i,j);
        }
    }
    cout<<"********* 补零后的矩阵******"<<'\t'<<endl;
    cout<<resm<<endl;
//***********************图像和滤波器都做fft2d**************************
    Matrix<complex<double> > resifft(p,q,complex<double>(0,0));
    Matrix<complex<double> > resmfft(p,q,complex<double>(0,0));
    resifft = fft2d(resi,p,q);
    resmfft = fft2d(resm,p,q);

    cout<<"********* 补零后的fft图像******"<<'\t'<<endl;
    cout<<resifft<<endl;
    cout<<"********* 补零后的fft矩阵******"<<'\t'<<endl;
    cout<<resmfft<<endl;
//***********************频域：图像与滤波器点乘：**************************
    Matrix<complex<double> > res(p,q,complex<double>(0,0));
    res = res.dotmul(resifft,resmfft);

    cout<<"********* 点乘后******"<<'\t'<<endl;
    cout<<res<<endl;
//***********************转回时域：对点乘做ifft2d**************************
    p = calcN(p);
    q = calcN(q);
    cout<<"p="<<p<<",q="<<q<<endl;
    res = ifft2d(res,p,q);
    cout<<"********* ifft******"<<'\t'<<endl;
    cout<<res<<endl;
//***********************取实部**************************
    Matrix<double> resa(h,w,0);
    for(int i =0;i<h;++i)
    {
        for(int j=0;j<w;++j)
        {
            resa(i,j) = res(i,j).real();
        }
    }
    cout<<"*********点乘、ifft后、取实部的结果********"<<'\t'<<endl;
    cout<<resa<<endl;
//***********************归一化**************************
    resa = Normalization(resa);
    cout<<"********归一化***resa**********"<<'\t'<<endl;
    cout<<resa<<endl;
//***********************矩阵转图像**************************
    QImage resimg = resa.toQImage(resa);

    return resimg;
}

Matrix<double> imageprocess::Normalization(Matrix<double> & mat)
{
     int height = mat.getNRow();
     int width = mat.getNCol();
     double min = 100000.0;
     double max = -1.0;
     for(int i=0;i<height;i++)
       {
             for(int j=0;j<width;j++)
             {
                 if(mat(i,j) > max)
                     max = mat(i,j);
                 if(mat(i,j) < min)
                     min = mat(i,j);
             }
       }
     for(int i=0;i<height;i++)
        {
             for(int j=0;j<width;j++)
             {
                 mat(i,j) = (mat(i,j) - min) / (max - min) * 255;//归一化
             }
        }
     return mat;

}

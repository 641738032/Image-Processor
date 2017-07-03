//
//  matrix.h
//  matrix02
//
//  Created by 朱小琳 on 2017/4/27.
//  Copyright © 2017年 朱小琳. All rights reserved.
//

#ifndef matrix_h
#define matrix_h

#include <iostream>
#include <vector>
#include <stdexcept>
#include<QImage>
#include <complex>
#include <vector>

#include <QtWidgets/QApplication>
#include <QDebug>
#include <QtCore/qmath.h>

using std::range_error;
using std::vector;
using std::cout;
using std::endl;
using std::ostream;

template<typename T>//double 改成T
class Matrix
{
private:
    T** _data;// point to the data
    
    size_t _nRow, _nCol;// number of row and column
    
    // flag to indicate row-first or column-first
    int _t;
    bool checkRange(size_t r, size_t c) const;
    void swap(Matrix<T>& mat);//Matrix<T>

public:

    int _startR=0;
    int _startC=0;
    Matrix(size_t r, size_t c, T init);// construct a r-row and c-column matrix with all init element
    
    Matrix(const Matrix<T>& mat);// copy constructor
    
    Matrix& operator=(Matrix<T> mat);
    ~Matrix();
    
    // get element at r-row and c-column
    // return reference to make it modifiable
    T& operator()(size_t r, size_t c) const;
    
    // output the matrix
    template<typename U>//都用T太乱
    friend std::ostream& operator<<(std::ostream& out, const Matrix<U>& rhs);//template 用U Matrix<U>&
    
    size_t getNRow() const;
    size_t getNCol() const;
    
    Matrix<T> subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const;//行始  行末  列始 列末
    
    std::vector<T> getRow(size_t r) const;
    std::vector<T> getCol(size_t c) const;
    
    bool setRow(size_t r, const std::vector<T>& d);
    bool setCol(size_t c, const std::vector<T>& d);
    bool uset();
    
    Matrix<T>& transpose();// transpose it in-place

    static Matrix<int> fromQImage(const QImage&,char);
    static QImage toQImage(const Matrix<T>&);
    static QImage toQImage(const Matrix<T>&,const Matrix<T>&,const Matrix<T>&);

//    // circular shift in row or column direction
//       void rowCyclicShift(int r);
//       void colCyclicShift(int c);
//       // fftshift
//       Matrix<T> fftshift();

       //***************点乘*******************
       Matrix<T> dotmul(Matrix<T>& mat1,Matrix<T>& mat2);

       //***************卷积*******************
       Matrix<T> conv(Matrix<T>& m1,Matrix<T>& m2);

       Matrix<T> zeroPadding_M(Matrix<T> & ,int ,int);
       Matrix<T> Duplicate_M(Matrix<T>&,int,int );

/********************矩阵求逆************************************/
       Matrix<T> inverse(Matrix<T>& m);
/********************矩阵求逆************************************/
    
/*******************************************************************************
     addiction
******************************************************************************/
    Matrix<T> operator+(Matrix<T>);//operator (matrix m1 + matrix m2)
    Matrix<T> operator+(T);//operator(matrix m + number)
    friend Matrix<T> operator+(T t,Matrix<T> mat) //number + matrix m
    {
        return mat.operator+(t);
    }
    Matrix<T> addmatrix(Matrix<T>& mat1,Matrix<T> mat2);//matrix m1 + matrix m2
    Matrix<T> addnum(Matrix<T>& mat,T t);//matrix m + number
    
    /*Matrix<T> operator+(const Matrix<T>& );//m+mC
    Matrix<T> operator+(const T& );//mC+9
    template<typename V>
    friend Matrix<V> operator+(const V& t,const Matrix<V>& rhs)//9+mC
    {
        return  rhs.operator+( t );
    }
    */
    
/*******************************************************************************
     substraction
******************************************************************************/
    
    Matrix<T> operator-(Matrix<T>);
    Matrix<T> operator-(T);
    friend Matrix<T> operator-(T t,Matrix<T> mat)
    {
        Matrix<T> m = mat.operator-(t);
        for(size_t i=0; i<m.getNRow(); i++)
        {
            for(size_t j=0; j<m.getNCol(); j++)
            {
                m(i,j) = 0 - m(i,j);
            }
        }
        return m;
    }
    Matrix<T> subtractmatrix(Matrix<T>&,Matrix<T>);
    Matrix<T> subtractnum(Matrix<T>&,T);
/*******************************************************************************
     multiplication
******************************************************************************/
    
    Matrix<T> operator*(Matrix<T>);
    Matrix<T> operator*(T);
    friend Matrix<T> operator*(T t,Matrix<T> mat)
    {
        return mat.operator*(t);
    }
    Matrix<T> multimatrix(Matrix<T>& mat1,Matrix<T> mat2);
    Matrix<T> multinum(Matrix<T>& mat,T t);
    
  
/*******************************************************************************
     multiplication
******************************************************************************/
    
    Matrix<T> operator/(const T&);
    
};



/*******************************************************************************
 -------------------------------------------------------------------------------
 ******************************************************************************/
template<typename T>
Matrix<T>::Matrix(size_t r, size_t c, T init):
_nRow(r), _nCol(c), _t(0)
{
    // new _data
    _t=0;
    _data = new T * [r];// 每个指针指向一个指针数组
    for(size_t i = 0; i<r ; i++)
    {
        _data[i] = new T[c];//这个指针数组的每个指针元素又指向一个数组 创建一个 r行 c列的数组
    }
    // new _data[i] within loop
    for( size_t i =0; i<r;i++)
    {
        for(size_t j = 0; j <c ;j++)
        {
            _data[i][j] = init;//初始化数组元素
            //cout<<"初始化"<<endl;
        }
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& mat):
_nRow(mat._nRow), _nCol(mat._nCol), _t(mat._t)
{
    // new _data
    //double ** _data;
    _data = new T *[_nRow];
    for (size_t i = 0;i<_nRow;i++)
    {
        _data [i] = new T [_nCol];
    }
    // new and assign _data[i] within loop
    for(size_t i =0;i<_nRow;i++)
    {
        for(size_t j = 0;j<_nCol;j++)
        {
            _data[i][j] = mat(i,j);
        }
    }
}

template<typename T>
void Matrix<T>::swap(Matrix<T>& mat) {
    std::swap(_data, mat._data);
    std::swap(_nRow, mat._nRow);
    std::swap(_nCol, mat._nCol);
    std::swap(_t, mat._t);
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T> mat)
{
    swap(mat);
    return *this;
}

template<typename T>
size_t Matrix<T>::getNRow() const
{
    // return the number of row
        return _nRow;
}

template<typename T>
size_t Matrix<T>::getNCol() const
{
    // return the number of column
        return _nCol;
}

template<typename T>
bool Matrix<T>::checkRange(size_t r, size_t c) const
{
    // check if r-row and c-column is out of range
    if(r < 0||r>=_nRow ||c < 0|| c>=_nCol)
    {
        return false;
    }
    else
    {
         return  true;
    }
}


template<typename T>
T& Matrix<T>::operator()(size_t r, size_t c) const
{
    if (!checkRange(r,c))
    {
        throw range_error("in operator(), row or col are out of range!");
    }
    if(0==_t)
        return _data[(r+_startR)%_nRow][(c+_startC)%_nCol];
    else
        return _data[(c+_startC)%_nCol][(r+_startR)%_nRow];
}

template<typename T>
std::vector<T> Matrix<T>::getRow(size_t r) const
{
    // get all element in r-row and push it back in a vector
    //获取r行中的所有元素并将其推回到向量中
    vector<T> vrow;
    if(!checkRange(r,0))
        throw range_error("in getRow,r is out of range!");
    for(size_t i = 0;i<_nCol;i++)
    {
        vrow.push_back(operator()(r,i));
    }
    // remember check range first记住检查范围先
    return vrow;
}

template<typename T>
std::vector<T> Matrix<T>::getCol(size_t c) const
{
    // get all element in c-column and push it back in a vector
    vector<T> vcol;
    if(!checkRange(0,c))
        throw range_error("in getCol,c is out of range!");
    for(size_t i = 0;i<_nRow;i++)
    {
        vcol.push_back(operator()(i,c));
    }
    // remember check range first
    return vcol;
}

template<typename T>
bool Matrix<T>::setRow(size_t r, const std::vector<T>& d)
{
    // set the element of the r-row
    // remember check range first
    /*if(!checkRange(r,0) || d.size()!=_nRow)
    {
        throw range_error("in setRow,out of range!");
    }
    for(size_t i=0;i<_nCol;i++){
        operator()(r,i)=d[i];
    }
    
    return true;*/
    if(_t == 0 )
    {
        if(!checkRange(r,0))
        {
            throw range_error("setRow():r-row is out of range");
        }
        if(d.size() != _nCol)
        {
            throw range_error("setRow():d.size does not match with col");
        }
        else
        {
            for(size_t i = 0; i < _nCol; i++)
            {
                _data[r][i] = d[i];
            }
            return true;
        }
    }
    else if(_t == 1 )
    {
        if(!checkRange(0,r))
        {
            throw range_error("setRow():r-column is out of range");
        }
        if(d.size() != _nRow)
        {
            throw range_error("setRow():d.size does not match with row");
        }
        else
        {
            for(size_t i = 0; i < _nRow; i++)
            {
                _data[i][r] = d[i];
            }
            return true;
        }
    }
    return false;
}

template<typename T>
bool Matrix<T>::setCol(size_t c, const std::vector<T>& d)
{
    // set the element of the c-column
    // remember check range first
    if(!checkRange(0,c)||d.size()!=_nRow){
        throw range_error("in setCol,out of range!");
    }
    for(size_t i=0;i<_nRow;i++){
        operator()(i,c)=d[i];
    }
    return true;
}

template<typename T>
Matrix<T>::~Matrix()
{
    if(0==_t)
    {
        // delete _data[i] first
        for( size_t i = 0; i < _nRow ;i++)
        {
            delete []  _data[i];
        }
    }
    else
    {
        // then delete _data
        for(size_t i =0;i<_nCol;i++)
        {
            delete [] _data[i];
        }
    }
    // note the value of _t
    delete  [] _data;
}

template<typename T>
Matrix<T> Matrix<T>::subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const
{
    // note the validity of value:rs, re, cs, ce
    // try to use "except" to broadcast the exception
    if(_t==0)
    {
        if(rs> re||rs<0||re>=_nRow||cs>ce||cs<0||ce>=_nCol)
        {
            throw std::range_error("in subMatrix,cs ||ce||rs||re out of range");
        }
        else
        {
            size_t r = re-rs+1;
            size_t c= ce-cs+1;
            Matrix<T> temp(r,c,0);
            for(size_t i=0;i<r;i++)
            {
                for(size_t j=0;j<c;j++)
                {
                    temp(i,j)=_data[rs+i][cs+j];
                }
            }
            return temp;
        }
    }
}

template<typename T>
Matrix<T>& Matrix<T>::transpose()
{
    // change _t
    _t = ! _t;
    // swap _nRow and _nCol
    size_t temp = _nRow;
    _nRow = _nCol;
    _nCol = temp;
    return *this;
}

template<typename T>
bool Matrix<T>::uset()
{
    return _t;
}
/*******************************************************************************
 -------------------------------------------------------------------------------
 ******************************************************************************/



/*******************************************************************************
 -------------------------------------------------------------------------------
 addiction
 ******************************************************************************/

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> mat)
{
    return addmatrix(*this, mat);  //addiction m1 + m2
}

template<typename T>
Matrix<T> Matrix<T>::addmatrix(Matrix<T>& m1, Matrix<T> m2)
{
    if(!(m1.getNRow() == m2.getNRow() && m1.getNCol() == m2.getNCol()))
    {
        throw range_error("in addmatrix,size of m1 is not equal to m2");
    }
    else
    {
        size_t r = m1.getNRow();
        size_t c = m1.getNCol();
        Matrix<T> m(r,c,0);
        for(size_t i=0; i < r; i++)
        {
            for(size_t j=0; j < c; j++)
            {
                m(i,j) = m1(i,j) + m2(i,j);//
            }
        }
        return m;
    }
}

template<typename T>
Matrix<T> Matrix<T>::operator+(T t)
{
    return addnum(*this, t);
}

template<typename T>
Matrix<T> Matrix<T>::addnum(Matrix<T>& m1, T t)
{
    size_t r = m1.getNRow();
    size_t c = m1.getNCol();
    Matrix<T> m(r,c,0);
    for(size_t i=0; i < r; i++)
    {
        for(size_t j=0; j < c; j++)
        {
            
            m(i,j) = m1(i,j) + t;//
        }
    }
    return m;
}

/*
 template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& mat)
{
    if(_nRow!=mat._nRow||_nCol!=mat._nCol)
    {
        throw std::range_error("in operator+,out of range");
    }
    size_t r = mat._nRow;
    size_t c = mat._nCol;
    Matrix<T> temp(r,c,0);
    for(size_t i=0;i<_nRow;i++)
    {
        for(size_t j =0;j<_nCol;j++)
        {
            temp(i,j) = _data[i][j] + mat._data[i][j];
        }
    }
    return  temp;
}


template<typename T>
Matrix<T> Matrix<T>::operator+(const T& t)
{
    size_t r = _nRow;
    size_t c = _nCol;
    Matrix<T> temp(r,c,0);
    for(size_t i=0;i<_nRow;i++)
    {
        for(size_t j =0;j<_nCol;j++)
        {
            temp(i,j) = _data[i][j] + t;
        }
    }
    return  temp;
}
*/

/*******************************************************************************
 -------------------------------------------------------------------------------
 substraction
 ******************************************************************************/

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> mat)
{
    return subtractmatrix(*this, mat);
}

template<typename T>
Matrix<T> Matrix<T>::subtractmatrix(Matrix<T>& m1, Matrix<T> m2)
{
    if(!(m1.getNRow() == m2.getNRow() && m1.getNCol() == m2.getNCol()))
    {
        throw range_error("in substractmatrix,size of m1 is not equal to m2");
    }
    else
    {
        size_t r = m1.getNRow();
        size_t c = m1.getNCol();
        Matrix<T> m(r,c,0);
        for(size_t i=0; i < r; i++)
        {
            for(size_t j=0; j < c; j++)
            {
                m(i,j) = m1(i,j) - m2(i,j);//
            }
        }
        return m;
    }
}

template<typename T>
Matrix<T> Matrix<T>::operator-(T t)
{
    return subtractnum(*this, t);
}

template<typename T>
Matrix<T> Matrix<T>::subtractnum(Matrix<T>& m1, T t)
{
    size_t r = m1.getNRow();
    size_t c = m1.getNCol();
    Matrix<T> m(r,c,0);
    for(size_t i=0; i < r; i++)
    {
        for(size_t j=0; j < c; j++)
        {
            
            m(i,j) = m1(i,j) - t;//
        }
    }
    return m;
}

/*******************************************************************************
 -------------------------------------------------------------------------------
 multiplication
 ******************************************************************************/

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> mat)
{
    return multimatrix(*this, mat);
}

template<typename T>
Matrix<T> Matrix<T>::multimatrix(Matrix<T>& m1, Matrix<T> m2)
{
    if(m1.getNCol() != m2.getNRow())
    {
        throw std::range_error("in operator*,can't do multiplication");
    }
    size_t r = m1.getNRow();
    size_t c = m2.getNCol();
    Matrix<T> temp(r,c,0);
    for(size_t i=0;i<r;i++)
    {
        for(size_t j =0;j<c;j++)
        {
            T sum = 0;
            for(size_t k = 0;k<m2.getNRow();k++)
            {
                sum  +=  m1(i,k) * m2(k,j);
            }
            temp(i,j) = sum;
        }
    }
    return  temp;
    
}

template<typename T>
Matrix<T> Matrix<T>::operator*(T t)
{
    return multinum(*this, t);
}

template<typename T>
Matrix<T> Matrix<T>::multinum(Matrix<T>& m1, T t)
{
    size_t r = m1.getNRow();
    size_t c = m1.getNCol();
    Matrix<T> m(r,c,0);
    for(size_t i=0; i < r; i++)
    {
        for(size_t j=0; j < c; j++)
        {
            
            m(i,j) = m1(i,j) * t;//
        }
    }
    return m;
}
/*
template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& mat1,const Matrix<T>& mat2)
{
    if(mat1._nCol!=mat2._nRow)
    {
        throw std::range_error("in operator*,can't do multiplication");
    }
    size_t r = mat1._nRow;
    size_t c = mat2._nCol;
    Matrix<T> temp(r,c,0);
    for(size_t i=0;i<mat1._nRow;i++)
    {
        for(size_t j =0;j<mat2._nCol;j++)
        {
            T sum = 0;
            for(size_t k = 0;k<mat2._nRow;k++)
            {
                sum  = sum + mat1._data[i][k] * mat2._data[k][j];
            }
            temp(i,j) = sum;
        }
    }
    return  temp;
}
*/
/*
template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& mat)
{
    if(_nCol!=mat._nRow)
    {
        throw std::range_error("in operator*,can't do multiplication");
    }
    size_t r = _nRow;
    size_t c = mat._nCol;
    Matrix<T> temp(r,c,0);
    for(size_t i=0;i<_nRow;i++)
    {
        for(size_t j =0;j<mat._nCol;j++)
        {
            T sum = 0;
            for(size_t k = 0;k<mat._nRow;k++)
            {
                sum  = sum + _data[i][k] * mat._data[k][j];
            }
            temp(i,j) = sum;
        }
    }
    return  temp;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const T& a)
{
    size_t r = _nRow;
    size_t c = _nCol;
    Matrix<T> temp(r,c,0);
    for(size_t i=0;i<_nRow;i++)
    {
        for(size_t j =0;j<_nCol;j++)
        {
            temp(i,j)  =  _data[i][j] * a;
        }
    }
    return  temp;
}
 */

/*******************************************************************************
 -------------------------------------------------------------------------------
 division
 ******************************************************************************/

template<typename T>
Matrix<T> Matrix<T>::operator/(const T& a)
{
    size_t r = _nRow;
    size_t c = _nCol;
    Matrix<T> temp(r,c,0);
    for(size_t i=0;i<_nRow;i++)
    {
        for(size_t j =0;j<_nCol;j++)
        {
            temp(i,j)  =  _data[i][j] / a;
        }
    }
    return  temp;
}
/*******************************************************************************
 operator<<
******************************************************************************/
template<typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& rhs)
{
    for(size_t i=0; i<rhs._nRow; ++i) {
        for(size_t j=0; j<rhs._nCol; ++j) {
            out << rhs(i,j) << "\t";
        }
        out << endl;
    }
    return out;
}


/*******************************************************************************
Image to Matrix       //Matrix to QImage
******************************************************************************/
template<typename T>
Matrix<int> Matrix<T>::fromQImage(const QImage& img ,char t)
{
    int width = img.width();
    int height = img.height();
    Matrix<int> mat(height,width,0);
    for(int i=0;i < height ; ++i)
    {
           for(int j=0;j < width ; ++j)
           {
               switch (t)
               {
               case 'r':
                   mat(i,j) = qRed(img.pixel(j,i));
                   break;
               case 'g':
                   mat(i,j) = qGreen(img.pixel(j,i));
                   break;
               case 'b':
                   mat(i,j) = qBlue(img.pixel(j,i));
                   break;
               case 'y':
                   mat(i,j) = qGray(img.pixel(j,i));
                   //mat(i,j) = qGray(img.pixel(i,j));
                   break;
               default:
                   break;
               }
           }
    }
     return mat;
}

template<typename T>
QImage Matrix<T>::toQImage(const Matrix<T>& mat)
{
    int width =mat.getNCol();
    int height =mat.getNRow();
    QImage img(width,height,QImage::Format_RGB32);
    double gray;
    for(int i=0;i<height;++i)
    {
         for(int j=0;j<width;++j)
         {
            std::complex<double> temp=mat(i,j);
            gray=sqrt(pow(temp.real(),2)+pow(temp.imag(),2));//取模
            img.setPixel(j,i,qRgb(gray,gray,gray));
          }
    }
    return img;
}

template<typename T>
QImage Matrix<T>::toQImage(const Matrix<T>& matr,const Matrix<T>& matg,const Matrix<T>& matb)
{
    cout<<"Matrix to QImage"<<endl;
    int width =matr.getNCol();
    int height =matr.getNRow();
    QImage img(width,height,QImage::Format_RGB32);
    for(int i=0;i<height;++i)
    {
         for(int j=0;j<width;++j)
         {
            std::complex<double> red = matr(i,j);
            std::complex<double> green = matg(i,j);
            std::complex<double> blue = matb(i,j);
            double r=sqrt(pow(red.real(),2)+pow(red.imag(),2));//取模
            double g=sqrt(pow(green.real(),2)+pow(green.imag(),2));//取模
            double b=sqrt(pow(blue.real(),2)+pow(blue.imag(),2));//取模
//            r=qLn(1+qAbs(r))/qLn(1+qAbs(10));
//            g=qLn(1+qAbs(g))/qLn(1+qAbs(10));
//            b=qLn(1+qAbs(b))/qLn(1+qAbs(10));
            img.setPixel(j,i,qRgb(r,g,b));
          }
    }
     return img;
}

/*******************************************************************************
fftShift
******************************************************************************/
//template<typename T>
//void Matrix<T>::rowCyclicShift(int r) {
//    _startR -= (r % _nRow) ;
//    if(_startR<0) _startR += _nRow;
//    cout<<"_startR = "<<_startR<<endl;
//    cout<< "row shift"<<endl;
//}

//template<typename T>
//void Matrix<T>::colCyclicShift(int c) {
//    _startC -= (c % _nCol);
//    if(_startC<0) _startC += _nCol;
//    cout<<"_startC = "<<_startC<<endl;
//    cout<< "col shift"<<endl;
//}

//template<typename T>
//Matrix<T> Matrix<T>::fftshift() {
//    rowCyclicShift(_nRow/2);
//    colCyclicShift(_nCol/2);
//    return *this;
//}

//***************点乘*******************
template<typename T>
Matrix<T> Matrix<T>::dotmul(Matrix<T>& mat1,Matrix<T>& mat2)
{

   Matrix<T> mat(mat1);
    if(!(mat1.getNRow()==mat2.getNRow()&&mat1.getNCol()==mat2.getNCol()))
   {
       throw std::range_error("in dotmultiply(),can't do dotmultiply");
   }
   else
   {
       for(size_t i=0;i<mat1._nRow;i++)
       {
           for(size_t j =0;j<mat1._nCol;j++)
           {
               mat(i,j) = mat1(i,j)*mat2(i,j);
           }
       }
   }
    return mat;
}

//***************卷积*******************
template<typename T>
Matrix<T> Matrix<T>::conv(Matrix<T>& m1,Matrix<T>& m2)//m1图像，m2滤波器
{

        cout<<"--------------------filter-----------"<<endl;
        cout<<m2<<endl;
        //----------------变换滤波器----------------
        Matrix<T> mfil1(m2.getNRow(), m2.getNCol(), 0);//第一次变换后的滤波器矩阵,滤波器一般为正方形
        Matrix<T> mfil2(m2.getNRow(), m2.getNCol(), 0);//第二次变换后的滤波器矩阵
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
        vector<T> filter;
        for(size_t k1=0;k1<mfil2.getNRow();k1++)
        {
            for(size_t k2=0;k2<mfil2.getNCol();k2++)
            {
                filter.push_back(mfil2(k1,k2));
            }
        }
        //-------------给原图的矩阵四周补零---------------
        int half = mfil2.getNRow() / 2;

        Matrix<T> mimg = zeroPadding_M(m1,mfil2.getNCol(),mfil2.getNRow());
//        Matrix<T> mimg = Duplicate_M(m1,mfil2.getNCol(),mfil2.getNRow());
        cout<<"-----------------zero image-----------"<<endl;
        cout<<mimg<<endl;
        //-------------卷积：即补零后的矩阵与滤波器的矩阵相乘相加---------------
        Matrix<T> resimg(m1.getNRow(),m1.getNCol(),0);//卷积后的结果矩阵
        int sum,seq;
        for(size_t i=half;i<=mimg.getNRow()-1-half;i++)
        {
            for(size_t j=half;j<=mimg.getNCol()-1-half;j++)
            {
                //(i,j)这个位置对应于滤波器的中心位置
                sum=0;
                seq=0;//滤波器vector里的元素的下标
                for(size_t m=i-half;m<=i+half;m++)//滤波器的所在的行的范围
                {
                    for(size_t n=j-half;n<=j+half;n++)//滤波器所在的列的范围
                    {
                        sum += mimg(m,n) * filter[seq];
                        seq++;
                    }
                }

                resimg(i-half,j-half) = sum;
            }
        }
        cout<<"卷积"<<endl;

        return resimg;
}
template<typename T>
Matrix<T> Matrix<T>::zeroPadding_M(Matrix<T>& matimg,int nCol,int nRow)
{
    Matrix<T> padded(matimg.getNRow()+nRow-1,matimg.getNCol()+nCol-1,0);

    int widthp = padded.getNCol();
    int heightp = padded.getNRow();    //扩展边界
    cout<<"补零后的行："<<heightp<<"补零后的列："<<widthp<<endl;
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

template<typename T>
Matrix<T> Matrix<T>::Duplicate_M(Matrix<T>& matimg,int nCol,int nRow)
{
    Matrix<T> padded(matimg.getNRow()+nRow-1,matimg.getNCol()+nCol-1,0);
    int width = padded.getNCol();
    int height = padded.getNRow();    //扩展边界
    cout<<"边界重复后的行："<<height<<"边界重复后的列："<<width<<endl;

    for (int i = nRow/2 ; i<height-nRow/2 ; i++)//1111原图像的区域
    {
        for (int j = nCol/2 ; j<width-nCol/2 ; j++)
        {
            padded(i,j) = matimg(i-nRow/2,j-nCol/2);
            cout<<"   (i,j):("<<i<<","<<j<<")"<<'\t'<<"padded:"<<padded(i,j);
        }
    }

    for (int i= nRow/2;i<height-nRow/2;i++)//222原图左边区域
    {
        for (int j = 0 ; j < nCol/2;j++)
        {
            padded(i,j) = matimg(i,nCol/2);
            cout<<"左边"<< "(i,j):("<<i<<","<<j<<")"<<'\t'<<"padded:"<<padded(i,j);
        }
    }

    for (int i=nRow/2;i<height-nRow/2;i++)//333原图右边区域
    {
        for (int j = width-nCol/2 ; j<width ; j++)
        {
            padded(i,j) = matimg(i,width-nCol/2 - 1);
            cout<<"   (i,j):("<<i<<","<<j<<")"<<'\t'<<"padded:"<<padded(i,j);
        }
    }

    for (int i = 0 ; i < nRow/2 ; i++)//444原图上边区域
    {
        for (int j = 0 ; j<width ; j++)
        {
            padded(i,j) = matimg(nRow/2,j);
            cout<<"   (i,j):("<<i<<","<<j<<")"<<'\t'<<"padded:"<<padded(i,j);
        }
    }

    for (int i = height - nRow/2 ; i < height ; i++)//555原图下边区域
    {
        for (int j = 0 ; j<width ; j++)
        {
            padded(i,j) = matimg(height- nRow/2 - 1,j);
            cout<<"   (i,j):("<<i<<","<<j<<")"<<'\t'<<"padded:"<<padded(i,j);
        }
    }
    for (int i = 0; i < height ; i++)//555原图下边区域
    {
        for (int j = 0 ; j<width ; j++)
        {
            cout<<"    padded: " <<endl;
            cout<<padded(i,j)<<"\t";
        }
    }
    cout<<endl;
    return padded;
    cout<<"Duplicate"<<endl;
}


/********************矩阵求逆************************************/
template<typename T>
Matrix<T> Matrix<T>::inverse(Matrix<T>& mat)
{
    double D = mat(0,0)*mat(1,1)*mat(2,2) + mat(1,0)*mat(2,1)*mat(0,2) + mat(0,1)*mat(1,2)*mat(2,0)
                - mat(0,2)*mat(1,1)*mat(2,0) - mat(0,1)*mat(1,0)*mat(2,2) - mat(0,0)*mat(1,2)*mat(2,1);

        if(D == 0)
        {
            cout<<"the input matrix can not be inverted"<<endl;
//            return img;
        }
        double A11 = mat(1,1)*mat(2,2) - mat(1,2)*mat(2,1);
           double A12 = -( mat(1,0)*mat(2,2) - mat(1,2)*mat(2,0) );
           double A13 = mat(1,0)*mat(2,1) - mat(1,1)*mat(2,0);
           double A21 = -( mat(0,1)*mat(2,2) - mat(0,2)*mat(2,1) );
           double A22 = mat(0,0)*mat(2,2) - mat(0,2)*mat(2,0);
           double A23 = -( mat(0,0)*mat(2,1) - mat(2,0)*mat(0,1) );
           double A31 = mat(0,1)*mat(1,2) - mat(1,1)*mat(0,2);
           double A32 = -( mat(0,0)*mat(1,2) - mat(1,0)*mat(0,2) );
           double A33 = mat(0,0)*mat(1,1) - mat(0,1)*mat(1,0);

           Matrix<double> AB(3,3,0);
           AB(0,0) = A11;
           AB(0,1) = A21;
           AB(0,2) = A31;
           AB(1,0) = A12;
           AB(1,1) = A22;
           AB(1,2) = A32;
           AB(2,0) = A13;
           AB(2,1) = A23;
           AB(2,2) = A33;

           Matrix<double> inv_mat(3,3,0.0);
           inv_mat = (1.0/D)*AB;
           return inv_mat;

}

/********************矩阵求逆************************************/

#endif /* matrix_h */

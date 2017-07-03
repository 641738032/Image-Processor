#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<mypixmapitem.h>
#include<QDir>
#include<imageprocess.h>
#include<cmath>
#include<QRegularExpression>
#include<QRegularExpressionMatchIterator>
#include<iostream>
using namespace std;
#include"matrix.h"
#include<QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fileDlg(new QFileDialog()),
    gpi(NULL),
    gs(NULL),
    ltDlg(new Dialog(this)),
    lrDlg(new LinearDialog(this)),
    gbDlg(new GaussBlurDialog(this)),
    bfDlg(new BilateralFilterDialog(this)),
    mfDlg(new MedianFilterDialog(this)),
    maxDlg(new MaximumFilterDialog(this)),
    miniDlg(new MinimumDialog(this)),
    transDlg(new TransformDialog(this)),//仿射变换
    lowDlg(new LowPassDialog(this)),
    highDlg(new HighPassDialog(this))
{
    ui->setupUi(this);
    connect(ltDlg,SIGNAL(confirmed(double)),this,SLOT(on_ltDlg_confirmed(double)));
    connect(lrDlg,SIGNAL(confirmed(QString,bool)),this,SLOT(on_lrDlg_confirmed(QString,bool)));
     connect(gbDlg,SIGNAL(confirmed(int ,double,bool)),this,
             SLOT(on_gbDlg_confirmed(int,double,bool)));

     connect(bfDlg,SIGNAL(confirmed(int ,double,double)),this,
             SLOT(on_bfDlg_confirmed(int,double,double)));

     connect(mfDlg,SIGNAL(confirmed(int,bool)),this,
             SLOT(on_mfDlg_confirmed(int,bool)));

     connect(maxDlg,SIGNAL(confirmed(int,bool)),this,
             SLOT(on_maxDlg_confirmed(int,bool)));
     connect(miniDlg,SIGNAL(confirmed(int,bool)),this,
             SLOT(on_miniDlg_confirmed(int,bool)));

     connect(transDlg,SIGNAL(confirmed(const QMatrix& )),this,
             SLOT(on_transDlg_confirmed(const QMatrix&)));

     connect(lowDlg,SIGNAL(confirmed(Matrix<double>&,bool)),this,
             SLOT(on_lowDlg_confirmed(Matrix<double>&,bool)));
     connect(highDlg,SIGNAL(confirmed(Matrix<double>&,bool)),this,
             SLOT(on_highDlg_confirmed(Matrix<double>&,bool)));
}

MainWindow::~MainWindow()
{
    delete gpi;
    delete gs;
    delete fileDlg;
    delete ltDlg;
    delete lrDlg;
    delete gbDlg;
    delete bfDlg;
    delete mfDlg;
    delete maxDlg;
    delete miniDlg;
    delete transDlg;
    delete lowDlg;
    delete highDlg;
    delete ui;

}

void MainWindow::on_btnOpen_clicked()//button OPEN的槽函数
{
    if (fileDlg->exec())
    {
        QStringList file = fileDlg->selectedFiles();//使用QFileDialog来实现打开一个对话框，供用户选择
        QImage img = QImage(file[0]);//将选择的文件存储在img

        QDir dir = fileDlg->directory();//记录当前已经打开图片所在的位置
        QStringList filters;//设置一个filters来控制可以读取的图片类型
        filters << "*.jpg" <<"*.bmp"<<"*.png"<<"*.tif";//读取jpg，bmp，png，tif等格式图片
        images = dir.entryInfoList(filters); //找到符合条件的文件

        for(int i=0;i<images.length();i++)
            if(images[i].absoluteFilePath() == file[0])
                curIdx = i;

        showImg(img);//使用showImg来显示选择的img文件
        //Last_img=QImage(files[0]);
        //cout << "open"<<endl;

//        int h = img.height();
//        int w = img.width();
//        Matrix<int> mat (h,w,0);
//        Matrix<int> matr (h,w,0);
//        Matrix<int> matg (h,w,0);
//        Matrix<int> matb (h,w,0);
//        mat = mat.fromQImage(img,'y');
//        matr = matr.fromQImage(img,'r');
//        matg = matg.fromQImage(img,'g');
//        matb = matb.fromQImage(img,'b');

//        cout<<"Image to Matrix \t"<< mat <<endl;
//        cout<<"after fft2"<<fft2d(mat,h,w)<<endl;
//        //showImg(mat.toQImage(fft2d(matr,h,w),fft2d(matg,h,w),fft2d(matb,h,w)));
//        //showImg(Matrix<T>::toQImage(fft2d(mat,h,w)));
//        showImg(mat.toQImage(fft2d(mat,h,w)));


//        int h = img.height();
//        int w = img.width();
//        Matrix<int> mat (h,w,0);
//         mat = mat.fromQImage(img,'y');
//         cout<<"Image to Matrix \t"<< mat <<endl;
//         cout<<mat.transpose()<<endl;

//        Matrix<int> mat2(3,3,1);
//        Matrix<int> mat1(5,5,1);
//        Matrix<int> mat(5,5,0);
//        mat = mat.conv(mat1,mat2);
//        cout<<"*****conv******mat********"<<endl;
//        cout<<mat<<endl;

//        Matrix<int> mat4(5,5,2);
//        Matrix<int> mat3(5,5,4);
//        Matrix<int> mat5(5,5,0);
//        mat5 = mat5.dotmul(mat3,mat4);
//        cout<<"*****dot******mat5********"<<endl;
//        cout<<mat5<<endl;
    }
}
 void MainWindow::showImg(QImage img)//
 {
     if(NULL != gpi)////
     {
         delete gpi;
         gpi = NULL;
     }
     if(NULL != gs)//
     {
         delete gs;
         gs = NULL;
     }

    //gpi = new QGraphicsPixmapItem();
     gpi = new MyPixmapItem();
     gpi->setPixmap(QPixmap ::fromImage(img));


     gs = new QGraphicsScene();
     gs ->addItem(gpi);

     ui->graphicsView->setScene(gs);

     reset();
 }

void MainWindow::on_btnReset_clicked()
{
    reset();

}
 void MainWindow::reset()
 {
     if(NULL==gpi)
         return;

     gpi->reset();   //图片复原

     gs->setSceneRect(gpi->boundingRect());//将场景复原//

     ui->graphicsView->fitInView(gpi->boundingRect(),
                                 Qt::KeepAspectRatio);//控件复原，让graphicsView根据窗口大小自动缩放

 }

 void MainWindow::on_btnPre_clicked()
 {
    if (0 == images.length())
        return;

     if(curIdx<=0) //当前图片为第一张
        curIdx = images.length();//
    showImg(QImage(images[--curIdx].absoluteFilePath()));


 }

 void MainWindow::on_btnNext_clicked()
 {
     if (0==images.length())
         return;

     if(curIdx>=images.length()-1) //当前图片为最后一张
         curIdx = -1;//
     showImg(QImage(images[++curIdx].absoluteFilePath()));//向后切换图片时，从列表中获取下一张图片并显示

 }
 void MainWindow::on_actionRgb2gray_triggered(){
       QImage img;
       bool ok;
       ok=getDisplayImage(img);
       if(ok)
           showImg(imageprocess::rgb2gray(img));
   }

 void MainWindow::on_actionInverse_triggered(){
       QImage img;
       bool ok;
       ok=getDisplayImage(img);
       if(ok)
           showImg(imageprocess::inverse(img));
   }

   bool MainWindow::getDisplayImage(QImage&img)  const{
      // cout<<"getDisplayImg"<<endl;
       bool ok;
       if(gpi)
       {
           QList<QGraphicsItem*> items=
                   ui->graphicsView->scene()->items();
           QGraphicsPixmapItem* item=
                   (QGraphicsPixmapItem*)items.at(0);
           img=item->pixmap().toImage();
           //cout<<"getDisplayImg-true"<<endl;
           return true;
       }
          // cout<<"getDisplayImg-false"<<endl;
           return false;
   }

   void MainWindow::on_actionLogTran_triggered(){
       ltDlg->exec();
   }


   void MainWindow::on_ltDlg_confirmed(double c){

       cout<<"get c:"<<c<<endl;
       QImage img;
       bool ok;
       ok = getDisplayImage(img);
       if(ok){
           showImg(imageprocess::logTransformation(img,c));
       }
   }

   void MainWindow::on_actionHistEq_triggered()
   {
       cout<<"histeq"<<endl;
       bool ok;
       QImage img;
       ok = getDisplayImage(img);
       if(ok){
           showImg(imageprocess::histEq(img));
       }
   }

   void MainWindow::on_actionColorHist_triggered()
   {
       cout<<"colorhisteq"<<endl;
       bool ok;
       QImage img;
       ok = getDisplayImage(img);
       if(ok){
           showImg(imageprocess::colorHistEq(img));
       }
   }

   void MainWindow::on_actionLinear_triggered()//打开对话框，点OK以后回传参数
   {
       cout<<"Linear"<<endl;
       lrDlg->exec();
   }

 void MainWindow::on_lrDlg_confirmed(QString txt,bool gray)
 {
     cout<<txt.toStdString()<<endl;//按照输入的格式输出矩阵

     int nCol;
     cout<<"Linear Filter"<<endl;
     vector<double> kernel = parseMatrix1(txt,nCol);//使用简单法解析矩阵

    for(size_t k = 0;k<kernel.size();++k )//将输入的矩阵分开 ，输出
    {
        cout<< kernel[k]<<"\t";
        cout<<"nCol:"<<nCol<<endl;
    }

    imageprocess::kernelNormalization(kernel);//矩阵的归一化
    if (-1==nCol) return;

    QImage img;
    bool ok = getDisplayImage(img);
    if(ok)
    {
//         cout<<ok <<endl;
    if(gray)
         showImg(imageprocess::linearFilterGray(img,kernel,nCol));
    //当从LinearDialog回传的参数gray 为true时，执行linearFilterGray
    else
        showImg(imageprocess::linearFilter(img,kernel,nCol));
    //当从LinearDialog回传的参数gray 为false时，执行linearFilter
         cout<<"zhixing"<<endl;      //测试
    }
 }


vector<double> MainWindow::parseMatrix1(QString txt, int & nCol)//解析矩阵
{
    //cout<<"parseMatrix111"<<endl;

    //spilt into row by;
    QStringList rows =  txt.replace("\n","").
            trimmed().split(QRegularExpression("\\s*;"));//  用 ";"换行，分割字符串，得到每一行
    nCol = -1;
    vector<double> ret;//保存数据

    //process each row  //针对每一行
    for (int i=0;i<rows.size();i++)
    {
        bool ok;
        QString row =  rows[i];
        if (row.trimmed().isEmpty()) continue;
        //first row
        if (nCol == -1)
        {
            int temp;
            ok = readOneRow(row, ret, temp);
            if (ok)
                nCol =  temp;
            else
                return vector<double>();
        }
        //other row
        else
        {
            int temp;
            ok = readOneRow(row, ret, temp);
            if(ok && temp==nCol)
                continue;
            else
            {
                nCol = -1;
                return vector<double>();
            }
        }
    }
    return ret;
}

vector<double> MainWindow::parseMatrix2(QString txt, int & nCol){
    vector<double> ret;
    QRegularExpression reRow("[-\\d\\.\\s,]+;?");
    QRegularExpression reCol("-?\\d*\\.?\\d+[,|\\s]?");
    QRegularExpressionMatchIterator remiRow = reRow.globalMatch(txt);
    while(remiRow.hasNext()) {
    QString row = remiRow.next().captured().replace(";","").trimmed();
    if (row.isEmpty()) continue;
    QRegularExpressionMatchIterator remiCol = reCol.globalMatch(row);
    nCol = -1;
    int tmp = 0;
    while(remiCol.hasNext()) {
    QString col = remiCol.next().captured().replace(",","").trimmed();
    if (col.isEmpty()) continue;
    bool ok;
    double d = col.toDouble(&ok);
    if(ok) {
    ret.push_back(d);
    tmp++;
    }
    else
    return vector<double>();
    }
    if(-1==nCol) nCol = tmp;
    else if (nCol != tmp) return vector<double>();
    }
    return ret;
}

bool MainWindow::readOneRow(QString row , std::vector<double>&kernel,int& nCol)//针对每一行
{
    cout<<"readOneRow"<<endl;
    nCol = 0;
    //spilt into number by , and \s
    QStringList cols = row.trimmed().split(QRegularExpression("\\s*,|\\s+"));//用“，”和空格分割字符串，得到每一列
    for (int j=0;j<cols.size();j++)
    {
        //parse each col
        bool ok;
        QString col =cols[j];
        if (col.trimmed().isEmpty()) continue;
        double d = col.trimmed().toDouble(&ok);
        if (ok)
        {
            nCol++;
            kernel.push_back(d);
        }
        else
        {
            return false;
        }
    }
    return true;
}

//--------------------->
void MainWindow::on_actionGaussBlur_triggered()
{
    gbDlg->exec();
}

void MainWindow::on_gbDlg_confirmed(int size, double sigma,bool gray)
{
   cout<<"get size="<<size<<"   get sigma="<<sigma<<"       get radiobtn_gray"<<gray<<endl;
    QImage img;
    bool ok = getDisplayImage(img);
    if (ok)
    {
        if(gray)
        showImg(imageprocess::gaussBlurGray(img,size,sigma));
        else
         showImg(imageprocess::gaussBlur(img,size,sigma));
    }
}

//-------------->
void MainWindow::on_actionBilateralFilter_triggered()
{
    bfDlg->exec();
}

void MainWindow::on_bfDlg_confirmed(int size,double sigmaSpace,double sigmaColor)
{
   cout<<"get size="<<size<<"get sigmaSpace:"<<sigmaSpace<<"   get sigmaColor="<<sigmaColor<<endl;
    QImage img;
    bool ok = getDisplayImage(img);
    if (ok)
        showImg(imageprocess::bilateralFilter(img,size,sigmaSpace,sigmaColor));
}

//--------------------->
void MainWindow::on_actionMedianFilter_triggered()
{
    cout<<"median filter"<<endl;
    mfDlg->exec();
}

void MainWindow::on_mfDlg_confirmed(int size,bool gray)
{
    cout<<"get size="<<size<<"  gray?="<<gray<<endl;
     QImage img;
     bool ok = getDisplayImage(img);
     //3 for morph ,7 for median
     if (ok)
     {
         if(gray)
             showImg(imageprocess::MedianFilter(img,size));
         else
             showImg(imageprocess::MedianFilterRGB(img,size));
     }

}

void MainWindow::on_actionMaximumFilter_triggered()
{
    cout<<"maximum filter"<<endl;
    maxDlg->exec();
}

void MainWindow::on_maxDlg_confirmed(int size,bool gray)
{
    cout<<"get size="<<size<<endl;
     QImage img;
     bool ok = getDisplayImage(img);
     //3 for morph ,7 for median
     if (ok)
     {
         if(gray)
         showImg(imageprocess::MaximumFilter(img,size));
         else
             showImg(imageprocess::MaximumFilterRGB(img,size));
     }
}

void MainWindow::on_actionMinimumFilter_triggered()
{
    cout<<"minimum filter"<<endl;
    miniDlg->exec();
}

void MainWindow::on_miniDlg_confirmed(int size,bool gray)
{
     cout<<"get size="<<size<<endl;
     QImage img;
     bool ok = getDisplayImage(img);
     //3 for morph ,7 for median
     if (ok)
     {
         if(gray)
             showImg(imageprocess::MinimumFilter(img,size));
         else
             showImg(imageprocess::MinimumFilterRGB(img,size));
     }
}

void MainWindow::on_actionFft_triggered()
{
    cout<<"FFT"<<endl;
    bool ok;
    QImage img;
    ok = getDisplayImage(img);
    if(ok){
        showImg(imageprocess::imgfft2d(img));
    }
}
void MainWindow::on_actionIfft_triggered()
{

    cout<<"IFFT"<<endl;
    bool ok;
    QImage img;
    ok = getDisplayImage(img);
    if(ok){
        showImg(imageprocess::imgIfft2d(img));
    }

}


/*******************仿射变换**************************/
void MainWindow::on_actionTransform_triggered()
{
    cout<<"******Transform****"<<endl;
    transDlg->exec();
}

void MainWindow::on_transDlg_confirmed(const QMatrix& mat)
{
    cout<<"*****Affine Transform******"<<endl;
    bool ok;
    QImage img;
    ok = getDisplayImage(img);
    cout<<"**getDisplayImage*****"<<endl;
    if(ok)
    {
        QPointF pos=gpi->pos();
        cout<<"**pos*****"<<endl;
        double scale=gpi->scale();
        cout<<"**scale*****"<<endl;
        //QImage img = imageprocess::Transform(mat);
        showImg(imageprocess::Transform(img,mat));

        gpi->setPos(pos);
        cout<<"**setPos*****"<<endl;
        gpi->setTransformOriginPoint(
                      gpi->boundingRect().width()/2,
                      gpi->boundingRect().height()/2);
        cout<<"**setTransformOriginPoint*****"<<endl;
        gpi->setScale(scale);
        cout<<"**setScale*****"<<endl;
    }
}

//button SAVE
void MainWindow::on_btnSave_clicked()//button OPEN的槽函数
{
    QImage img;
    bool ok;
    ok=getDisplayImage(img);
    if(ok)
    {
        QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Image"),
        "",
        tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        if(! ( img.save(filename) ) ) //保存图像
        {
            QMessageBox::information(this,
                tr("Failed to save the image"),
                tr("Failed to save the image!"));
            return;
        }
    }
    }
}


/*******************低通滤波**************************/
void MainWindow::on_actionLowpassFilter_triggered()
{
    cout<<"******Lowpass****"<<endl;
    lowDlg->exec();
}

void MainWindow::on_lowDlg_confirmed(Matrix<double>& mat,bool spatial)
{
    cout<<"*********mat*********"<<'\t'<<endl;
    cout<<mat<<endl;
    QImage img;
    bool ok = getDisplayImage(img);

    if (ok)
    {
        if(spatial)
            showImg(imageprocess::LowPassFilterS(img,mat));
        else
            showImg(imageprocess::LowpassFilterF(img,mat));
    }

}
/*******************高通滤波**************************/
void MainWindow::on_actionHighpassFilter_triggered()
{
    cout<<"******Lowpass****"<<endl;
    highDlg->exec();
}

void MainWindow::on_highDlg_confirmed(Matrix<double>& mat,bool spatial)
{
    cout<<"*********mat*********"<<'\t'<<endl;
    cout<<mat<<endl;
    QImage img;
    bool ok = getDisplayImage(img);

    if (ok)
    {
        if(spatial)
            showImg(imageprocess::HighPassFilterS(img,mat));
        else
            showImg(imageprocess::HighpassFilterF(img,mat));
    }

}

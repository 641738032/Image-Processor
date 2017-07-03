#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include<QImage>
#include<mypixmapitem.h>
#include<iostream>
using namespace std;
#include<dialog.h>

#include<vector>

#include<QFileInfoList>

#include"lineardialog.h"
#include"gaussblurdialog.h"
#include"bilateralfilterdialog.h"
#include"medianfilterdialog.h"
#include"maximumfilterdialog.h"
#include"minimumdialog.h"
#include"imageprocess.h"

#include"transformdialog.h"


#include"matrix.h"
#include"fft.h"

#include"lowpassdialog.h"
#include"highpassdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QFileDialog * fileDlg;
    //QGraphicsPixmapItem *gpi;
    MyPixmapItem  *gpi;
    QGraphicsScene  *gs;

    Dialog *ltDlg;
    LinearDialog *lrDlg; //打开对话框
    GaussBlurDialog *gbDlg;
    BilateralFilterDialog *bfDlg;
    MedianFilterDialog *mfDlg;
    MaximumFilterDialog *maxDlg;
    MinimumDialog *miniDlg;

    TransformDialog * transDlg;

    LowPassDialog* lowDlg;
    HighPassDialog* highDlg;

    void reset();//

    bool getDisplayImage(QImage&) const;

    QFileInfoList images; //
    int curIdx;

    void showImg(QImage img);

    static vector<double>parseMatrix1(QString txt, int &nCol);
    static vector<double>parseMatrix2(QString txt, int &nCol);
    static bool readOneRow(QString row ,std::vector<double>&kernel,int &nCol);

public slots:
   void on_btnOpen_clicked();
   void on_btnReset_clicked();
   void on_btnPre_clicked();
   void on_btnNext_clicked();
   void on_actionRgb2gray_triggered();
   void on_actionInverse_triggered();

   void on_actionLogTran_triggered();

    void on_ltDlg_confirmed(double);//

   void on_actionHistEq_triggered();
   void on_actionColorHist_triggered();

   void on_actionLinear_triggered();
   void on_lrDlg_confirmed(QString,bool);//lineFilter 输入对话框

   void on_actionGaussBlur_triggered();
   void on_gbDlg_confirmed(int size,double sigma,bool gray);//lineFilter 输入对话框

   void on_actionBilateralFilter_triggered();
   void on_bfDlg_confirmed(int,double,double);//

   void on_actionMedianFilter_triggered();
   void on_mfDlg_confirmed(int,bool);//

   void on_actionMaximumFilter_triggered();
   void on_maxDlg_confirmed(int,bool);//

   void on_actionMinimumFilter_triggered();
   void on_miniDlg_confirmed(int,bool);//

   void on_actionFft_triggered();
   void on_actionIfft_triggered();

   void on_actionTransform_triggered();
   void on_transDlg_confirmed(const QMatrix& mat);

   //button SAVE
   void on_btnSave_clicked();

   void on_actionLowpassFilter_triggered();
   void on_lowDlg_confirmed(Matrix<double>& mat,bool spatial);

   void on_actionHighpassFilter_triggered();
   void on_highDlg_confirmed(Matrix<double>& mat,bool spatial);
};

#endif // MAINWINDOW_H


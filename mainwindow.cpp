#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.h>
//#include <opencv/highgui.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>



using namespace cv;
using namespace std ;
Point x1;
Point x2;
int seuil=0;
int thikness = 0;
int mode_line = 0;
int draw_mode=0;
// global variabls
VideoCapture capture;
Mat frame_design =Mat(480,640,CV_8UC3,Scalar(255,255,255));
Mat frame, frame_hsv,frame_gauss;
Scalar color_paint = Scalar(0,0,0);
int hold=0;
int test_detection = 0;

int upperColor1[3]={0,0,0};
int lowerColor1[3]={255,255,255};

int upperColor2[3]={0,0,0};
int lowerColor2[3]={255,255,255};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void My_inRange(Mat frame_hsv,Mat& res_frame1, Mat& res_frame2) {
    res_frame1 = Mat::zeros(frame_hsv.size(), 0);
    res_frame2 = Mat::zeros(frame_hsv.size(), 0);

    for (int i = 0; i < frame_hsv.rows; i++) {
        for (int j = 0; j < frame_hsv.cols; j++) {
            if ((int)frame_hsv.at<Vec3b>(i, j)[0] >= lowerColor1[0] && (int)frame_hsv.at<Vec3b>(i, j)[0] <= upperColor1[0]
                && (int)frame_hsv.at<Vec3b>(i, j)[1] >= lowerColor1[1] && (int)frame_hsv.at<Vec3b>(i, j)[1] <= upperColor1[1]
                && (int)frame_hsv.at<Vec3b>(i, j)[2] >= lowerColor1[2] && (int)frame_hsv.at<Vec3b>(i, j)[2] <= upperColor1[2]) {

                res_frame1.at<uchar>(i, j) = 255;
            }
            else {
                res_frame1.at<uchar>(i, j) = 0;
            }
            if ((int)frame_hsv.at<Vec3b>(i, j)[0] >= lowerColor2[0] && (int)frame_hsv.at<Vec3b>(i, j)[0] <= upperColor2[0]
                && (int)frame_hsv.at<Vec3b>(i, j)[1] >= lowerColor2[1] && (int)frame_hsv.at<Vec3b>(i, j)[1] <= upperColor2[1]
                && (int)frame_hsv.at<Vec3b>(i, j)[2] >= lowerColor2[2] && (int)frame_hsv.at<Vec3b>(i, j)[2] <= upperColor2[2]) {
                res_frame2.at<uchar>(i, j) = 255;
            }
            else {
                res_frame2.at<uchar>(i, j) = 0;
            }

        }
    }
}








void  Invers_frame(Mat img){
    Mat temps=img.clone();
    for (int i = 0; i <img.rows ; ++i)
        for (int j = 0; j <img.cols ; ++j)
            img.at<Vec3b>(i,j)=temps.at<Vec3b>(i,img.cols-j-1);
}


int __distance(Point a, Point b) {
    return int(sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2)));
}

Point contours_detection(Mat& frame,Mat res_frame) {
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(5, 5));
    erode(res_frame, res_frame, erodeElement);
    erode(res_frame, res_frame, erodeElement);
    erode(res_frame, res_frame, erodeElement);
    erode(res_frame, res_frame, erodeElement);
    dilate(res_frame, res_frame, dilateElement);
    dilate(res_frame, res_frame, dilateElement);
    dilate(res_frame, res_frame, dilateElement);
    dilate(res_frame, res_frame, dilateElement);
    vector<Point> point_white;
    int x = 0, y = 0;
    for (int i = 0; i < res_frame.rows; i++) {
        for (int j = 0; j < res_frame.cols; j++) {
            if (res_frame.at<uchar>(i, j) == 255) {
                point_white.push_back(Point(j, i));
                x += j;
                y += i;
            }
        }
    }
    if (point_white.size() > 0) {
        x = x / point_white.size(); y = y / point_white.size();
        int d_t,d = 0;
        for (int k = 0; k < point_white.size(); k++) {
            circle(frame, Point(point_white.at(k).x, point_white.at(k).y), 1, Scalar(0, 0, 255));
            d_t = __distance(Point(x,y),Point(point_white.at(k).x, point_white.at(k).y));
            if (d_t > d)
                d = d_t;
        }
        circle(frame, Point(int(x), int(y)), d, Scalar(255, 255, 255));
        circle(frame, Point(int(x), int(y)), 5, Scalar(255, 255, 255));
        return Point(x, y);
    }
    return Point(-1, -1);
}

void MainWindow::on_Start_clicked()
{
    capture.open(0);
    Mat gauss = Mat::ones(3, 3, CV_32FC1) / (float)16;
   gauss.at<float>(0, 1) = (float)2.0 / 16;
   gauss.at<float>(1, 0) = (float)2.0 / 16;
   gauss.at<float>(1, 1) = (float)4.0 / 16;
   gauss.at<float>(1, 2) = (float)2.0 / 16;
   gauss.at<float>(2, 1) = (float)2.0 / 16;
 Invers_frame(frame_design);
    for (;;) {
        capture >> frame;
        if (frame.empty())
            break;
        filter2D(frame, frame_gauss, -1, gauss, Point(-1, -1), 0, BORDER_DEFAULT);
        cvtColor(frame_gauss, frame_hsv, COLOR_BGR2HSV);

if(hold==1)
    {
     rectangle(frame,Rect(100,200,17,17),Scalar(0),2,8,0);
     rectangle(frame,Rect(200,200,17,17),Scalar(0),2,8,0);
    }
if(test_detection == 1){
    Mat res_inrange1;
    Mat res_inrange2;
    Mat res_inrange3;
    My_inRange(frame_hsv, res_inrange1, res_inrange2);
    Point center1 = contours_detection(frame, res_inrange1);
    Point center2 = contours_detection(frame, res_inrange2);
    if (center1.x != -1 && center1.x != -1) {

        if(__distance(center1,center2)<seuil)
        {
            Invers_frame(frame_design);
            if(draw_mode == 0){
                if(mode_line==0){
                    x1.x = int((center1.x + center2.x) / 2);
                    x1.y = int((center1.y + center2.y) / 2);
                    circle(frame_design, x1, 3, color_paint, thikness, 8, 0);
                    mode_line=1;
                      }else
                {
                    x2.x = int((center1.x + center2.x) / 2);
                    x2.y = int((center1.y + center2.y) / 2);
                    line(frame_design,x1,x2,color_paint,thikness);
                    x1=x2;
                }
            }else{
                x1.x = int((center1.x + center2.x) / 2);
                x1.y = int((center1.y + center2.y) / 2);
                circle(frame_design, x1, 3, color_paint, thikness, 8, 0);
            }
            Invers_frame(frame_design);
        }
    }
}
        cv::resize(frame,frame,Size(640,480));
        cvtColor(frame, frame,CV_BGR2RGB);
        Invers_frame(frame);
ui->video->setPixmap(QPixmap::fromImage(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888)));
cvtColor(frame_design, frame_design,CV_BGR2RGB);

ui->paint->setPixmap(QPixmap::fromImage(QImage(frame_design.data, frame_design.cols, frame_design.rows, frame_design.step, QImage::Format_RGB888)));


        //imshow("camera", frame);


        if (waitKey(60) == 27)
            break;

}
    capture.release();
    printf("ok");



}

void MainWindow::on_Stop_clicked()
{


}

void MainWindow::on_Stopplay_clicked()
{

        capture.release();

}

void MainWindow::on_Change_clicked()
{
   hold=1;
   mode_line=0;
   test_detection = 0;

}

void MainWindow::on_save_clicked()
{
    hold=0;
    upperColor1[0] = 0;upperColor1[1] = 0;upperColor1[2] = 0;lowerColor1[0] = 255; lowerColor1[0] = 255;lowerColor1[0] = 255;
    upperColor2[0] = 0;upperColor2[1] = 0; upperColor2[2] = 0;lowerColor2[0] = 255; lowerColor2[0] = 255;lowerColor2[0] = 255;
    Mat color11 =frame_hsv(Rect(104,202,13,13));
    Mat color22=frame_hsv(Rect(204,202,13,13));
    Mat color1=frame(Rect(640-104-11,202,13,13));
    Mat color2=frame(Rect(640-204-11,202,13,13));
   ui->Color11->setPixmap(QPixmap::fromImage(QImage(color1.data, color1.cols, color1.rows, color1.step, QImage::Format_RGB888)));
   ui->Color22->setPixmap(QPixmap::fromImage(QImage(color2.data, color2.cols, color2.rows, color2.step, QImage::Format_RGB888)));
   int i, j;
    for( i=0 ; i<color11.rows;i++)
    {
        for( j=0;j<color11.cols; j++)
        {
            if(upperColor1[0]<color11.at<Vec3b>(i,j)[0]){upperColor1[0]= color11.at<Vec3b>(i,j)[0];}
            if(upperColor1[1]<color11.at<Vec3b>(i,j)[1]){upperColor1[1]= color11.at<Vec3b>(i,j)[1];}
            if(upperColor1[2]<color11.at<Vec3b>(i,j)[2]){upperColor1[2]= color11.at<Vec3b>(i,j)[2];}
            if(lowerColor1[0]>color11.at<Vec3b>(i,j)[0]){lowerColor1[0]= color11.at<Vec3b>(i,j)[0];}
            if(lowerColor1[1]>color11.at<Vec3b>(i,j)[1]){lowerColor1[1]= color11.at<Vec3b>(i,j)[1];}
            if(lowerColor1[2]>color11.at<Vec3b>(i,j)[2]){lowerColor1[2]= color11.at<Vec3b>(i,j)[2];}
       }
    }
    for( i=0 ; i<color22.rows;i++)
    {
        for( j=0;j<color22.cols; j++)
        {
            if(upperColor2[0]<color22.at<Vec3b>(i,j)[0]){upperColor2[0]= color22.at<Vec3b>(i,j)[0];}
            if(upperColor2[1]<color22.at<Vec3b>(i,j)[1]){upperColor2[1]= color22.at<Vec3b>(i,j)[1];}
            if(upperColor2[2]<color22.at<Vec3b>(i,j)[2]){upperColor2[2]= color22.at<Vec3b>(i,j)[2];}
            if(lowerColor2[0]>color22.at<Vec3b>(i,j)[0]){lowerColor2[0]= color22.at<Vec3b>(i,j)[0];}
            if(lowerColor2[1]>color22.at<Vec3b>(i,j)[1]) {lowerColor2[1]= color22.at<Vec3b>(i,j)[1];}
            if(lowerColor2[2]>color22.at<Vec3b>(i,j)[2]){lowerColor2[2]= color22.at<Vec3b>(i,j)[2];}
        }
       }
test_detection = 1;

}

void MainWindow::on_New_clicked()
{
    frame_design =Mat(480,640,CV_8UC3,Scalar(255,255,255));
}

void MainWindow::on_Threshold_actionTriggered(int action)
{
    seuil=ui->Threshold->value()+1;
}


void MainWindow::on_Eraser_clicked()
{
    ui->Paint_b->setEnabled(true);
    ui->Eraser->setEnabled(false);
    color_paint = Scalar(255,255,255);
}

void MainWindow::on_Paint_b_clicked()
{
    ui->Eraser->setEnabled(true);
    ui->Paint_b->setEnabled(false);
    color_paint = Scalar(0,0,0);
}

void MainWindow::on_mode_point_clicked()
{
    ui->mode_line->setEnabled(true);
    ui->mode_point->setEnabled(false);
    draw_mode = 1;
}

void MainWindow::on_mode_line_clicked()
{
    ui->mode_point->setEnabled(true);
    ui->mode_line->setEnabled(false);
    mode_line = 0;
    draw_mode = 0;
}

void MainWindow::on_Thikness_actionTriggered(int action)
{
    thikness = ui->Thikness->value()/10;
// hena probleme m3a seil
}

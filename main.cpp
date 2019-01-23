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


int main(int argc, char *argv[])
{
    printf("welcom");

    QApplication a(argc, argv);
    MainWindow w ;
    w.show();


    return a.exec();
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <QMessageBox>
#include <QTime>
#include <QtCore/qmath.h>
#include "showpoint.h"

#include "ElSe.h"
#include "PuRe.h"
#include "utils.h"
#include "eyeimageprocessor.h"

using namespace tbb;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    num = 0;

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(clickCalibButton()));
    connect(ui->ReloadButton, SIGNAL(clicked()), this, SLOT(clickReloadButton()));
    connect(&theTimer, &QTimer::timeout, this, &MainWindow::updateImage);
    if(sceneCap.open(0) && eyeCap.open(2))
    {
        sceneImage= Mat::zeros(sceneCap.get(CV_CAP_PROP_FRAME_HEIGHT), sceneCap.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
        eyeImage = Mat::zeros(eyeCap.get(CV_CAP_PROP_FRAME_HEIGHT), eyeCap.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
        theTimer.start(20);
    }
    else{
        //exit(0);
    }
    gaze=Point2f(320.0,240.0);
    z_length=1000;//z轴距离mm
    l_perPix=1.0;//每像素实际距离mm
    sceneLabel = new QLabel(this);
    eyeLabel = new QLabel(this);
    ui->horizontalLayout->addWidget(sceneLabel);
    ui->horizontalLayout->addWidget(eyeLabel);
}

MainWindow::~MainWindow()
{
    delete sceneLabel;
    delete eyeLabel;
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *e)
{
    QImage image = QImage((uchar*)(sceneImage.data),sceneImage.cols,sceneImage.rows,QImage::Format_RGB888);
    QImage image1 = QImage((uchar*)(eyeImage.data),eyeImage.cols,eyeImage.rows,QImage::Format_RGB888);
    sceneLabel->setPixmap(QPixmap::fromImage(image));
    sceneLabel->resize(image.size());
    sceneLabel->show();
    eyeLabel->setPixmap(QPixmap::fromImage(image1));
    eyeLabel->resize(image1.size());
    eyeLabel->show();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_D)
    {
        num = num % 9;
        char eyeName[128],sceneName[128];
        sprintf((char*)&eyeName[0],".//calibImage//eye_%d.png",num);
        sprintf((char*)&sceneName[0],".//calibImage//scene_%d.png",num);
        cvtColor(sceneImage,sceneImage,CV_RGB2BGR);
        //cvtColor(eyeImage,eyeImage,CV_RGB2BGR);
        cv::imwrite(eyeName,orig_eye);
        cv::imwrite(sceneName,sceneImage);
        num++;
        this->update();
    }
    QWidget::keyPressEvent(event);
}

void MainWindow::updateImage()
{
    sceneCap>>sceneImage;
    eyeCap>>eyeImage;
    if(sceneImage.data && eyeImage.data)
    {
        flip(sceneImage,sceneImage,0);//1代表水平方向旋转180度
        orig_eye = eyeImage.clone();
        Mat proc;
        //clock_t start=clock();
        QTime time;
        time.start();

        /*cv::resize(eyeImage, proc, Size(0, 0), 0.2, 0.2, 1);
        TrackerParams params;
        params.Radius_Max = 10;
        params.Radius_Min = 8;
        params.Pupil_center = Point2f(0, 0);
        params.Corneal_center=Point2f(0,0);

        findPupilEllipse(proc, params);
        CCircle_detector::compute_ellipse(eyeImage, in, params.Pupil_center,params.Corneal_center);
        eyeVector.x = params.Pupil_center.x - 320;//params.Corneal_center.x;
        eyeVector.y = params.Pupil_center.y - 240;//params.Corneal_center.y;
*/

        EyeData data;
        Size inputSize;
        inputSize.width = 640;
        inputSize.height = 480;
        data.input = Mat(inputSize, eyeImage.type() );
        cv::resize(eyeImage, data.input, inputSize);

        if (data.input.channels() > 1) // TODO: make it algorithm dependent
                    cvtColor(data.input, data.input, CV_BGR2GRAY);
        data.pupil = Pupil();
                data.validPupil = false;
        QPointF sROI, eROI;
        sROI = QPointF(0,0);
        eROI = QPointF(1,1);
        Rect userROI = Rect(
                Point(sROI.x() * data.input.cols, sROI.y() * data.input.rows),
                Point( eROI.x() * data.input.cols, eROI.y() * data.input.rows)
            );
        float scalingFactor = 1;
        //scalingFactor = 1.0 / cfg.processingDownscalingFactor;//需要看看

        Mat downscaled;
        cv::resize(data.input(userROI), downscaled, Size(),
               scalingFactor, scalingFactor,
               INTER_AREA);

        Rect coarseROI = {0, 0, downscaled.cols, downscaled.rows };
        coarseROI = PupilDetectionMethod::coarsePupilDetection( downscaled, 0.5f, 60, 40);
        data.coarseROI = Rect(
                             userROI.tl() + coarseROI.tl() / scalingFactor,
                             userROI.tl() + coarseROI.br() / scalingFactor
                        );
        PupilDetectionMethod *pupilDetectionMethod = new ElSe();//new PuRe();
        pupilDetectionMethod->run( downscaled, coarseROI, data.pupil );
        data.pupil.confidence = PupilDetectionMethod::outlineContrastConfidence(downscaled, data.pupil);
        if (data.pupil.center.x > 0 && data.pupil.center.y > 0) {
            // Upscale
            data.pupil.resize( 1.0 / scalingFactor );

            // User region shift
            data.pupil.shift( userROI.tl() );
            data.validPupil = true;
        }
        Mat imgRGB;
        cvtColor(data.input, eyeImage, COLOR_GRAY2RGB);
        ellipse(eyeImage,Point(data.pupil.center.x ,data.pupil.center.y),Size(data.pupil.size.width/2,data.pupil.size.height/2),data.pupil.angle,0,360,CV_RGB(255,0,0),1,8);
        Point point = Point(data.pupil.center.x ,data.pupil.center.y );
        circle(eyeImage,point,2,CV_RGB(255,0,0),-1);



        eyeVector.x = point.x - 320;//params.Corneal_center.x;
        eyeVector.y = point.y - 240;//params.Corneal_center.y;

        Point2f curr=Homography_map_point(eyeVector,map_matrix);
        if(((curr.x - gaze.x)*(curr.x - gaze.x) + (curr.y - gaze.y)*(curr.y - gaze.y) < 25*25))
        {
            gaze.x=gaze.x*0.8+curr.x*0.2;
            gaze.y=gaze.y*0.8+curr.y*0.2;
        }
        else
        {
            gaze.x=curr.x;
            gaze.y=curr.y;
        }
        cv::circle(sceneImage,gaze,5,CV_RGB(0,255,0),-1);
        //std::cout << double(clock() - start)<< std::endl;
        //std::cout << time.elapsed()<< std::endl;
        cvtColor(sceneImage,sceneImage,CV_BGR2RGB);
        cvtColor(eyeImage,eyeImage,CV_BGR2RGB);

        compute_angle(gaze);
        pitch_s=QString::number(pitch,'f',6)+QString::fromLocal8Bit("°");
        yaw_s=QString::number(yaw,'f',6)+QString::fromLocal8Bit("°");
        ui->pitch_qs->setText(pitch_s);
        ui->yaw_qs->setText(yaw_s);

        this->update();
    }
}

void MainWindow::clickCalibButton()
{
    ShowCal *calshow = new ShowCal();
    calshow->show();
}


void MainWindow::clickReloadButton()
{
    FILE* fp ;//C://Users//Hewie//Desktop//613
    fp = fopen(".//matrix.txt", "r" );
    if(fp==NULL)
    {
        QMessageBox::information(this,"提示","can not open matrix.txt!");
    }
    //     fseek( fp, 0L, SEEK_SET );
    double temp[3][3]={0};

    for(int i=0; i<3;i++)
    {

        fscanf( fp, "%lf %lf %lf", &temp[i][0],&temp[i][1],&temp[i][2]);

        this->map_matrix[i][0]=temp[i][0];
        this->map_matrix[i][1]=temp[i][1];
        this->map_matrix[i][2]=temp[i][2];
    }

    fclose(fp);
}

CvPoint2D32f  MainWindow::Homography_map_point(CvPoint2D32f  p,double map_matrix[][3])
{
    CvPoint2D32f  p2;
    double z = map_matrix[2][0]*p.x + map_matrix[2][1]*p.y + map_matrix[2][2];
    p2.x = (float)((map_matrix[0][0]*p.x + map_matrix[0][1]*p.y + map_matrix[0][2])/z);
    p2.y = (float)((map_matrix[1][0]*p.x + map_matrix[1][1]*p.y + map_matrix[1][2])/z);
    return p2;
}


void MainWindow::compute_angle(cv::Point2f gaze)
{
    yaw=qAtan((gaze.x-320.0)*l_perPix/z_length)*(180.0/3.1415926);
    pitch=qAtan((240.0-gaze.y)*l_perPix/qSqrt(z_length*z_length+((gaze.x-320.0)*l_perPix)*(gaze.x-320.0)*l_perPix))*(180.0/3.1415926);
}

void MainWindow::on_pushButton_2_clicked()
{
    ShowPoint *showPoint = new ShowPoint();
    showPoint->show();
}

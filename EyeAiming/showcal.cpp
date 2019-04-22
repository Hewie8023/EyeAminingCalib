#include "showcal.h"
#include "ui_showcal.h"
#include "ccorneal.h"

#include "ElSe.h"
#include "PuRe.h"
#include "utils.h"
#include "eyeimageprocessor.h"



ShowCal::ShowCal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowCal)
{
    ui->setupUi(this);
    m_CalNum = 0;
    this->setWindowIcon(QIcon("eye.ico"));
    //this->setWindowTitle(QString::fromLocal8Bit("九点标定_第") + QString::number(m_CalNum+1,'f',0) + QString::fromLocal8Bit("图"));
    this->setWindowTitle(QString::number(m_CalNum+1,'f',0));


    sceneLabel = new QLabel(this);
    eyeLabel = new QLabel(this);
    ui->horizontalLayout->addWidget(sceneLabel);
    ui->horizontalLayout->addWidget(eyeLabel);
    sceneLabel->installEventFilter(this);
    eyeLabel->installEventFilter(this);
    //connect(sceneLabel, SIGNAL(clicked()), this, SLOT(updateImage()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(clicknextButton()));
    connect(ui->computeButton, SIGNAL(clicked()), this, SLOT(clickcomputeButton()));

    char eyeName[128],sceneName[128];
    sprintf((char*)&eyeName[0],".//calibImage//eye_%d.png",m_CalNum);
    sprintf((char*)&sceneName[0],".//calibImage//scene_%d.png",m_CalNum);
    sceneImage = imread(sceneName);
    eyeImage = imread(eyeName);
    this->update();

}

ShowCal::~ShowCal()
{
    delete sceneLabel;
    delete eyeLabel;
    delete DoCalibration;
    delete ui;
}
void ShowCal::paintEvent(QPaintEvent *e)
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
/*
void ShowCal::updateImage()
{

    char eyeName[128],sceneName[128];
    sprintf((char*)&eyeName[0],"C://Users//Hewie//Desktop//613//calibImage//eye_%d.png",m_CalNum);
    sprintf((char*)&sceneName[0],"C://Users//Hewie//Desktop//613//calibImage//scene_%d.png",m_CalNum);
    sceneImage = imread(sceneName);
    eyeImage = imread(eyeName);
    if(sceneImage.data && eyeImage.data)
    {
        Mat proc;
        cv::resize(eyeImage, proc, Size(0, 0), 0.2, 0.2, 1);
        TrackerParams params;
        params.Radius_Max = 10;
        params.Radius_Min = 8;
        params.Pupil_center = Point2f(0, 0);
        params.Corneal_center=Point2f(0,0);
        findPupilEllipse(proc, params);

        CCircle_detector::compute_ellipse(eyeImage, in, params.Pupil_center,params.Corneal_center);

        vectors[m_CalNum].x = params.Pupil_center.x - params.Corneal_center.x;
        vectors[m_CalNum].y = params.Pupil_center.y - params.Corneal_center.y;
        cvtColor(sceneImage,sceneImage,CV_BGR2RGB);
        cvtColor(eyeImage,eyeImage,CV_BGR2RGB);
        this->update();
    }
}*/

bool ShowCal::eventFilter(QObject *obj, QEvent *event)
{
    char eyeName[128],sceneName[128];
    sprintf((char*)&eyeName[0],".//calibImage//eye_%d.png",m_CalNum);
    sprintf((char*)&sceneName[0],".//calibImage//scene_%d.png",m_CalNum);
    if(qobject_cast<QLabel*>(obj)==sceneLabel&&event->type() == QEvent::MouseButtonPress)
    {
        sceneLabel->setStyleSheet("background-color: rgb(0, 255, 255);");
        m_point = cursor().pos();
        m_point = sceneLabel->mapFromGlobal(m_point);
        scenecalpoints[m_CalNum].x = m_point.rx();
        scenecalpoints[m_CalNum].y = m_point.ry();

        sceneImage=imread(sceneName);
        cv::circle(sceneImage,cv::Point(m_point.rx(),m_point.ry()),5,CV_RGB(0,255,0),-1);

        cvtColor(sceneImage,sceneImage,CV_BGR2RGB);

        eyeImage=imread(eyeName);
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
        PupilDetectionMethod *pupilDetectionMethod = new ElSe();// new PuRe();
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
        ellipse(eyeImage,Point(data.pupil.center.x ,data.pupil.center.y),Size(data.pupil.size.width/2,data.pupil.size.height/2),data.pupil.angle,0,360,CV_RGB(0,0,255),1,8);
        Point point = Point(data.pupil.center.x ,data.pupil.center.y );
        circle(eyeImage,point,2,CV_RGB(0,0,255),-1);

        vectors[m_CalNum].x = point.x - 320;//corneal_center.x;
        vectors[m_CalNum].y = point.y - 240;//corneal_center.y;

        this->update();
        return true;
    }
/*    else if(qobject_cast<QLabel*>(obj)==eyeLabel&&event->type() == QEvent::MouseButtonPress) {
        eyeLabel->setStyleSheet("background-color: rgb(0, 255, 255);");
        m_point = cursor().pos();
        m_point = eyeLabel->mapFromGlobal(m_point);
        cv::Point2f center=Point2f(0.0,0.0);
        cv::Point2f corneal_center;
        center.x = m_point.rx();
        center.y = m_point.ry();

        eyeImage=imread(eyeName);

        IplImage *rgbI = &IplImage(eyeImage);
        IplImage * image = cvCreateImage(cvGetSize(rgbI), IPL_DEPTH_8U, 1);//单通道处理
        IplImage *threshold_image=cvCreateImage(cvGetSize(rgbI), IPL_DEPTH_8U, 1);//
        if (eyeImage.channels() == 3)
        {
            cvCvtColor(rgbI, image, CV_BGR2GRAY);
        }
        else
        {
            cvCopy(rgbI, image);
        }
        cvSmooth(image, image, CV_GAUSSIAN, 5, 5);


        CCorneal CorImage;
        CCircle_detector inner_circle;
        CorImage.remove_corneal_reflection(image, threshold_image, center.x, center.y,
                (int)image->height/10, CorImage.corneal_reflection.x, CorImage.corneal_reflection.y, CorImage.corneal_reflection_r);

        CCircle_detector::Draw_Cross(rgbI, CorImage.corneal_reflection.x, CorImage.corneal_reflection.y, 5, 5, CV_RGB(0,255,0));
        corneal_center.x=CorImage.corneal_reflection.x;
        corneal_center.y=CorImage.corneal_reflection.y;
        inner_circle.starburst_circle_contour_detection((unsigned char*)image->imageData, image->width, image->height,
                                                        inner_circle.edge_threshold, inner_circle.rays, inner_circle.min_feature_candidates, center.x, center.y, image->widthStep / sizeof(uchar));
        int inliers_num = 0;
        CvSize ellipse_axis;
        if (inner_circle.edge_point.size() > 15)
        {
            inner_circle.pupil_fitting_inliers((unsigned char*)image->imageData, image->width, image->height, inliers_num);
            ellipse_axis.width = (int)inner_circle.circle_param[0];
            ellipse_axis.height = (int)inner_circle.circle_param[1];
            CvPoint pupil = { 0,0 };
            pupil.x = inner_circle.circle_param[2];
            pupil.y = inner_circle.circle_param[3];
            CvScalar Red = CV_RGB(255, 0, 0);
            cvEllipse(rgbI, pupil, ellipse_axis, -inner_circle.circle_param[4] * 180 / PI, 0, 360, Red, 1);
            CCircle_detector::Draw_Cross(rgbI, pupil.x, pupil.y, 5, 5, Red);
            center.x=pupil.x;
            center.y=pupil.y;
        }
        rgbI = NULL;
        cvReleaseImage(&threshold_image);
        cvReleaseImage(&image);

        vectors[m_CalNum].x = center.x - 320;//corneal_center.x;
        vectors[m_CalNum].y = center.y - 240;//corneal_center.y;

///////////////////////////////////////////////////////////////////
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
        PupilDetectionMethod *pupilDetectionMethod =  new ElSe();//new PuRe();
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
        ellipse(eyeImage,Point(data.pupil.center.x ,data.pupil.center.y),Size(data.pupil.size.width/2,data.pupil.size.height/2),data.pupil.angle,0,360,CV_RGB(0,0,255),1,8);
        Point point = Point(data.pupil.center.x ,data.pupil.center.y );
        circle(eyeImage,point,2,CV_RGB(0,0,255),-1);

        vectors[m_CalNum].x = point.x - 320;//corneal_center.x;
        vectors[m_CalNum].y = point.y - 240;//corneal_center.y;
        ///////////////////////////////////////////////////////////////////

        //cvtColor(eyeImage,eyeImage,CV_BGR2RGB);

        this->update();
        return true;
    }*/
    else
    {
        return false;
    }
}
void ShowCal::clicknextButton()
{
    if(m_CalNum == 0)
    {
        //this->updateImage();
    }
    else if(m_CalNum == 9){
        m_CalNum = 0;
        //this->updateImage();
    }
    m_CalNum++;
    m_CalNum = m_CalNum % 9;
    char eyeName[128],sceneName[128];
    sprintf((char*)&eyeName[0],".//calibImage//eye_%d.png",m_CalNum);
    sprintf((char*)&sceneName[0],".//calibImage//scene_%d.png",m_CalNum);
    sceneImage = imread(sceneName);
    eyeImage = imread(eyeName);
    //this->setWindowTitle(QString::fromLocal8Bit("九点标定_第")+QString::number(m_CalNum+1,'f',0)+QString::fromLocal8Bit("图"));
    this->setWindowTitle(QString::number(m_CalNum+1,'f',0));
    this->update();
}

void ShowCal::clickcomputeButton()
{
    DoCalibration = new CDoCalibration();
    DoCalibration->cal_calibration_homography(scenecalpoints,vectors);
    FILE  *stream ;
    stream = fopen(".//matrix.txt", "w" );

    for(  int i=0; i<3; i++ )
    {

        fprintf( stream, "%lf", DoCalibration->map_matrix[i][0] );
        fprintf( stream, "%s", " ");
        fprintf( stream, "%lf", DoCalibration->map_matrix[i][1] );
        fprintf( stream, "%s", " ");
        fprintf( stream, "%lf", DoCalibration->map_matrix[i][2] );
        fprintf( stream, "%s", "\n" );

    }
    fclose(stream);
}

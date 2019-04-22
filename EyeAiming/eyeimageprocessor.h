#ifndef EYEIMAGEPROCESSOR_H
#define EYEIMAGEPROCESSOR_H

#include <QObject>

#include <opencv/cv.h>

#include "InputWidget.h"


#include "ElSe.h"

#ifdef STARBURST
#include "pupil-detection/Starburst.h"
#endif
#ifdef SWIRSKI
#include "pupil-detection/Swirski.h"
#endif
#include "PupilDetectionMethod.h"


class EyeData : public InputData {
public:
    explicit EyeData(){
        timestamp = 0;
        input = cv::Mat();
        pupil = Pupil();
        validPupil = false;
        processingTimestamp = 0;
    }

    cv::Mat input;
    Pupil pupil;
    bool validPupil;
    cv::Rect coarseROI;

    // TODO: header, toQString, and the reading from file (see the Calibration class) should be unified
    // to avoid placing things in the wrong order / with the wrong string
    QString header(QString prefix = "") const {
        QString tmp;
        tmp.append(prefix + "timestamp");
        tmp.append(gDataSeparator);
        tmp.append(prefix + "pupil.x");
        tmp.append(gDataSeparator);
        tmp.append(prefix + "pupil.y");
        tmp.append(gDataSeparator);
        tmp.append(prefix + "pupil.width");
        tmp.append(gDataSeparator);
        tmp.append(prefix + "pupil.height");
        tmp.append(gDataSeparator);
        tmp.append(prefix + "pupil.angle");
        tmp.append(gDataSeparator);
        tmp.append(prefix + "pupil.confidence");
        tmp.append(gDataSeparator);
        tmp.append(prefix + "pupil.valid");
        tmp.append(gDataSeparator);
        tmp.append(prefix + "processingTime");
        tmp.append(gDataSeparator);
        return tmp;
    }

    QString toQString() const {
        QString tmp;
        tmp.append(QString::number(timestamp));
        tmp.append(gDataSeparator);
        tmp.append(QString::number(pupil.center.x));
        tmp.append(gDataSeparator);
        tmp.append(QString::number(pupil.center.y));
        tmp.append(gDataSeparator);
        tmp.append(QString::number(pupil.size.width));
        tmp.append(gDataSeparator);
        tmp.append(QString::number(pupil.size.height));
        tmp.append(gDataSeparator);
        tmp.append(QString::number(pupil.angle));
        tmp.append(gDataSeparator);
        tmp.append(QString::number(pupil.confidence));
        tmp.append(gDataSeparator);
        tmp.append(QString::number(validPupil));
        tmp.append(gDataSeparator);
        tmp.append(QString::number(processingTimestamp));
        tmp.append(gDataSeparator);
        return tmp;
    }
};

Q_DECLARE_METATYPE(EyeData)


class EyeImageProcessorConfig
{
public:
    EyeImageProcessorConfig()
        :
          inputSize(cv::Size(0, 0)),
          flip(CV_FLIP_NONE),
          undistort(false),
          coarseDetection(false),
          processingDownscalingFactor(1),
          tracking(true)
    {}

    cv::Size inputSize;
    CVFlip flip;
    bool undistort;
    bool coarseDetection;
    double processingDownscalingFactor;
    QString pupilDetectionMethod;
    bool tracking;

    void save(QSettings *settings)
    {
        settings->sync();
        settings->setValue("width", inputSize.width);
        settings->setValue("height", inputSize.height);
        settings->setValue("flip", flip);
        settings->setValue("undistort", undistort);
        settings->setValue("coarseDetection", coarseDetection);
        settings->setValue("processingDownscalingFactor", processingDownscalingFactor);
        settings->setValue("pupilDetectionMethod", pupilDetectionMethod);
        settings->setValue("tracking", tracking);
    }

    void load(QSettings *settings)
    {
        settings->sync();
        set1(settings, "width", inputSize.width);
        set1(settings, "height", inputSize.height);
        set1(settings, "flip", flip);
        set1(settings, "undistort", undistort);
        set1(settings, "coarseDetection", coarseDetection);
        set1(settings, "processingDownscalingFactor", processingDownscalingFactor);
        set1(settings, "pupilDetectionMethod", pupilDetectionMethod);
        set1(settings, "tracking", tracking);
    }
};


class EyeImageProcessor : public QObject
{
    Q_OBJECT
public:
    explicit EyeImageProcessor(QString id, QObject *parent = 0);
    ~EyeImageProcessor();
    QSettings *settings;
    QVector<PupilDetectionMethod*> availablePupilDetectionMethods;
    EyeImageProcessorConfig cfg;
    EyeData data;

signals:
    void newData(EyeData data);

public slots:
    void process(Timestamp t, const cv::Mat &frame);
    void updateConfig();
    void newROI(QPointF sROI, QPointF eROI);

private:
    QString id;
    QMutex cfgMutex;
    QPointF sROI, eROI;

    PupilDetectionMethod *pupilDetectionMethod;

    unsigned int pmIdx;
};

#endif // EYEIMAGEPROCESSOR_H

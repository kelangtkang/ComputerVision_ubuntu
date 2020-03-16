#ifndef PROCESSCAMERA_H
#define PROCESSCAMERA_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
#include <QThread>
#include <QWidget>
#include <QObject>
#include <QImage>

class ProcessCamera : public QThread
{
    Q_OBJECT

public:
    explicit ProcessCamera();
    ~ProcessCamera();
    void run();
    void stop();
    void process(cv::Mat);

private slots:
    void setOriginal();
    void setGrey();
    void setHSV();
    void setNegative();
    void slotSetResolution(int);

    void slotChangeCamera(int);
    void slotStartCamera();
    void slotOnTrackingToProcess();
    void slotOffTrackingToProcess();
    void slotFrameTrackingToProcess(cv::Mat);

signals:
    void sendFrameProcessed(QImage);
    void signalFrameToTracking(cv::Mat);
    void signalFrameToDetection(cv::Mat);

private:
    int numb;
    int m_deviceID;
    int m_frameWidth, m_frameHeight;
    bool m_stopped;
    bool m_tracking;
    QImage outputFrame;
    cv::Mat m_frameProcessed;
    cv::Mat m_frameOriginal;
    cv::Mat in;
    cv::Mat out;

    cv::VideoCapture *m_camera;
};

#endif // PROCESSCAMERA_H

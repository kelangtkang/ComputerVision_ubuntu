#ifndef COLORTRACKING_H
#define COLORTRACKING_H

#include "arduino.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QCoreApplication>
#include <QMainWindow>
#include <QThread>
#include <QMutex>

#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QSlider>
#include <QSpinBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

class ColorTracking : public QWidget
{
    Q_OBJECT

public:
    ColorTracking();
    ~ColorTracking();
    void stop();

    void createGUI();
    void morphObject(cv::Mat &thresh);
    void drawObject();
    void trackingObject(cv::Mat, cv::Mat);
    QGroupBox* createGroupBox(QSpinBox *m_spinMin, QSpinBox *m_spinMax, const QString Min, const QString Max, const QString group);

public slots:
    void H_minValueChange(int value);
    void H_maxValueChange(int value);
    void S_minValueChange(int value);
    void S_maxValueChange(int value);
    void V_minValueChange(int value);
    void V_maxValueChange(int value);

    void run(cv::Mat);
    void slotResetValue();
    void changeNamePushButton();

signals:
    void signalSerial(int i);
    void signalProcessToTracking();
    void signalImageToMainCamera(QImage);
    void signalFrameTrackingToProcess(cv::Mat);

private:
    cv::Mat m_frameProcessTracking;
    cv::Mat m_frameOriginalDetection;
    cv::Mat m_frameHadTracking;
    cv::Mat m_frameThreshold;
    cv::Mat m_frameTracking;
    cv::Mat aaa;

    bool m_stopped;

    int m_index;
    int x = 0, y = 0;
    int H_min, H_max, S_min, S_max, V_min, V_max;
    double m_area;
    double m_refArea;
    double m_sideLength;
    const int MIN_OBJECT_AREA = 25 * 25;
    const int MAX_OBJECT_AREA = 500 * 500;

    QPushButton *m_onOffArduino;
    QSpinBox *m_spinHMin, *m_spinHMax, *m_spinSMin, *m_spinSMax, *m_spinVMin, *m_spinVMax;

    QLabel *m_labelThreshold;
    QHBoxLayout *m_ngang;
    QVBoxLayout *m_doc;

    Arduino *m_arduino;
    QMutex m_mutex;
};

#endif // COLORTRACKING_H

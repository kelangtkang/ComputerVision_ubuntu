#ifndef OBJECTDETECTION_H
#define OBJECTDETECTION_H

#include "arduino.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QApplication>
#include <QMainWindow>
#include <QCloseEvent>
#include <QThread>

#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QImage>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

class ObjectDetection : public QWidget
{
    Q_OBJECT

public:
    ObjectDetection();
    ~ObjectDetection();
    void createGUI();

private slots:
    void run(cv::Mat);

private:
    cv::Mat m_frameProcessDetection;
    cv::Mat m_frameOriginalDetection;
    cv::Mat m_frameDetection;
    QImage m_imageDetection;

    QWidget *window;

    QLabel *m_detectionCamera;
    QHBoxLayout *m_ngang;

};

#endif // OBJECTDETECTION_H

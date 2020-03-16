#include "objectdetection.h"

ObjectDetection::ObjectDetection()
{
}

ObjectDetection::~ObjectDetection()
{
}

void ObjectDetection::run(cv::Mat frameDetection)
{
    frameDetection.copyTo(m_frameDetection);

    cv::cvtColor(m_frameDetection, m_frameProcessDetection, cv::COLOR_BGR2RGB);
    QImage m_imageDetection((const uchar *) m_frameProcessDetection.data, m_frameProcessDetection.cols, m_frameProcessDetection.rows, m_frameProcessDetection.step1(), QImage::Format_RGB888);
    m_detectionCamera -> setPixmap(QPixmap::fromImage(m_imageDetection));
}

void ObjectDetection::createGUI()
{
    m_detectionCamera = new QLabel();
    m_ngang = new QHBoxLayout(this);
    m_ngang -> addWidget(m_detectionCamera);
    this -> setWindowTitle("Real Time Object Detection");
}

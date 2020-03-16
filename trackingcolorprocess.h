#ifndef TRACKINGCOLORPROCESS_H
#define TRACKINGCOLORPROCESS_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QObject>
#include <QImage>

class TrackingColorProcess : public QObject
{
    Q_OBJECT

public:
    typedef struct
    {
        std::string name;
        int x;
        int y;
        int H_min = H_min;
        int H_max = H_max;
        int S_min = S_min;
        int S_max = S_max;
        int V_min = V_min;
        int V_max = V_max;
        cv::Mat threshold;
    }inf;
    inf infObject;

    TrackingColorProcess();
    ~TrackingColorProcess();

    void filterColor();
//    void trackingObject(int &x, int &y, inf tempObject, cv::Mat &m_frameProcessed);
    void breakLoop();

public slots:
    void H_minValueChange(int value);
    void H_maxValueChange(int value);
    void S_minValueChange(int value);
    void S_maxValueChange(int value);
    void V_minValueChange(int value);
    void V_maxValueChange(int value);

signals:
    void sendFrameThreshold(QImage frame);

private:
    cv::Mat m_frameProcessed;
    cv::Mat m_originalFrame;
    cv::Mat m_threshold;
    cv::VideoCapture *m_camera;

    bool m_breakLoop;
    int x = 0, y = 0;
    int H_min, H_max, S_min, S_max, V_min, V_max;
};

#endif // TRACKINGCOLORPROCESS_H

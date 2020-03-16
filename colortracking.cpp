#include "colortracking.h"

ColorTracking::ColorTracking() //Phuong thuc khoi tao mac dinh
{
    //Khoi tao gia tri ban dau
    H_min = 0; H_max = 255;
    S_min = 0; S_max = 255;
    V_min = 0; V_max = 255;

    m_stopped = false;
    m_arduino = new Arduino;

    //windows -> setAttribute(Qt::WA_DeleteOnClose, true);
}

ColorTracking::~ColorTracking() //Phuong thuc huy
{
//    m_arduino -> ~Arduino();
    delete m_arduino;
}

void ColorTracking::stop()
{
    m_stopped = true;
}

void ColorTracking::run(cv::Mat frameOriginal) //Xu ly hinh anh va nhan dien doi tuong
{
    frameOriginal.copyTo(m_frameTracking);
    cv::cvtColor(m_frameTracking, m_frameProcessTracking, cv::COLOR_BGR2HSV);
    cv::Scalar lower = cv::Scalar(H_min, S_min, V_min);
    cv::Scalar higer = cv::Scalar(H_max, S_max, V_max);
    cv::inRange(m_frameProcessTracking, lower, higer, m_frameThreshold);

    this -> morphObject(m_frameThreshold);
    this -> trackingObject(m_frameThreshold, m_frameTracking);

    emit signalFrameTrackingToProcess(m_frameTracking);

//    cv::cvtColor(m_frameTracking, aaa, cv::COLOR_BGR2RGB);
//    QImage imageTracking((const uchar *) aaa.data, aaa.cols, aaa.rows, aaa.step1(), QImage::Format_RGB888);
//    emit signalImageToMainCamera(imageTracking);

    QImage imageThreshold((const unsigned char *) m_frameThreshold.data, m_frameThreshold.cols, m_frameThreshold.rows, QImage::Format_Indexed8);
    m_labelThreshold -> setPixmap(QPixmap::fromImage(imageThreshold));

    if ((this -> isHidden()) == true)
    {
        emit signalProcessToTracking(); //Chuyen sang camera tracking
        return;
    }
}

void ColorTracking::morphObject(cv::Mat &thresh)
{
    cv::Mat erodeElement = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat dilateElement = getStructuringElement(cv::MORPH_RECT, cv::Size(8, 8));

    cv::erode(thresh, thresh, erodeElement);
    cv::erode(thresh, thresh, erodeElement);
    cv::dilate(thresh, thresh, dilateElement);
    cv::dilate(thresh, thresh, dilateElement);
}

void ColorTracking::trackingObject(cv::Mat m_threshold, cv::Mat frameTraking)
{
    cv::Mat temp;
    m_threshold.copyTo(temp);

    std::vector < std::vector <cv::Point> > contours;
    std::vector < cv::Vec4i > hierarchy;
    cv::findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    m_refArea = 0;
    bool objectFound = false;

    if(contours.size() < 20 && contours.size() >0)
    {
        for(int index = 0; index >= 0 ; index = (hierarchy[index])[0])
        {
            cv::Moments moment = cv::moments((cv::Mat)contours[index]);
            m_area = moment.m00;
            m_sideLength = sqrt(m_area);
            if (m_area > MIN_OBJECT_AREA && m_area < MAX_OBJECT_AREA && m_area > m_refArea)
            {
                x = moment.m10 / m_area;
                y = moment.m01 / m_area;
                objectFound = true;
                m_refArea = m_area;
                m_index = index;
            }
            else objectFound = false;
        }

        if (objectFound == true)
        {
            drawObject();
            cv::Rect r = boundingRect(contours.at(m_index));
            cv::rectangle(frameTraking, r, cv::Scalar(0 , 255, 0), 2, 8, 0);
            cv::line(frameTraking, cv::Point(x - 10, y), cv::Point(x + 10, y), cv::Scalar(0, 0, 255), 2);
            cv::line(frameTraking, cv::Point(x, y - 10), cv::Point(x, y + 10), cv::Scalar(0, 0, 255), 2);
            emit signalSerial(1);
        }
        else emit signalSerial(0);
    }
}

void ColorTracking::drawObject()
{
}

void ColorTracking::H_minValueChange(int value) { H_min = value; }
void ColorTracking::H_maxValueChange(int value) { H_max = value; }
void ColorTracking::S_minValueChange(int value) { S_min = value; }
void ColorTracking::S_maxValueChange(int value) { S_max = value; }
void ColorTracking::V_minValueChange(int value) { V_min = value; }
void ColorTracking::V_maxValueChange(int value) { V_max = value; }

void ColorTracking::slotResetValue()
{
    H_min = 0; H_max = 255;
    S_min = 0; S_max = 255;
    V_min = 0; V_max = 255;
}

void ColorTracking::createGUI() //Tao giao dien
{
    //Tao cua so dieu chinh kenh mau
    //Tao nut dieu chinh kenh mau
    m_spinHMin = new QSpinBox;
    m_spinHMax = new QSpinBox;
    m_spinSMin = new QSpinBox;
    m_spinSMax = new QSpinBox;
    m_spinVMin = new QSpinBox;
    m_spinVMax = new QSpinBox;

    QGroupBox *grid_H = createGroupBox(m_spinHMin, m_spinHMax, "H Min", "H Max", "H color value");
    QGroupBox *grid_S = createGroupBox(m_spinSMin, m_spinSMax, "S Min", "S Max", "S color value");
    QGroupBox *grid_V = createGroupBox(m_spinVMin, m_spinVMax, "V Min", "V Max", "V color value");

    //Thay doi khoang gia tri mau HSV
    QObject::connect(m_spinHMin, SIGNAL(valueChanged(int)), this, SLOT(H_minValueChange(int)));
    QObject::connect(m_spinHMax, SIGNAL(valueChanged(int)), this, SLOT(H_maxValueChange(int)));
    QObject::connect(m_spinSMin, SIGNAL(valueChanged(int)), this, SLOT(S_minValueChange(int)));
    QObject::connect(m_spinSMax, SIGNAL(valueChanged(int)), this, SLOT(S_maxValueChange(int)));
    QObject::connect(m_spinVMin, SIGNAL(valueChanged(int)), this, SLOT(V_minValueChange(int)));
    QObject::connect(m_spinVMax, SIGNAL(valueChanged(int)), this, SLOT(V_maxValueChange(int)));

    //Tao nut bat tat Arduino
    m_onOffArduino = new QPushButton("START");
    m_onOffArduino -> setFont(QFont("Calibri", 12, QFont::Bold));
    m_onOffArduino -> setStyleSheet("background-color: rgb(0, 102, 0);");
    m_onOffArduino -> setCursor(Qt::PointingHandCursor);

    QGridLayout *gridArduino = new QGridLayout;
    gridArduino -> addWidget(m_onOffArduino, 0, 0);
    QGroupBox *groupArduino = new QGroupBox("Arduino");
    groupArduino -> setLayout(gridArduino);

    QObject::connect(m_onOffArduino, SIGNAL(clicked()), this, SLOT(changeNamePushButton()));

    //Sap xep nut chuc nang
    m_doc = new QVBoxLayout;
    m_doc -> addWidget(grid_H);
    m_doc -> addWidget(grid_S);
    m_doc -> addWidget(grid_V);
    m_doc -> addWidget(groupArduino);

    //Sap xep giao dien
    m_labelThreshold = new QLabel;
    m_ngang = new QHBoxLayout;
    m_ngang -> addWidget(m_labelThreshold);
    m_ngang -> addLayout(m_doc);

    this -> setWindowTitle("Threshold Camera");
    this -> setLayout(m_ngang);
}

QGroupBox* ColorTracking::createGroupBox(QSpinBox *m_spinMin, QSpinBox *m_spinMax, const QString Min, const QString Max, const QString nameGroup)
{
    QLabel *MinLabel = new QLabel(Min);
    QSlider *m_sliderMin = new QSlider(Qt::Horizontal);
    m_sliderMin -> setMinimumWidth(150);
    m_sliderMin -> setRange(0, 255);
    m_spinMin -> setRange(0, 255);
    QObject::connect(m_sliderMin, SIGNAL(valueChanged(int)), m_spinMin, SLOT(setValue(int)));
    QObject::connect(m_spinMin, SIGNAL(valueChanged(int)), m_sliderMin, SLOT(setValue(int)));

    QLabel *MaxLabel = new QLabel(Max);
    QSlider *m_sliderMax = new QSlider(Qt::Horizontal);
    m_sliderMax -> setRange(0, 255);
    m_sliderMax -> setSliderPosition(255);
    m_spinMax -> setRange(0, 255);
    m_spinMax -> setValue(255);
    QObject::connect(m_sliderMax, SIGNAL(valueChanged(int)), m_spinMax, SLOT(setValue(int)));
    QObject::connect(m_spinMax, SIGNAL(valueChanged(int)), m_sliderMax, SLOT(setValue(int)));

    QGridLayout *grid = new QGridLayout;
    grid -> addWidget(MinLabel, 0, 1);
    grid -> addWidget(m_sliderMin, 1, 0);
    grid -> addWidget(m_spinMin, 1, 1);
    grid -> addWidget(MaxLabel, 2, 1);
    grid -> addWidget(m_sliderMax, 3, 0);
    grid -> addWidget(m_spinMax, 3, 1);
    QGroupBox *group = new QGroupBox(nameGroup);
    group -> setLayout(grid);

    return group;
}

void ColorTracking::changeNamePushButton()
{
    if(!m_onOffArduino -> text().compare("START"))
    {
        m_onOffArduino -> setText("STOP");
        m_onOffArduino -> setStyleSheet("background-color: rgb(255, 0, 0);");
        m_arduino -> run();
        QObject::connect(this, SIGNAL(signalSerial(int)), m_arduino, SLOT(actionArduino(int)));
    }
    else
    {
        m_onOffArduino -> setText("START");
        m_onOffArduino -> setStyleSheet("background-color: rgb(0, 102, 0);");
        QObject::disconnect(this, SIGNAL(signalSerial(int)), m_arduino, SLOT(actionArduino(int)));
    }
}

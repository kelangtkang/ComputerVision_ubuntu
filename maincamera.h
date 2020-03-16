#ifndef MAINCAMERA_H
#define MAINCAMERA_H

#include <processcamera.h>
#include <colortracking.h>
#include <objectdetection.h>
#include <cameralabel.h>

#include <QString>
#include <QThread>
#include <QImage>
#include <QCamera>
#include <QtMultimedia>
#include <QtMultimediaWidgets>

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

#include <QMenuBar>
#include <QAction>
#include <QActionGroup>

#include <QMetaType>
#include <QDebug>

class MainCamera : public QMainWindow
{
    Q_OBJECT

public:
    MainCamera();
    ~MainCamera();
    void createMenu();
    void createChucNang();
    void createThongTin();
    QPushButton* createButton(const QString nameButton);

private slots:
    void changeNamePushButton();
    void createCamera(QImage frame);
    void updateCameraDevice(QAction *);
    void trackingColor();
    void objectDetection();
    void slotSetValueInfo(int x, int y, int r, int g, int b);
    void slotSetResolution(QAction *);

signals:
    void signal_onOff();
    void signalStartCamera();
    void signalChangeEffect(int);
    void signalDeviceID(int);
    void signalResolution(int);

private:
    ColorTracking *m_colorTracking;
    ObjectDetection *m_objectDetection;
    ProcessCamera *m_processMain;

    CameraLabel *m_labelCamera;
    QLabel *m_xValue, *m_yValue, *m_rValue, *m_gValue, *m_bValue;

    QPushButton *m_onOffCamera;
    QGroupBox *m_groupThongTin, *m_groupEffect, *m_groupDetect;

    QAction *m_settings, *m_exit;
    QAction *m_1080x720, *m_720x480, *m_800x600, *m_640x480, *m_320x240;
};

#endif // MAINCAMERA_H

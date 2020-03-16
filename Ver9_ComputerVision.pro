INCLUDEPATH += /usr/local/include

CONFIG += c++11

TARGET = ComputerVision

QT += widgets
QT += core gui serialport
QT += multimedia multimediawidgets

LIBS += /usr/local/lib/libopencv_core.so.3.4.0
LIBS += /usr/local/lib/libopencv_highgui.so.3.4.0
LIBS += /usr/local/lib/libopencv_imgcodecs.so.3.4.0
LIBS += /usr/local/lib/libopencv_imgproc.so.3.4.0

LIBS += /usr/local/lib/libopencv_features2d.so.3.4.0
LIBS += /usr/local/lib/libopencv_calib3d.so.3.4.0

LIBS += /usr/local/lib/libopencv_videoio.so.3.4.0
LIBS += /usr/local/lib/libopencv_videostab.so.3.4.0

SOURCES += \
    processcamera.cpp \
    maincamera.cpp \
    main.cpp \
    arduino.cpp \
    objectdetection.cpp \
    colortracking.cpp \
    cameralabel.cpp

HEADERS += \
    processcamera.h \
    maincamera.h \
    arduino.h \
    objectdetection.h \
    colortracking.h \
    cameralabel.h

RESOURCES += \
    File.qrc

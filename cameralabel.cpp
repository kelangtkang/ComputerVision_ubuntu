#include "cameralabel.h"

CameraLabel::CameraLabel()
{
    setMouseTracking(true);
    setCursor(Qt::CrossCursor);
}

void CameraLabel::mouseMoveEvent(QMouseEvent *event)
{    
    QPointF positionCusor = event -> pos();
    m_xValue = positionCusor.x();
    m_yValue = positionCusor.y();

    if(this -> pixmap() != 0) //Kiem tra QLabel empty hay khong, neu empty thi con tro chuot vao QLabel se crash
    {
        QRgb rgbValue = pixmap() -> toImage().pixel(positionCusor.x(), positionCusor.y());
        m_rValue = qRed(rgbValue);
        m_gValue = qGreen(rgbValue);
        m_bValue = qBlue(rgbValue);
    }
    event -> accept();
    emit signalPosionCusor(m_xValue, m_yValue, m_rValue, m_gValue, m_bValue);
}

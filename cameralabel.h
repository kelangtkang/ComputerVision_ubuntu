#ifndef CAMERALABEL_H
#define CAMERALABEL_H

#include <QLabel>
#include <QMouseEvent>

class CameraLabel : public QLabel
{
    Q_OBJECT

public:
    CameraLabel();

signals:
    void signalPosionCusor(int x, int y, int r, int g, int b);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    int m_xValue, m_yValue;
    float m_rValue, m_gValue, m_bValue;
};

#endif // CAMERALABEL_H

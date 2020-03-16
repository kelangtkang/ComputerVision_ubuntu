#ifndef ARDUINO_H
#define ARDUINO_H

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QWidget>
#include <QMessageBox>
#include <QString>

class Arduino : public QWidget
{
    Q_OBJECT

public:
    Arduino();
    ~Arduino();
    void run();

public slots:
    void actionArduino(int i);

private:
    QSerialPort *m_arduino;
    static const quint16 m_vendor_id = 9025;
    static const quint16 m_product_id = 67;
    QString m_portName;
    bool m_arduinoAvailable;
};

#endif // ARDUINO_H

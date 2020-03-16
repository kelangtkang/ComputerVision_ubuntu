#include "arduino.h"

Arduino::Arduino()
{
    m_arduino = new QSerialPort;
}

Arduino::~Arduino()
{
    delete m_arduino;
}

void Arduino::run()
{
    m_arduinoAvailable = false;
    m_portName = "";

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
        {
            if(serialPortInfo.vendorIdentifier() == m_vendor_id)
            {
                if(serialPortInfo.productIdentifier() == m_product_id)
                {
                    m_portName = serialPortInfo.portName();
                    m_arduinoAvailable = true;
                }
            }
        }

    }

    if(m_arduinoAvailable)
    {
        m_arduino -> setPortName(m_portName);
        m_arduino -> open(QSerialPort::WriteOnly);
        m_arduino -> setBaudRate(QSerialPort::Baud9600);
        m_arduino -> setDataBits(QSerialPort::Data8);
        m_arduino -> setParity(QSerialPort::NoParity);
        m_arduino -> setStopBits(QSerialPort::OneStop);
        m_arduino -> setFlowControl(QSerialPort::NoFlowControl);
    }
    else
    {
        QMessageBox::warning(this, "Port error", "Couldn't find the Arduino!");
    }
}

void Arduino::actionArduino(int i)
{
    if (i == 1)
        m_arduino -> write("1");
    else if (i ==0)
        m_arduino -> write("0");
}

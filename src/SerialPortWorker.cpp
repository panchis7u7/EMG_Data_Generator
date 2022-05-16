#include <include/SerialPortWorker.hpp>
#include <QDebug>
#include <QThread>

SerialPortWorker::SerialPortWorker(QQueue<char*>& serialBufferQueue, QObject* parent): QObject(parent), m_qqSerialBufferQueue(serialBufferQueue) {
    m_bWorking = false;
    m_bAborted = false;
    m_qspSerialPort = nullptr;
}

SerialPortWorker::~SerialPortWorker() {
    if(m_qspSerialPort) {
        if(m_qspSerialPort->isOpen()) m_qspSerialPort->close();
        delete m_qspSerialPort;
    }
}

int SerialPortWorker::connect(QString comPort) {
    m_qspSerialPort = new QSerialPort();
    m_qspSerialPort->setPortName(comPort);
    m_qspSerialPort->setBaudRate(QSerialPort::BaudRate::Baud9600);
    m_qspSerialPort->setDataBits(QSerialPort::DataBits::Data8);
    m_qspSerialPort->setParity(QSerialPort::Parity::NoParity);
    m_qspSerialPort->setStopBits(QSerialPort::StopBits::OneStop);
    m_qspSerialPort->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    m_qspSerialPort->open(QIODevice::OpenModeFlag::ReadWrite);
    qDebug() << "SerialPort Status: " << m_qspSerialPort->isOpen();
}

void SerialPortWorker::requestWork() {
    m_qmxMutex.lock();
    m_bWorking = true;
    m_bAborted = false;
    qDebug() << "Request worker start in thread " << thread()->currentThreadId();
    m_qmxMutex.unlock();
}

void SerialPortWorker::abortWork() {
    m_qmxMutex.lock();
    if(m_bWorking) {
        m_bAborted = true;
        qDebug() << "Request worker start in thread " << thread()->currentThreadId();
    }
    m_qmxMutex.unlock();
}

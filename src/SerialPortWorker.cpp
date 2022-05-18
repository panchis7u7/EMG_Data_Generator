#include <include/SerialPortWorker.hpp>
#include <QDebug>
#include <QThread>

SerialPortWorker::SerialPortWorker(QQueue<Frame*>& serialBufferQueue, QObject* parent): QObject(parent), m_qqSerialBufferQueue(serialBufferQueue) {
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
    if(!m_qspSerialPort->isOpen())
        return -1;
    return 0;
}

int SerialPortWorker::disconnect() {
    if (m_qspSerialPort->isOpen()) {
        m_qspSerialPort->close();
        return 0;
    }
    return -1;
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

void SerialPortWorker::sendFrame(Frame* frame) {

}

void SerialPortWorker::sendData(Frame* data) {

}

void SerialPortWorker::doWork() {
    qDebug() << "Starting worker process in thread " << thread()->currentThreadId();
    quint8 inByte = 0, xored = 0, nDataBytes = 0, checksum = 0;
    Frame* inFrame;
    RecieverStatus status = RecieverStatus::RCV_STATE_IDLE;

    while(!m_bAborted) {
        //Check if there is alredy a frame pending in the queue.
        if(!m_qqSerialBufferQueue.isEmpty()) {
            Frame* outFrame = m_qqSerialBufferQueue.dequeue();
            sendFrame(outFrame);
            delete outFrame;
            // If there are no frames queued, wait 10 msec for serial availability.
        } else if(m_qspSerialPort->waitForReadyRead(10)) {
                QByteArray recievedData = m_qspSerialPort->readAll();
                while(recievedData.count() > 0) {
                    inByte = quint8(recievedData[0]);
                    recievedData.remove(0,1);

                    if(inByte == Frame::FRAME_ESCAPE_CHAR)
                        xored = Frame::FRAME_XOR_CHAR;
                    else {
                        inByte ^= xored;
                        xored = 0x00;

                        switch(status) {
                            case RCV_STATE_IDLE:
                                if(inByte == Frame::FRAME_START) {
                                    if(inFrame == nullptr)
                                        inFrame = new Frame();
                                    else
                                        inFrame->clear();
                                    inFrame->addByte(inByte);
                                    checksum = inByte;
                                    status = RecieverStatus::RCV_STATE_CMDID;
                                }
                            break;

                            case RCV_STATE_CMDID:
                                inFrame->addByte(inByte);
                                checksum += inByte;
                                status = RecieverStatus::RCV_STATE_DATA_LEN;
                            break;

                            case RCV_STATE_DATA_LEN:
                                nDataBytes = inByte;
                                inFrame->addByte(inByte);
                                checksum += inByte;
                                status = RecieverStatus::RCV_STATE_DATA;
                            break;

                            case RCV_STATE_DATA:
                                inFrame->addByte(inByte);
                                checksum += inByte;
                                if(--nDataBytes == 0)
                                    status = RecieverStatus::RCV_STATE_CHECKSUM;
                                else if(nDataBytes < 0)
                                    status = RecieverStatus::RCV_STATE_IDLE;
                            break;

                            case RCV_STATE_CHECKSUM:
                                if(inByte == checksum) {
                                    status = RecieverStatus::RCV_STATE_IDLE;
                                    inFrame->addByte(checksum);
                                    this->frameReceieved(inFrame);
                                } else {
                                    status = RecieverStatus::RCV_STATE_IDLE;
                                    inFrame->clear();
                                    delete inFrame;
                                }
                                nDataBytes = inByte;
                                inFrame->addByte(inByte);
                                checksum += inByte;
                            break;
                        }
                    }
                }
            }
        }

    if(m_qspSerialPort != nullptr) {
        m_qspSerialPort->close();
        qDebug() << "SerialPort Closed";
        delete m_qspSerialPort;
        qDebug() << "SerialPort destroyed";

        // Set _working to false, meaning the process can't be aborted anymore.
        m_qmxMutex.lock();
        m_bWorking = false;
        m_qmxMutex.unlock();

        qDebug() << "Worker process finished in thread "<<thread()->currentThreadId();

        emit finished();
    }
}

void SerialPortWorker::frameReceieved(Frame* frame) {
    qDebug() << frame->getStringData();
}

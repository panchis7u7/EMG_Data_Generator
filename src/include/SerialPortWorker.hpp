#ifndef SERIALPORTWORKER_HPP
#define SERIALPORTWORKER_HPP

#include <QObject>
#include <QQueue>
#include <QString>
#include <QMutex>
#include <QtSerialPort/QSerialPort>
#include <include/Frame.hpp>

typedef enum RecieverStatus {
    RCV_STATE_IDLE = 0,
    RCV_STATE_CMDID,
    RCV_STATE_DATA_LEN,
    RCV_STATE_DATA,
    RCV_STATE_CHECKSUM
} RecieverStatus;

class SerialPortWorker: public QObject {
   Q_OBJECT

public:
    SerialPortWorker(QQueue<Frame*>& SerialBufferQueue, QObject* parent = nullptr);
    ~SerialPortWorker();
    int connect(QString comPort);
    int disconnect();
    void requestWork();
    void abortWork();

private:
    bool m_bWorking;
    bool m_bAborted;
    QMutex m_qmxMutex;
    QSerialPort* m_qspSerialPort;
    QQueue<Frame*> m_qqSerialBufferQueue;
    quint8 calculateChecksum(QByteArray buffer);
    void sendData(Frame* frame);
    void frameReceieved(Frame* frame);

signals:
    void workRequested();
    void finished();

public slots:
    void doWork();
    void sendFrame(Frame* frame);
};

#endif // SERIALPORTWORKER_HPP

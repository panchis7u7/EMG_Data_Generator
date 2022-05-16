#ifndef SERIALPORTWORKER_HPP
#define SERIALPORTWORKER_HPP

#include <QObject>
#include <QQueue>
#include <QString>
#include <QMutex>
#include <QtSerialPort/QSerialPort>

class SerialPortWorker: public QObject {
   Q_OBJECT

public:
    SerialPortWorker(QQueue<char*>& SerialBufferQueue, QObject* parent = nullptr);
    ~SerialPortWorker();
    int connect(QString comPort);
    int disconnect();
    void requestWork();
    void abortWork();

private:
    static constexpr unsigned short RCV_SOF = 0;
    static constexpr unsigned short RCV_CMDID = 1;
    static constexpr unsigned short RCV_LEN = 2;
    static constexpr unsigned short RCV_DATA = 3;
    static constexpr unsigned short RCV_CHECKSUM = 4;

    bool m_bWorking;
    bool m_bAborted;
    QMutex m_qmxMutex;
    QSerialPort* m_qspSerialPort;
    QQueue<char*> m_qqSerialBufferQueue;
    quint8 calculateChecksum(QByteArray buffer);
    void sendData(char* frame);

signals:
public slots:
};

#endif // SERIALPORTWORKER_HPP

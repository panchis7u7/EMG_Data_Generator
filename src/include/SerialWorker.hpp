#ifndef SERIALWORKER_HPP
#define SERIALWORKER_HPP

#include <QObject>
#include <QMutex>
#include <QtSerialPort/QSerialPort>
#include <QQueue>
#include <include/Frame.hpp>

class SerialWorker: QObject {
    Q_OBJECT
public:
    explicit SerialWorker(QQueue<void>* OutFrameQueue, QObject* parent = nullptr);
    ~SerialWorker();

private:
    static constexpr unsigned short RCV_SOF = 0;
    static constexpr unsigned short RCV_CMDID = 1;
    static constexpr unsigned short RCV_LEN = 2;
    static constexpr unsigned short RCV_DATA = 3;
    static constexpr unsigned short RCV_CHECKSUM = 4;

    QMutex mutex;
    QSerialPort *m_Serial;
    QQueue<Frame*> *m_outFrameQueue;

signals:

public slots:

};

#endif // SERIALWORKER_HPP

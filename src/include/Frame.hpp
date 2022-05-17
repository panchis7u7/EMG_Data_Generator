#ifndef FRAME_HPP
#define FRAME_HPP

#include <QObject>

typedef enum FrameHeaders {
    SOF = 0,
    CMDID = 1,
    LEN = 2,
    DATA = 3,
    CHECKSUM = 4
} FrameHeaders;

class Frame: QObject {
    Q_OBJECT

public:
    explicit Frame(quint8 cmd, QByteArray data, QObject* parent = nullptr);
    ~Frame();
    QByteArray getBuffer();
    quint8 getCMDID();
    quint8 getDataLen();
    quint8 getByte(unsigned index);
    void clear();
    void addByte(quint8);

private:
    static constexpr unsigned short FRAME_START = 0x8A;

    unsigned calculateChecksum();

    QByteArray m_qbarrBuffer;
};

#endif // FRAME_HPP

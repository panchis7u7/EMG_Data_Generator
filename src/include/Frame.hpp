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

typedef enum FrameCommands {
    CAPTURE_DATA_REQUEST = 0,
} FrameCommands;

class Frame: public QObject {
    Q_OBJECT

public:
    static constexpr quint8 FRAME_START = 0x8A;
    static constexpr quint8 FRAME_ESCAPE_CHAR = 0x8B;
    static constexpr quint8 FRAME_XOR_CHAR = 0x20;
    static constexpr quint8 FRAME_NUM_EXTRA_BYTES = 4;

    explicit Frame(QObject* parent = nullptr);
    explicit Frame(quint8 cmd, QByteArray data, QObject* parent = nullptr);
    ~Frame();
    QByteArray getBuffer();
    quint8 getCMDID();
    quint8 getDataLen();
    quint8 getByte(unsigned index);
    QString getStringData();
    void clear();
    void addByte(quint8);

private:
    unsigned calculateChecksum();

    QByteArray m_qbarrBuffer;
};

#endif // FRAME_HPP

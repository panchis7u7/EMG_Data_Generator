#ifndef FRAME_HPP
#define FRAME_HPP

#include <QObject>

class Frame: public QObject {
    Q_OBJECT
public:
    static constexpr quint8 FRAME_START = 0x8A;
    static constexpr quint8 FRAME_ESCAPE_CHAR = 0x8B;
    static constexpr quint8 FRAME_XOR_CHAR = 0x20;
    static constexpr quint8 FRAME_NUM_EXTRA_BYTES = 4;

    static constexpr unsigned INDEX_START_OF_FRAME = 0;
    static constexpr unsigned INDEX_CMD = 1;
    static constexpr unsigned INDEX_DATA_LENGTH = 2;
    static constexpr unsigned INDEX_FIRST_DATA_BYTE = 3;

    explicit Frame(QObject* parent = nullptr);
    explicit Frame(quint8 cmd, QByteArray data, QObject *parent = nullptr);
    ~Frame();

    quint8  getCmd();
    quint8  getDataLength();
    quint8  getIndexedByte(int index);
    quint8  calculateChecksum();
    QByteArray getBuffer();
    void    clear();
    void    addByte(quint8 data);

private:
    QByteArray m_baBuffer;

signals:

public slots:
};

#endif // FRAME_HPP

#include <include/Frame.hpp>

Frame::Frame(QObject* parent): QObject(parent) {}

Frame::Frame(quint8 cmd, QByteArray data, QObject *parent): QObject(parent) {
    m_baBuffer[INDEX_START_OF_FRAME] = FRAME_START;
    m_baBuffer[INDEX_CMD] = cmd;
    quint8 dataCount = quint8(data.count());
    m_baBuffer[INDEX_DATA_LENGTH] = dataCount;

    for(int i = 0; i < dataCount; ++i)
        m_baBuffer[INDEX_FIRST_DATA_BYTE + i] = quint8(data[i]);

    m_baBuffer += calculateChecksum();
}

Frame::~Frame() { m_baBuffer.clear(); }

quint8 Frame::getCmd() {
    quint8 val = 0;
    if(m_baBuffer.count() >= INDEX_CMD)
        val = quint8(m_baBuffer[INDEX_CMD]);
    return val;
}

quint8 Frame::getDataLength() {
    quint8 val = 0;
    if(m_baBuffer.count() >= INDEX_DATA_LENGTH)
        val = quint8(m_baBuffer[INDEX_DATA_LENGTH]);
    return val;
}

quint8 Frame::getIndexedByte(int index) {
    quint8 val = 0;
    if(getDataLength() >= index)
        val = quint8(m_baBuffer[INDEX_FIRST_DATA_BYTE + index]);
    return val;
}

quint8 Frame::calculateChecksum() {
    quint8 val = 0;
    for(int i = 0; i < m_baBuffer.count(); ++i)
        val += quint8(m_baBuffer[i]);
    return val;
}

QByteArray Frame::getBuffer() { return m_baBuffer; }

void Frame::clear() { m_baBuffer.clear(); }

void Frame::addByte(quint8 data) { m_baBuffer.append(data); }

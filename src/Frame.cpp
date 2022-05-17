#include <include/Frame.hpp>

Frame::Frame(QObject* parent): QObject(parent) {}

Frame::Frame(quint8 cmd, QByteArray data, QObject* parent): QObject(parent) {
    qsizetype dataBytesCount = data.count();
    m_qbarrBuffer[FrameHeaders::SOF] = FRAME_START;
    m_qbarrBuffer[FrameHeaders::CMDID] = cmd;
    m_qbarrBuffer[FrameHeaders::LEN] = dataBytesCount;

    for(int i = 0; i < dataBytesCount; ++i)
        m_qbarrBuffer[FrameHeaders::DATA + i] = data[i];

    m_qbarrBuffer.append(calculateChecksum());
}

Frame::~Frame() {}

unsigned Frame::calculateChecksum() {
    quint8 checksum = 0;

    for(auto& byte : m_qbarrBuffer)
        checksum += quint8(byte);

    return checksum;
}

QByteArray Frame::getBuffer() { return m_qbarrBuffer; }

quint8 Frame::getCMDID() { return (m_qbarrBuffer.count() > 3) ? quint8(m_qbarrBuffer[FrameHeaders::CMDID]) : 0; }

quint8 Frame::getDataLen() { return (m_qbarrBuffer.count() > FrameHeaders::CMDID) ? quint8(m_qbarrBuffer[FrameHeaders::LEN]) : 0; }

quint8 Frame::getByte(unsigned int index) { return (getDataLen() > 0) ? quint8(m_qbarrBuffer[FrameHeaders::DATA + index]) : 0; }

void Frame::clear() { m_qbarrBuffer.clear(); }

void Frame::addByte(quint8 byte) { m_qbarrBuffer.append(byte); }

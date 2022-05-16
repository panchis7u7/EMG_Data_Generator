#include <QApplication>
#include <QThread>
#include <QQueue>
#include <include/MainWindow.hpp>
#include <include/SerialPortWorker.hpp>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(768, 768);
    window.show();

    QQueue<char*> serialBufferQueue;

    QThread* threadSerial = new QThread();
    SerialPortWorker* serialWorker = new SerialPortWorker(serialBufferQueue);

    serialWorker->moveToThread(threadSerial);
    return app.exec();
}

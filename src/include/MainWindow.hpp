#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QQueue>
#include <include/SerialPortWorker.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupGuiComponents();

private slots:
    void on_pbSerialCD_clicked();

private:
    Ui::MainWindow *ui;
    SerialPortWorker* m_spwWorker;
    QQueue<char*> m_qqSerialBufferQueue;
};

#endif // MAINWINDOW_HPP

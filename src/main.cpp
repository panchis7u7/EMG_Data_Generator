#include <QApplication>
#include <include/MainWindow.hpp>
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(400, 300);
    window.show();

    return app.exec();
}

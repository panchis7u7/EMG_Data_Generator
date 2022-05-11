#include <QApplication>
#include <include/MainWindow.hpp>
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(768, 768);
    window.show();

    return app.exec();
}

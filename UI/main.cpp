#include "mywindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    
    QApplication app(argc, argv);
    MyWindow window;
    window.setMinimumSize(QSize(1500,800));
    window.show();

    return app.exec();

    // MyWindow window(argc, argv);
    // window.show();
    // return window.run();
}


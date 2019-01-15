#include "loadprofiledialog.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //LoadProfileDialog w;
    //w.grabGesture(Qt::PanGesture);
    //w.grabGesture(Qt::PinchGesture);
    w.show();

    return a.exec();
}

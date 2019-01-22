#include "loadprofiledialog.h"
#include "mainwindow.h"
#include <QApplication>
#include "code.cpp"

//Q_DECLARE_METATYPE(BsplineTask)
Q_DECLARE_METATYPE(Points)
Q_DECLARE_METATYPE(Bspline)

int main(int argc, char *argv[])
{
    qRegisterMetaType<BsplineTask>();
    qRegisterMetaType<Points>();
    qRegisterMetaType<Bspline>();
    //main2();

    QApplication a(argc, argv);
    MainWindow w;
    //LoadProfileDialog w;
    //w.grabGesture(Qt::PanGesture);
    //w.grabGesture(Qt::PinchGesture);
    w.show();

    return a.exec();
}

#include "loadprofiledialog.h"
#include "mainwindow.h"
#include <Ellipse.h>
#include <QApplication>
#include "code.cpp"

//Q_DECLARE_METATYPE(BsplineTask)
Q_DECLARE_METATYPE(Points)
Q_DECLARE_METATYPE(doubles)
Q_DECLARE_METATYPE(Bspline)

int main(int argc, char *argv[])
{
    MAKE_PROTOTYPE(UniformKS)
    MAKE_PROTOTYPE(ValueKS)
    MAKE_PROTOTYPE(MultiplicityValueKS)
    MAKE_PROTOTYPE(BeginKS)
    MAKE_PROTOTYPE(EndKS)
    MAKE_PROTOTYPE(RationalKS)
    MAKE_PROTOTYPE(BiRationalKS)
    MAKE_PROTOTYPE(CumulKS)
    MAKE_PROTOTYPE(CustomKS)

    qRegisterMetaType<BsplineTask>();
    qRegisterMetaType<Points>();
    qRegisterMetaType<Bspline>();
    qRegisterMetaType<doubles>();
    //main2();

    QApplication a(argc, argv);
    MainWindow w;
    //LoadProfileDialog w;
    //w.grabGesture(Qt::PanGesture);
    //w.grabGesture(Qt::PinchGesture);
    w.show();

    return a.exec();
}

#-------------------------------------------------
#
# Project created by QtCreator 2019-01-08T12:16:03
#
#-------------------------------------------------

QT += core gui
QT += charts concurrent
QT += quickcontrols2
QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTShapeParametrization
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../ShapeParametrizationCpp/include
INCLUDEPATH += $$PWD/../ShapeParametrizationCpp/src

CONFIG += c++11
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp
QMAKE_CFLAGS_RELEASE += -fopenmp
QMAKE_CFLAGS_DEBUG += -fopenmp
LIBS += -fopenmp

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        loadprofiledialog.cpp \
        profiledata.cpp \
        #$$PWD/../ShapeParametrizationCpp/src/Table.cpp \
        #$$PWD/../ShapeParametrizationCpp/src/Point.cpp \
        #$$PWD/../ShapeParametrizationCpp/src/Line.cpp \
        $$PWD/../ShapeParametrizationCpp/src/*.cpp \
    mychart.cpp \
    mychartview.cpp \
    RangeSlider.cpp \
    rangesliderlayout.cpp \
    MultiSliderView.cpp \
    code.cpp \
    bsplinetask.cpp \
    qtutils.cpp \
    knotlabel.cpp \
    knotlistsource.cpp \
    knotlistdest.cpp \
    optimizeknotsdialog.cpp

HEADERS += \
        mainwindow.h \
        loadprofiledialog.h \
        profiledata.h \
        #$$PWD/../ShapeParametrizationCpp/include/Table.h \
        #$$PWD/../ShapeParametrizationCpp/include/Point.h \
        #$$PWD/../ShapeParametrizationCpp/include/Line.h \
        $$PWD/../ShapeParametrizationCpp/include/*.h \
    mainwindow.h \
    mychartview.h \
    mychart.h \
    RangeSlider.h \
    rangesliderlayout.h \
    MultiSliderView.h \
    bsplinetask.h \
    qtutils.h \
    knotlabel.h \
    knotlistsource.h \
    knotlistdest.h \
    mychart.h \
    optimizeknotsdialog.h


#HEADERS += *.h
#SOURCES += *.cpp


FORMS +=


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    licence_header

/****************************************************************************
** Meta object code from reading C++ file 'mychartview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QTShapeParametrization/mychartview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mychartview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChartView_t {
    QByteArrayData data[8];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChartView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChartView_t qt_meta_stringdata_ChartView = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ChartView"
QT_MOC_LITERAL(1, 10, 14), // "clickableEvent"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "QScatterSeries*"
QT_MOC_LITERAL(4, 42, 7), // "series1"
QT_MOC_LITERAL(5, 50, 7), // "series2"
QT_MOC_LITERAL(6, 58, 5), // "point"
QT_MOC_LITERAL(7, 64, 8) // "from1to2"

    },
    "ChartView\0clickableEvent\0\0QScatterSeries*\0"
    "series1\0series2\0point\0from1to2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChartView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3, QMetaType::QPointF, QMetaType::Bool,    4,    5,    6,    7,

       0        // eod
};

void ChartView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChartView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clickableEvent((*reinterpret_cast< QScatterSeries*(*)>(_a[1])),(*reinterpret_cast< QScatterSeries*(*)>(_a[2])),(*reinterpret_cast< QPointF(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QScatterSeries* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChartView::*)(QScatterSeries * , QScatterSeries * , QPointF , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChartView::clickableEvent)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ChartView::staticMetaObject = { {
    &QChartView::staticMetaObject,
    qt_meta_stringdata_ChartView.data,
    qt_meta_data_ChartView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ChartView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChartView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChartView.stringdata0))
        return static_cast<void*>(this);
    return QChartView::qt_metacast(_clname);
}

int ChartView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QChartView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ChartView::clickableEvent(QScatterSeries * _t1, QScatterSeries * _t2, QPointF _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

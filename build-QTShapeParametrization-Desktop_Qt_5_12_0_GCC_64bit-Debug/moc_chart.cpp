/****************************************************************************
** Meta object code from reading C++ file 'chart.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QTShapeParametrization/chart.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chart.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Chart_t {
    QByteArrayData data[12];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Chart_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Chart_t qt_meta_stringdata_Chart = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Chart"
QT_MOC_LITERAL(1, 6, 11), // "seriesAdded"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 16), // "QAbstractSeries*"
QT_MOC_LITERAL(4, 36, 6), // "series"
QT_MOC_LITERAL(5, 43, 6), // "zoomed"
QT_MOC_LITERAL(6, 50, 6), // "bounds"
QT_MOC_LITERAL(7, 57, 13), // "boundsChanged"
QT_MOC_LITERAL(8, 71, 15), // "changeZoomLevel"
QT_MOC_LITERAL(9, 87, 10), // "emitSignal"
QT_MOC_LITERAL(10, 98, 18), // "enableBoundChanges"
QT_MOC_LITERAL(11, 117, 3) // "val"

    },
    "Chart\0seriesAdded\0\0QAbstractSeries*\0"
    "series\0zoomed\0bounds\0boundsChanged\0"
    "changeZoomLevel\0emitSignal\0"
    "enableBoundChanges\0val"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Chart[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    1,   47,    2, 0x06 /* Public */,
       7,    1,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   53,    2, 0x09 /* Protected */,
       8,    0,   56,    2, 0x29 /* Protected | MethodCloned */,
      10,    1,   57,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QRectF,    6,
    QMetaType::Void, QMetaType::QRectF,    6,

 // slots: parameters
    QMetaType::QRectF, QMetaType::Bool,    9,
    QMetaType::QRectF,
    QMetaType::Void, QMetaType::Bool,   11,

       0        // eod
};

void Chart::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Chart *_t = static_cast<Chart *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->seriesAdded((*reinterpret_cast< QAbstractSeries*(*)>(_a[1]))); break;
        case 1: _t->zoomed((*reinterpret_cast< QRectF(*)>(_a[1]))); break;
        case 2: _t->boundsChanged((*reinterpret_cast< QRectF(*)>(_a[1]))); break;
        case 3: { QRectF _r = _t->changeZoomLevel((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QRectF*>(_a[0]) = std::move(_r); }  break;
        case 4: { QRectF _r = _t->changeZoomLevel();
            if (_a[0]) *reinterpret_cast< QRectF*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->enableBoundChanges((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSeries* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Chart::*)(QAbstractSeries * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Chart::seriesAdded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Chart::*)(QRectF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Chart::zoomed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Chart::*)(QRectF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Chart::boundsChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Chart::staticMetaObject = { {
    &QtCharts::QChart::staticMetaObject,
    qt_meta_stringdata_Chart.data,
    qt_meta_data_Chart,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Chart::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Chart::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Chart.stringdata0))
        return static_cast<void*>(this);
    return QtCharts::QChart::qt_metacast(_clname);
}

int Chart::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtCharts::QChart::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Chart::seriesAdded(QAbstractSeries * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Chart::zoomed(QRectF _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Chart::boundsChanged(QRectF _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

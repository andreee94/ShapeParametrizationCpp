/****************************************************************************
** Meta object code from reading C++ file 'loadprofiledialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QTShapeParametrization/loadprofiledialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loadprofiledialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LoadProfileDialog_t {
    QByteArrayData data[16];
    char stringdata0[226];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LoadProfileDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LoadProfileDialog_t qt_meta_stringdata_LoadProfileDialog = {
    {
QT_MOC_LITERAL(0, 0, 17), // "LoadProfileDialog"
QT_MOC_LITERAL(1, 18, 2), // "ok"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 12), // "ProfileData*"
QT_MOC_LITERAL(4, 35, 4), // "data"
QT_MOC_LITERAL(5, 40, 22), // "RadioCartesianSelected"
QT_MOC_LITERAL(6, 63, 24), // "RadioCylindricalSelected"
QT_MOC_LITERAL(7, 88, 14), // "OpenFileDialog"
QT_MOC_LITERAL(8, 103, 15), // "ComboXActivated"
QT_MOC_LITERAL(9, 119, 5), // "index"
QT_MOC_LITERAL(10, 125, 15), // "ComboYActivated"
QT_MOC_LITERAL(11, 141, 15), // "ComboRActivated"
QT_MOC_LITERAL(12, 157, 20), // "CheckReverseXChanged"
QT_MOC_LITERAL(13, 178, 5), // "state"
QT_MOC_LITERAL(14, 184, 20), // "CheckReverseYChanged"
QT_MOC_LITERAL(15, 205, 20) // "CheckReverseZChanged"

    },
    "LoadProfileDialog\0ok\0\0ProfileData*\0"
    "data\0RadioCartesianSelected\0"
    "RadioCylindricalSelected\0OpenFileDialog\0"
    "ComboXActivated\0index\0ComboYActivated\0"
    "ComboRActivated\0CheckReverseXChanged\0"
    "state\0CheckReverseYChanged\0"
    "CheckReverseZChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LoadProfileDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    1,   70,    2, 0x08 /* Private */,
      10,    1,   73,    2, 0x08 /* Private */,
      11,    1,   76,    2, 0x08 /* Private */,
      12,    1,   79,    2, 0x08 /* Private */,
      14,    1,   82,    2, 0x08 /* Private */,
      15,    1,   85,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   13,

       0        // eod
};

void LoadProfileDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LoadProfileDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ok((*reinterpret_cast< ProfileData*(*)>(_a[1]))); break;
        case 1: _t->RadioCartesianSelected(); break;
        case 2: _t->RadioCylindricalSelected(); break;
        case 3: _t->OpenFileDialog(); break;
        case 4: _t->ComboXActivated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ComboYActivated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->ComboRActivated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->CheckReverseXChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->CheckReverseYChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->CheckReverseZChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LoadProfileDialog::*)(ProfileData * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LoadProfileDialog::ok)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LoadProfileDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_LoadProfileDialog.data,
    qt_meta_data_LoadProfileDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LoadProfileDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoadProfileDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LoadProfileDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int LoadProfileDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void LoadProfileDialog::ok(ProfileData * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

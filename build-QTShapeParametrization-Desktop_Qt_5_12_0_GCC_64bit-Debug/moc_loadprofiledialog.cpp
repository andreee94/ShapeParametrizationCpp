/****************************************************************************
** Meta object code from reading C++ file 'loadprofiledialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QTShapeParametrization/loadprofiledialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loadprofiledialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LoadProfileDialog_t {
    QByteArrayData data[8];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LoadProfileDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LoadProfileDialog_t qt_meta_stringdata_LoadProfileDialog = {
    {
QT_MOC_LITERAL(0, 0, 17), // "LoadProfileDialog"
QT_MOC_LITERAL(1, 18, 22), // "RadioCartesianSelected"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 24), // "RadioCylindricalSelected"
QT_MOC_LITERAL(4, 67, 14), // "OpenFileDialog"
QT_MOC_LITERAL(5, 82, 15), // "ComboXActivated"
QT_MOC_LITERAL(6, 98, 5), // "index"
QT_MOC_LITERAL(7, 104, 15) // "ComboYActivated"

    },
    "LoadProfileDialog\0RadioCartesianSelected\0"
    "\0RadioCylindricalSelected\0OpenFileDialog\0"
    "ComboXActivated\0index\0ComboYActivated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LoadProfileDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    1,   42,    2, 0x08 /* Private */,
       7,    1,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void LoadProfileDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LoadProfileDialog *_t = static_cast<LoadProfileDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RadioCartesianSelected(); break;
        case 1: _t->RadioCylindricalSelected(); break;
        case 2: _t->OpenFileDialog(); break;
        case 3: _t->ComboXActivated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->ComboYActivated((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
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
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

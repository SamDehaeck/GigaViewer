/****************************************************************************
** Meta object code from reading C++ file 'coordinator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../coordinator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'coordinator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Coordinator_t {
    QByteArrayData data[16];
    char stringdata[173];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Coordinator_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Coordinator_t qt_meta_stringdata_Coordinator = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Coordinator"
QT_MOC_LITERAL(1, 12, 19), // "controlCameraThread"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "startNew"
QT_MOC_LITERAL(4, 42, 3), // "dev"
QT_MOC_LITERAL(5, 46, 12), // "LoadNewMovie"
QT_MOC_LITERAL(6, 59, 11), // "StartNewAVT"
QT_MOC_LITERAL(7, 71, 13), // "StartNewVimba"
QT_MOC_LITERAL(8, 85, 11), // "StartNewIds"
QT_MOC_LITERAL(9, 97, 9), // "changeFps"
QT_MOC_LITERAL(10, 107, 6), // "newFps"
QT_MOC_LITERAL(11, 114, 15), // "stopAcquisition"
QT_MOC_LITERAL(12, 130, 13), // "changeShutter"
QT_MOC_LITERAL(13, 144, 4), // "time"
QT_MOC_LITERAL(14, 149, 14), // "setAutoShutter"
QT_MOC_LITERAL(15, 164, 8) // "fitRange"

    },
    "Coordinator\0controlCameraThread\0\0"
    "startNew\0dev\0LoadNewMovie\0StartNewAVT\0"
    "StartNewVimba\0StartNewIds\0changeFps\0"
    "newFps\0stopAcquisition\0changeShutter\0"
    "time\0setAutoShutter\0fitRange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Coordinator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x0a /* Public */,
       1,    1,   69,    2, 0x2a /* Public | MethodCloned */,
       5,    1,   72,    2, 0x0a /* Public */,
       6,    1,   75,    2, 0x0a /* Public */,
       7,    1,   78,    2, 0x0a /* Public */,
       8,    1,   81,    2, 0x0a /* Public */,
       9,    1,   84,    2, 0x0a /* Public */,
      11,    0,   87,    2, 0x0a /* Public */,
      12,    1,   88,    2, 0x0a /* Public */,
      14,    1,   91,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Bool,   15,

       0        // eod
};

void Coordinator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Coordinator *_t = static_cast<Coordinator *>(_o);
        switch (_id) {
        case 0: _t->controlCameraThread((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->controlCameraThread((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->LoadNewMovie((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->StartNewAVT((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->StartNewVimba((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->StartNewIds((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->changeFps((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->stopAcquisition(); break;
        case 8: _t->changeShutter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->setAutoShutter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Coordinator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Coordinator.data,
      qt_meta_data_Coordinator,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Coordinator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Coordinator::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Coordinator.stringdata))
        return static_cast<void*>(const_cast< Coordinator*>(this));
    return QObject::qt_metacast(_clname);
}

int Coordinator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE

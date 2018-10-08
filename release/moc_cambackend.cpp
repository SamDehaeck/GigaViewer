/****************************************************************************
** Meta object code from reading C++ file 'cambackend.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cambackend.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cambackend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CamBackend_t {
    QByteArrayData data[27];
    char stringdata[287];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CamBackend_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CamBackend_t qt_meta_stringdata_CamBackend = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CamBackend"
QT_MOC_LITERAL(1, 11, 13), // "NewImageReady"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 11), // "ImagePacket"
QT_MOC_LITERAL(4, 38, 2), // "im"
QT_MOC_LITERAL(5, 41, 14), // "shutterChanged"
QT_MOC_LITERAL(6, 56, 7), // "newTime"
QT_MOC_LITERAL(7, 64, 10), // "fpsChanged"
QT_MOC_LITERAL(8, 75, 4), // "msec"
QT_MOC_LITERAL(9, 80, 13), // "startTheTimer"
QT_MOC_LITERAL(10, 94, 8), // "interval"
QT_MOC_LITERAL(11, 103, 12), // "stopTheTimer"
QT_MOC_LITERAL(12, 116, 9), // "GrabFrame"
QT_MOC_LITERAL(13, 126, 14), // "StartRecording"
QT_MOC_LITERAL(14, 141, 5), // "start"
QT_MOC_LITERAL(15, 147, 7), // "recFold"
QT_MOC_LITERAL(16, 155, 5), // "codec"
QT_MOC_LITERAL(17, 161, 19), // "skipForwardBackward"
QT_MOC_LITERAL(18, 181, 7), // "forward"
QT_MOC_LITERAL(19, 189, 17), // "willStartTheTimer"
QT_MOC_LITERAL(20, 207, 16), // "willStopTheTimer"
QT_MOC_LITERAL(21, 224, 10), // "setRoiRows"
QT_MOC_LITERAL(22, 235, 4), // "rows"
QT_MOC_LITERAL(23, 240, 10), // "setRoiCols"
QT_MOC_LITERAL(24, 251, 4), // "cols"
QT_MOC_LITERAL(25, 256, 21), // "changedPluginSettings"
QT_MOC_LITERAL(26, 278, 8) // "settings"

    },
    "CamBackend\0NewImageReady\0\0ImagePacket\0"
    "im\0shutterChanged\0newTime\0fpsChanged\0"
    "msec\0startTheTimer\0interval\0stopTheTimer\0"
    "GrabFrame\0StartRecording\0start\0recFold\0"
    "codec\0skipForwardBackward\0forward\0"
    "willStartTheTimer\0willStopTheTimer\0"
    "setRoiRows\0rows\0setRoiCols\0cols\0"
    "changedPluginSettings\0settings"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CamBackend[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       5,    1,   92,    2, 0x06 /* Public */,
       7,    1,   95,    2, 0x06 /* Public */,
       9,    1,   98,    2, 0x06 /* Public */,
      11,    0,  101,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  102,    2, 0x0a /* Public */,
      13,    3,  103,    2, 0x0a /* Public */,
      13,    2,  110,    2, 0x2a /* Public | MethodCloned */,
      13,    1,  115,    2, 0x2a /* Public | MethodCloned */,
      17,    1,  118,    2, 0x0a /* Public */,
      19,    1,  121,    2, 0x0a /* Public */,
      20,    0,  124,    2, 0x0a /* Public */,
      21,    1,  125,    2, 0x0a /* Public */,
      23,    1,  128,    2, 0x0a /* Public */,
      25,    1,  131,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, QMetaType::QString,   14,   15,   16,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   14,   15,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void, QMetaType::QVariantMap,   26,

       0        // eod
};

void CamBackend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CamBackend *_t = static_cast<CamBackend *>(_o);
        switch (_id) {
        case 0: _t->NewImageReady((*reinterpret_cast< ImagePacket(*)>(_a[1]))); break;
        case 1: _t->shutterChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->fpsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->startTheTimer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->stopTheTimer(); break;
        case 5: _t->GrabFrame(); break;
        case 6: _t->StartRecording((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->StartRecording((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 8: _t->StartRecording((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->skipForwardBackward((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->willStartTheTimer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->willStopTheTimer(); break;
        case 12: _t->setRoiRows((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->setRoiCols((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->changedPluginSettings((*reinterpret_cast< QMap<QString,QVariant>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ImagePacket >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CamBackend::*_t)(ImagePacket );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CamBackend::NewImageReady)) {
                *result = 0;
            }
        }
        {
            typedef void (CamBackend::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CamBackend::shutterChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (CamBackend::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CamBackend::fpsChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (CamBackend::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CamBackend::startTheTimer)) {
                *result = 3;
            }
        }
        {
            typedef void (CamBackend::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CamBackend::stopTheTimer)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject CamBackend::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CamBackend.data,
      qt_meta_data_CamBackend,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CamBackend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CamBackend::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CamBackend.stringdata))
        return static_cast<void*>(const_cast< CamBackend*>(this));
    return QThread::qt_metacast(_clname);
}

int CamBackend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void CamBackend::NewImageReady(ImagePacket _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CamBackend::shutterChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CamBackend::fpsChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CamBackend::startTheTimer(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CamBackend::stopTheTimer()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

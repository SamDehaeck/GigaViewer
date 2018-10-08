/****************************************************************************
** Meta object code from reading C++ file 'cameracontrolsdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cameracontrolsdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cameracontrolsdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CameraControlsDialog_t {
    QByteArrayData data[22];
    char stringdata[334];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CameraControlsDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CameraControlsDialog_t qt_meta_stringdata_CameraControlsDialog = {
    {
QT_MOC_LITERAL(0, 0, 20), // "CameraControlsDialog"
QT_MOC_LITERAL(1, 21, 13), // "NeedNewSample"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 15), // "SetShutterSpeed"
QT_MOC_LITERAL(4, 52, 4), // "shut"
QT_MOC_LITERAL(5, 57, 14), // "SetAutoShutter"
QT_MOC_LITERAL(6, 72, 8), // "fitRange"
QT_MOC_LITERAL(7, 81, 10), // "SetRoiRows"
QT_MOC_LITERAL(8, 92, 4), // "rows"
QT_MOC_LITERAL(9, 97, 10), // "SetRoiCols"
QT_MOC_LITERAL(10, 108, 4), // "cols"
QT_MOC_LITERAL(11, 113, 12), // "GotNewSample"
QT_MOC_LITERAL(12, 126, 11), // "ImagePacket"
QT_MOC_LITERAL(13, 138, 3), // "imP"
QT_MOC_LITERAL(14, 142, 26), // "on_IntensityButton_clicked"
QT_MOC_LITERAL(15, 169, 30), // "on_shutterSpinBox_valueChanged"
QT_MOC_LITERAL(16, 200, 4), // "arg1"
QT_MOC_LITERAL(17, 205, 25), // "on_FitRangeButton_clicked"
QT_MOC_LITERAL(18, 231, 24), // "on_FitMeanButton_clicked"
QT_MOC_LITERAL(19, 256, 26), // "on_ROIRows_editingFinished"
QT_MOC_LITERAL(20, 283, 26), // "on_ROICols_editingFinished"
QT_MOC_LITERAL(21, 310, 23) // "on_pushButton_2_clicked"

    },
    "CameraControlsDialog\0NeedNewSample\0\0"
    "SetShutterSpeed\0shut\0SetAutoShutter\0"
    "fitRange\0SetRoiRows\0rows\0SetRoiCols\0"
    "cols\0GotNewSample\0ImagePacket\0imP\0"
    "on_IntensityButton_clicked\0"
    "on_shutterSpinBox_valueChanged\0arg1\0"
    "on_FitRangeButton_clicked\0"
    "on_FitMeanButton_clicked\0"
    "on_ROIRows_editingFinished\0"
    "on_ROICols_editingFinished\0"
    "on_pushButton_2_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CameraControlsDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    1,   80,    2, 0x06 /* Public */,
       5,    1,   83,    2, 0x06 /* Public */,
       7,    1,   86,    2, 0x06 /* Public */,
       9,    1,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   92,    2, 0x0a /* Public */,
      14,    0,   95,    2, 0x08 /* Private */,
      15,    1,   96,    2, 0x08 /* Private */,
      17,    0,   99,    2, 0x08 /* Private */,
      18,    0,  100,    2, 0x08 /* Private */,
      19,    0,  101,    2, 0x08 /* Private */,
      20,    0,  102,    2, 0x08 /* Private */,
      21,    0,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   10,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CameraControlsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CameraControlsDialog *_t = static_cast<CameraControlsDialog *>(_o);
        switch (_id) {
        case 0: _t->NeedNewSample(); break;
        case 1: _t->SetShutterSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->SetAutoShutter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->SetRoiRows((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->SetRoiCols((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->GotNewSample((*reinterpret_cast< ImagePacket(*)>(_a[1]))); break;
        case 6: _t->on_IntensityButton_clicked(); break;
        case 7: _t->on_shutterSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_FitRangeButton_clicked(); break;
        case 9: _t->on_FitMeanButton_clicked(); break;
        case 10: _t->on_ROIRows_editingFinished(); break;
        case 11: _t->on_ROICols_editingFinished(); break;
        case 12: _t->on_pushButton_2_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
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
            typedef void (CameraControlsDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraControlsDialog::NeedNewSample)) {
                *result = 0;
            }
        }
        {
            typedef void (CameraControlsDialog::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraControlsDialog::SetShutterSpeed)) {
                *result = 1;
            }
        }
        {
            typedef void (CameraControlsDialog::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraControlsDialog::SetAutoShutter)) {
                *result = 2;
            }
        }
        {
            typedef void (CameraControlsDialog::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraControlsDialog::SetRoiRows)) {
                *result = 3;
            }
        }
        {
            typedef void (CameraControlsDialog::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraControlsDialog::SetRoiCols)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject CameraControlsDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CameraControlsDialog.data,
      qt_meta_data_CameraControlsDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CameraControlsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraControlsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CameraControlsDialog.stringdata))
        return static_cast<void*>(const_cast< CameraControlsDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int CameraControlsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void CameraControlsDialog::NeedNewSample()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void CameraControlsDialog::SetShutterSpeed(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CameraControlsDialog::SetAutoShutter(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CameraControlsDialog::SetRoiRows(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CameraControlsDialog::SetRoiCols(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE

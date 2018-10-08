/****************************************************************************
** Meta object code from reading C++ file 'playbackdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../playbackdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'playbackdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PlaybackDialog_t {
    QByteArrayData data[32];
    char stringdata[472];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PlaybackDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PlaybackDialog_t qt_meta_stringdata_PlaybackDialog = {
    {
QT_MOC_LITERAL(0, 0, 14), // "PlaybackDialog"
QT_MOC_LITERAL(1, 15, 12), // "stopPlayback"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 6), // "newFps"
QT_MOC_LITERAL(4, 36, 3), // "fps"
QT_MOC_LITERAL(5, 40, 10), // "jumpFrames"
QT_MOC_LITERAL(6, 51, 7), // "forward"
QT_MOC_LITERAL(7, 59, 9), // "recordNow"
QT_MOC_LITERAL(8, 69, 7), // "checked"
QT_MOC_LITERAL(9, 77, 7), // "recFold"
QT_MOC_LITERAL(10, 85, 5), // "codec"
QT_MOC_LITERAL(11, 91, 21), // "on_stopButton_clicked"
QT_MOC_LITERAL(12, 113, 21), // "on_ffwdButton_clicked"
QT_MOC_LITERAL(13, 135, 20), // "on_rwdButton_clicked"
QT_MOC_LITERAL(14, 156, 21), // "on_playButton_toggled"
QT_MOC_LITERAL(15, 178, 20), // "on_recButton_toggled"
QT_MOC_LITERAL(16, 199, 22), // "on_RecSettings_clicked"
QT_MOC_LITERAL(17, 222, 21), // "on_backButton_clicked"
QT_MOC_LITERAL(18, 244, 21), // "on_toolButton_clicked"
QT_MOC_LITERAL(19, 266, 10), // "togglePlay"
QT_MOC_LITERAL(20, 277, 11), // "reversePlay"
QT_MOC_LITERAL(21, 289, 11), // "forwardPlay"
QT_MOC_LITERAL(22, 301, 24), // "on_fpsEdit_returnPressed"
QT_MOC_LITERAL(23, 326, 32), // "on_horizontalSlider_valueChanged"
QT_MOC_LITERAL(24, 359, 5), // "value"
QT_MOC_LITERAL(25, 365, 25), // "on_recTimedButton_toggled"
QT_MOC_LITERAL(26, 391, 18), // "finishedFirstTimer"
QT_MOC_LITERAL(27, 410, 19), // "finishedSecondTimer"
QT_MOC_LITERAL(28, 430, 22), // "newFrameNumberReceived"
QT_MOC_LITERAL(29, 453, 2), // "nr"
QT_MOC_LITERAL(30, 456, 10), // "showNewFps"
QT_MOC_LITERAL(31, 467, 4) // "msec"

    },
    "PlaybackDialog\0stopPlayback\0\0newFps\0"
    "fps\0jumpFrames\0forward\0recordNow\0"
    "checked\0recFold\0codec\0on_stopButton_clicked\0"
    "on_ffwdButton_clicked\0on_rwdButton_clicked\0"
    "on_playButton_toggled\0on_recButton_toggled\0"
    "on_RecSettings_clicked\0on_backButton_clicked\0"
    "on_toolButton_clicked\0togglePlay\0"
    "reversePlay\0forwardPlay\0"
    "on_fpsEdit_returnPressed\0"
    "on_horizontalSlider_valueChanged\0value\0"
    "on_recTimedButton_toggled\0finishedFirstTimer\0"
    "finishedSecondTimer\0newFrameNumberReceived\0"
    "nr\0showNewFps\0msec"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlaybackDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  124,    2, 0x06 /* Public */,
       3,    1,  125,    2, 0x06 /* Public */,
       5,    1,  128,    2, 0x06 /* Public */,
       7,    3,  131,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,  138,    2, 0x08 /* Private */,
      12,    0,  139,    2, 0x08 /* Private */,
      13,    0,  140,    2, 0x08 /* Private */,
      14,    1,  141,    2, 0x08 /* Private */,
      15,    1,  144,    2, 0x08 /* Private */,
      16,    0,  147,    2, 0x08 /* Private */,
      17,    0,  148,    2, 0x08 /* Private */,
      18,    0,  149,    2, 0x08 /* Private */,
      19,    0,  150,    2, 0x08 /* Private */,
      20,    0,  151,    2, 0x08 /* Private */,
      21,    0,  152,    2, 0x08 /* Private */,
      22,    0,  153,    2, 0x08 /* Private */,
      23,    1,  154,    2, 0x08 /* Private */,
      25,    1,  157,    2, 0x08 /* Private */,
      26,    0,  160,    2, 0x08 /* Private */,
      27,    0,  161,    2, 0x08 /* Private */,
      28,    1,  162,    2, 0x0a /* Public */,
      30,    1,  165,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, QMetaType::QString,    8,    9,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   29,
    QMetaType::Void, QMetaType::Int,   31,

       0        // eod
};

void PlaybackDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlaybackDialog *_t = static_cast<PlaybackDialog *>(_o);
        switch (_id) {
        case 0: _t->stopPlayback(); break;
        case 1: _t->newFps((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->jumpFrames((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->recordNow((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 4: _t->on_stopButton_clicked(); break;
        case 5: _t->on_ffwdButton_clicked(); break;
        case 6: _t->on_rwdButton_clicked(); break;
        case 7: _t->on_playButton_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->on_recButton_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->on_RecSettings_clicked(); break;
        case 10: _t->on_backButton_clicked(); break;
        case 11: _t->on_toolButton_clicked(); break;
        case 12: _t->togglePlay(); break;
        case 13: _t->reversePlay(); break;
        case 14: _t->forwardPlay(); break;
        case 15: _t->on_fpsEdit_returnPressed(); break;
        case 16: _t->on_horizontalSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->on_recTimedButton_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->finishedFirstTimer(); break;
        case 19: _t->finishedSecondTimer(); break;
        case 20: _t->newFrameNumberReceived((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->showNewFps((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PlaybackDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlaybackDialog::stopPlayback)) {
                *result = 0;
            }
        }
        {
            typedef void (PlaybackDialog::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlaybackDialog::newFps)) {
                *result = 1;
            }
        }
        {
            typedef void (PlaybackDialog::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlaybackDialog::jumpFrames)) {
                *result = 2;
            }
        }
        {
            typedef void (PlaybackDialog::*_t)(bool , QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlaybackDialog::recordNow)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject PlaybackDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PlaybackDialog.data,
      qt_meta_data_PlaybackDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PlaybackDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlaybackDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PlaybackDialog.stringdata))
        return static_cast<void*>(const_cast< PlaybackDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int PlaybackDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void PlaybackDialog::stopPlayback()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void PlaybackDialog::newFps(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PlaybackDialog::jumpFrames(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void PlaybackDialog::recordNow(bool _t1, QString _t2, QString _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE

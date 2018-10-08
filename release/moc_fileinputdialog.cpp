/****************************************************************************
** Meta object code from reading C++ file 'fileinputdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../fileinputdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fileinputdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FileInputDialog_t {
    QByteArrayData data[17];
    char stringdata[290];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FileInputDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FileInputDialog_t qt_meta_stringdata_FileInputDialog = {
    {
QT_MOC_LITERAL(0, 0, 15), // "FileInputDialog"
QT_MOC_LITERAL(1, 16, 16), // "StaticPicPressed"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 9), // "namegiven"
QT_MOC_LITERAL(4, 44, 17), // "OpencvFeedPressed"
QT_MOC_LITERAL(5, 62, 12), // "MoviePressed"
QT_MOC_LITERAL(6, 75, 14), // "AvtFeedPressed"
QT_MOC_LITERAL(7, 90, 16), // "VimbaFeedPressed"
QT_MOC_LITERAL(8, 107, 14), // "IdsFeedPressed"
QT_MOC_LITERAL(9, 122, 8), // "CloseApp"
QT_MOC_LITERAL(10, 131, 25), // "on_filePushButton_clicked"
QT_MOC_LITERAL(11, 157, 20), // "on_camButton_clicked"
QT_MOC_LITERAL(12, 178, 22), // "on_MovieButton_clicked"
QT_MOC_LITERAL(13, 201, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(14, 225, 20), // "on_AvtButton_clicked"
QT_MOC_LITERAL(15, 246, 22), // "on_vimbaButton_clicked"
QT_MOC_LITERAL(16, 269, 20) // "on_idsButton_clicked"

    },
    "FileInputDialog\0StaticPicPressed\0\0"
    "namegiven\0OpencvFeedPressed\0MoviePressed\0"
    "AvtFeedPressed\0VimbaFeedPressed\0"
    "IdsFeedPressed\0CloseApp\0"
    "on_filePushButton_clicked\0"
    "on_camButton_clicked\0on_MovieButton_clicked\0"
    "on_pushButton_2_clicked\0on_AvtButton_clicked\0"
    "on_vimbaButton_clicked\0on_idsButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FileInputDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    0,   87,    2, 0x06 /* Public */,
       5,    1,   88,    2, 0x06 /* Public */,
       6,    0,   91,    2, 0x06 /* Public */,
       7,    0,   92,    2, 0x06 /* Public */,
       8,    0,   93,    2, 0x06 /* Public */,
       9,    0,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   95,    2, 0x08 /* Private */,
      11,    0,   96,    2, 0x08 /* Private */,
      12,    0,   97,    2, 0x08 /* Private */,
      13,    0,   98,    2, 0x08 /* Private */,
      14,    0,   99,    2, 0x08 /* Private */,
      15,    0,  100,    2, 0x08 /* Private */,
      16,    0,  101,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FileInputDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FileInputDialog *_t = static_cast<FileInputDialog *>(_o);
        switch (_id) {
        case 0: _t->StaticPicPressed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->OpencvFeedPressed(); break;
        case 2: _t->MoviePressed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->AvtFeedPressed(); break;
        case 4: _t->VimbaFeedPressed(); break;
        case 5: _t->IdsFeedPressed(); break;
        case 6: _t->CloseApp(); break;
        case 7: _t->on_filePushButton_clicked(); break;
        case 8: _t->on_camButton_clicked(); break;
        case 9: _t->on_MovieButton_clicked(); break;
        case 10: _t->on_pushButton_2_clicked(); break;
        case 11: _t->on_AvtButton_clicked(); break;
        case 12: _t->on_vimbaButton_clicked(); break;
        case 13: _t->on_idsButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FileInputDialog::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileInputDialog::StaticPicPressed)) {
                *result = 0;
            }
        }
        {
            typedef void (FileInputDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileInputDialog::OpencvFeedPressed)) {
                *result = 1;
            }
        }
        {
            typedef void (FileInputDialog::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileInputDialog::MoviePressed)) {
                *result = 2;
            }
        }
        {
            typedef void (FileInputDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileInputDialog::AvtFeedPressed)) {
                *result = 3;
            }
        }
        {
            typedef void (FileInputDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileInputDialog::VimbaFeedPressed)) {
                *result = 4;
            }
        }
        {
            typedef void (FileInputDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileInputDialog::IdsFeedPressed)) {
                *result = 5;
            }
        }
        {
            typedef void (FileInputDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileInputDialog::CloseApp)) {
                *result = 6;
            }
        }
    }
}

const QMetaObject FileInputDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FileInputDialog.data,
      qt_meta_data_FileInputDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FileInputDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileInputDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FileInputDialog.stringdata))
        return static_cast<void*>(const_cast< FileInputDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int FileInputDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void FileInputDialog::StaticPicPressed(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FileInputDialog::OpencvFeedPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void FileInputDialog::MoviePressed(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FileInputDialog::AvtFeedPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void FileInputDialog::VimbaFeedPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void FileInputDialog::IdsFeedPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void FileInputDialog::CloseApp()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

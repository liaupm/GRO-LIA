/****************************************************************************
** Meta object code from reading C++ file 'GroThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gro-master/GroThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GroThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GroThread_t {
    QByteArrayData data[7];
    char stringdata0[63];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GroThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GroThread_t qt_meta_stringdata_GroThread = {
    {
QT_MOC_LITERAL(0, 0, 9), // "GroThread"
QT_MOC_LITERAL(1, 10, 13), // "renderedImage"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 5), // "image"
QT_MOC_LITERAL(4, 31, 11), // "stateChange"
QT_MOC_LITERAL(5, 43, 7), // "message"
QT_MOC_LITERAL(6, 51, 11) // "std::string"

    },
    "GroThread\0renderedImage\0\0image\0"
    "stateChange\0message\0std::string"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GroThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    0,   32,    2, 0x06 /* Public */,
       5,    2,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Bool,    2,    2,

       0        // eod
};

void GroThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GroThread *_t = static_cast<GroThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->renderedImage((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        case 1: _t->stateChange(); break;
        case 2: _t->message((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GroThread::*_t)(const QImage & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GroThread::renderedImage)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (GroThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GroThread::stateChange)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (GroThread::*_t)(std::string , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GroThread::message)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject GroThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_GroThread.data,
      qt_meta_data_GroThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GroThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GroThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GroThread.stringdata0))
        return static_cast<void*>(const_cast< GroThread*>(this));
    return QThread::qt_metacast(_clname);
}

int GroThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void GroThread::renderedImage(const QImage & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GroThread::stateChange()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void GroThread::message(std::string _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

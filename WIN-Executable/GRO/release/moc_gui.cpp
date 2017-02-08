/****************************************************************************
** Meta object code from reading C++ file 'gui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gro-master/gui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Gui_t {
    QByteArrayData data[17];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Gui_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Gui_t qt_meta_stringdata_Gui = {
    {
QT_MOC_LITERAL(0, 0, 3), // "Gui"
QT_MOC_LITERAL(1, 4, 9), // "startStop"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 4), // "open"
QT_MOC_LITERAL(4, 20, 4), // "step"
QT_MOC_LITERAL(5, 25, 6), // "reload"
QT_MOC_LITERAL(6, 32, 14), // "displayMessage"
QT_MOC_LITERAL(7, 47, 3), // "msg"
QT_MOC_LITERAL(8, 51, 5), // "clear"
QT_MOC_LITERAL(9, 57, 4), // "help"
QT_MOC_LITERAL(10, 62, 5), // "about"
QT_MOC_LITERAL(11, 68, 8), // "snapshot"
QT_MOC_LITERAL(12, 77, 9), // "moreCells"
QT_MOC_LITERAL(13, 87, 7), // "zoom_in"
QT_MOC_LITERAL(14, 95, 8), // "zoom_out"
QT_MOC_LITERAL(15, 104, 10), // "reset_zoom"
QT_MOC_LITERAL(16, 115, 4) // "dump"

    },
    "Gui\0startStop\0\0open\0step\0reload\0"
    "displayMessage\0msg\0clear\0help\0about\0"
    "snapshot\0moreCells\0zoom_in\0zoom_out\0"
    "reset_zoom\0dump"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Gui[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a /* Public */,
       3,    0,   85,    2, 0x0a /* Public */,
       4,    0,   86,    2, 0x0a /* Public */,
       5,    0,   87,    2, 0x0a /* Public */,
       6,    2,   88,    2, 0x0a /* Public */,
       6,    1,   93,    2, 0x2a /* Public | MethodCloned */,
       9,    0,   96,    2, 0x0a /* Public */,
      10,    0,   97,    2, 0x0a /* Public */,
      11,    0,   98,    2, 0x0a /* Public */,
      12,    0,   99,    2, 0x0a /* Public */,
      13,    0,  100,    2, 0x0a /* Public */,
      14,    0,  101,    2, 0x0a /* Public */,
      15,    0,  102,    2, 0x0a /* Public */,
      16,    0,  103,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    7,    8,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Gui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Gui *_t = static_cast<Gui *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startStop(); break;
        case 1: _t->open(); break;
        case 2: _t->step(); break;
        case 3: _t->reload(); break;
        case 4: _t->displayMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->displayMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->help(); break;
        case 7: _t->about(); break;
        case 8: _t->snapshot(); break;
        case 9: _t->moreCells(); break;
        case 10: _t->zoom_in(); break;
        case 11: _t->zoom_out(); break;
        case 12: _t->reset_zoom(); break;
        case 13: _t->dump(); break;
        default: ;
        }
    }
}

const QMetaObject Gui::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Gui.data,
      qt_meta_data_Gui,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Gui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Gui::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Gui.stringdata0))
        return static_cast<void*>(const_cast< Gui*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Gui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'cameras_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cameras_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cameras_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cameras_widget_t {
    QByteArrayData data[12];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cameras_widget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cameras_widget_t qt_meta_stringdata_cameras_widget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "cameras_widget"
QT_MOC_LITERAL(1, 15, 22), // "add_camera_user_notify"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 10), // "QAtomicInt"
QT_MOC_LITERAL(4, 50, 5), // "index"
QT_MOC_LITERAL(5, 56, 7), // "user_id"
QT_MOC_LITERAL(6, 64, 9), // "cam_index"
QT_MOC_LITERAL(7, 74, 3), // "WId"
QT_MOC_LITERAL(8, 78, 4), // "hwnd"
QT_MOC_LITERAL(9, 83, 23), // "signal_open_camera_list"
QT_MOC_LITERAL(10, 107, 4), // "open"
QT_MOC_LITERAL(11, 112, 20) // "on_btn_close_clicked"

    },
    "cameras_widget\0add_camera_user_notify\0"
    "\0QAtomicInt\0index\0user_id\0cam_index\0"
    "WId\0hwnd\0signal_open_camera_list\0open\0"
    "on_btn_close_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cameras_widget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   29,    2, 0x06 /* Public */,
       9,    1,   38,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::ULongLong, QMetaType::Int, 0x80000000 | 7,    4,    5,    6,    8,
    QMetaType::Void, QMetaType::Bool,   10,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void cameras_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        cameras_widget *_t = static_cast<cameras_widget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->add_camera_user_notify((*reinterpret_cast< const QAtomicInt(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< WId(*)>(_a[4]))); break;
        case 1: _t->signal_open_camera_list((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_btn_close_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (cameras_widget::*_t)(const QAtomicInt & , quint64 , int , WId );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cameras_widget::add_camera_user_notify)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (cameras_widget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cameras_widget::signal_open_camera_list)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject cameras_widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_cameras_widget.data,
      qt_meta_data_cameras_widget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *cameras_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cameras_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_cameras_widget.stringdata0))
        return static_cast<void*>(const_cast< cameras_widget*>(this));
    return QWidget::qt_metacast(_clname);
}

int cameras_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void cameras_widget::add_camera_user_notify(const QAtomicInt & _t1, quint64 _t2, int _t3, WId _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void cameras_widget::signal_open_camera_list(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'little_cam_wgt.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../little_cam_wgt.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'little_cam_wgt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_little_cam_wgt_t {
    QByteArrayData data[3];
    char stringdata0[28];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_little_cam_wgt_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_little_cam_wgt_t qt_meta_stringdata_little_cam_wgt = {
    {
QT_MOC_LITERAL(0, 0, 14), // "little_cam_wgt"
QT_MOC_LITERAL(1, 15, 11), // "mouse_click"
QT_MOC_LITERAL(2, 27, 0) // ""

    },
    "little_cam_wgt\0mouse_click\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_little_cam_wgt[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void little_cam_wgt::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        little_cam_wgt *_t = static_cast<little_cam_wgt *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mouse_click(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (little_cam_wgt::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&little_cam_wgt::mouse_click)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject little_cam_wgt::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_little_cam_wgt.data,
      qt_meta_data_little_cam_wgt,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *little_cam_wgt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *little_cam_wgt::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_little_cam_wgt.stringdata0))
        return static_cast<void*>(const_cast< little_cam_wgt*>(this));
    return QWidget::qt_metacast(_clname);
}

int little_cam_wgt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void little_cam_wgt::mouse_click()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_widget_t {
    QByteArrayData data[3];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_widget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_widget_t qt_meta_stringdata_widget = {
    {
QT_MOC_LITERAL(0, 0, 6), // "widget"
QT_MOC_LITERAL(1, 7, 11), // "mouse_click"
QT_MOC_LITERAL(2, 19, 0) // ""

    },
    "widget\0mouse_click\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_widget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        widget *_t = static_cast<widget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mouse_click(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (widget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&widget::mouse_click)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_widget.data,
      qt_meta_data_widget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *widget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_widget.stringdata0))
        return static_cast<void*>(const_cast< widget*>(this));
    return QWidget::qt_metacast(_clname);
}

int widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void widget::mouse_click()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

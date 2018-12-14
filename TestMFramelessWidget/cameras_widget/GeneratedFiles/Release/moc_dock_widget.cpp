/****************************************************************************
** Meta object code from reading C++ file 'dock_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../left_window/dock_widget/dock_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dock_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_dock_widget_t {
    QByteArrayData data[6];
    char stringdata0[68];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_dock_widget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_dock_widget_t qt_meta_stringdata_dock_widget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "dock_widget"
QT_MOC_LITERAL(1, 12, 11), // "is_Floating"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 7), // "is_show"
QT_MOC_LITERAL(4, 33, 16), // "on_title_clicked"
QT_MOC_LITERAL(5, 50, 17) // "on_button_clicked"

    },
    "dock_widget\0is_Floating\0\0is_show\0"
    "on_title_clicked\0on_button_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_dock_widget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   32,    2, 0x0a /* Public */,
       5,    0,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void dock_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        dock_widget *_t = static_cast<dock_widget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->is_Floating((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_title_clicked(); break;
        case 2: _t->on_button_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (dock_widget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dock_widget::is_Floating)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject dock_widget::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_dock_widget.data,
      qt_meta_data_dock_widget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *dock_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dock_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_dock_widget.stringdata0))
        return static_cast<void*>(const_cast< dock_widget*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int dock_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
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
void dock_widget::is_Floating(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_dock_title_bar_t {
    QByteArrayData data[4];
    char stringdata0[55];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_dock_title_bar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_dock_title_bar_t qt_meta_stringdata_dock_title_bar = {
    {
QT_MOC_LITERAL(0, 0, 14), // "dock_title_bar"
QT_MOC_LITERAL(1, 15, 17), // "on_button_clicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 20) // "mouse_double_clicked"

    },
    "dock_title_bar\0on_button_clicked\0\0"
    "mouse_double_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_dock_title_bar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    0,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void dock_title_bar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        dock_title_bar *_t = static_cast<dock_title_bar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_button_clicked(); break;
        case 1: _t->mouse_double_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (dock_title_bar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dock_title_bar::on_button_clicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (dock_title_bar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dock_title_bar::mouse_double_clicked)) {
                *result = 1;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject dock_title_bar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_dock_title_bar.data,
      qt_meta_data_dock_title_bar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *dock_title_bar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dock_title_bar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_dock_title_bar.stringdata0))
        return static_cast<void*>(const_cast< dock_title_bar*>(this));
    return QWidget::qt_metacast(_clname);
}

int dock_title_bar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void dock_title_bar::on_button_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void dock_title_bar::mouse_double_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 't9widget.h'
**
** Created: Wed Dec 4 18:57:50 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "t9input/t9widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 't9widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_T9Widget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   10,    9,    9, 0x05,
      33,    9,    9,    9, 0x05,
      41,    9,    9,    9, 0x05,
      53,    9,    9,    9, 0x05,
      66,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      87,   80,    9,    9, 0x0a,
     110,    9,    9,    9, 0x08,
     130,    9,    9,    9, 0x08,
     150,    9,    9,    9, 0x08,
     171,    9,    9,    9, 0x08,
     192,    9,    9,    9, 0x08,
     214,    9,    9,    9, 0x08,
     236,    9,    9,    9, 0x08,
     262,    9,    9,    9, 0x08,
     285,    9,    9,    9, 0x08,
     306,    9,    9,    9, 0x08,
     330,  327,    9,    9, 0x08,
     351,  327,    9,    9, 0x08,
     368,  327,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_T9Widget[] = {
    "T9Widget\0\0str\0newString(QString)\0"
    "enter()\0backSpace()\0update_str()\0"
    "update_str2()\0recver\0changeRecver(QWidget*)\0"
    "on_pyPgDn_clicked()\0on_pyPgUp_clicked()\0"
    "on_jingBtn_clicked()\0on_starBtn_clicked()\0"
    "on_enterBtn_clicked()\0on_spaceBtn_clicked()\0"
    "on_backSpaceBtn_clicked()\0"
    "on_methodBtn_clicked()\0on_pgDnBtn_clicked()\0"
    "on_pgUpBtn_clicked()\0id\0onNumberClicked(int)\0"
    "onPyClicked(int)\0onHzClicked(int)\0"
};

const QMetaObject T9Widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_T9Widget,
      qt_meta_data_T9Widget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &T9Widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *T9Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *T9Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_T9Widget))
        return static_cast<void*>(const_cast< T9Widget*>(this));
    return QWidget::qt_metacast(_clname);
}

int T9Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newString((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: enter(); break;
        case 2: backSpace(); break;
        case 3: update_str(); break;
        case 4: update_str2(); break;
        case 5: changeRecver((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 6: on_pyPgDn_clicked(); break;
        case 7: on_pyPgUp_clicked(); break;
        case 8: on_jingBtn_clicked(); break;
        case 9: on_starBtn_clicked(); break;
        case 10: on_enterBtn_clicked(); break;
        case 11: on_spaceBtn_clicked(); break;
        case 12: on_backSpaceBtn_clicked(); break;
        case 13: on_methodBtn_clicked(); break;
        case 14: on_pgDnBtn_clicked(); break;
        case 15: on_pgUpBtn_clicked(); break;
        case 16: onNumberClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: onPyClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: onHzClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void T9Widget::newString(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void T9Widget::enter()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void T9Widget::backSpace()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void T9Widget::update_str()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void T9Widget::update_str2()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE

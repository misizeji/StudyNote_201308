/****************************************************************************
** Meta object code from reading C++ file 'form.h'
**
** Created: Wed Dec 4 18:57:49 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "form.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'form.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Form[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x08,
      32,    5,    5,    5, 0x08,
      58,    5,    5,    5, 0x08,
      84,    5,    5,    5, 0x08,
     108,    5,    5,    5, 0x08,
     134,    5,    5,    5, 0x08,
     160,    5,    5,    5, 0x08,
     186,    5,    5,    5, 0x08,
     212,    5,    5,    5, 0x08,
     236,    5,    5,    5, 0x08,
     259,    5,    5,    5, 0x08,
     283,    5,    5,    5, 0x08,
     299,    5,    5,    5, 0x08,
     316,    5,    5,    5, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Form[] = {
    "Form\0\0on_pushButton_4_clicked()\0"
    "on_pushButton_5_clicked()\0"
    "on_pushButton_6_clicked()\0"
    "on_pushButton_clicked()\0"
    "on_pushButton_3_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_toolButton_3_clicked()\0"
    "on_toolButton_2_clicked()\0"
    "on_toolButton_clicked()\0update_string(QString)\0"
    "update_string2(QString)\0backSpaceText()\0"
    "backSpaceText2()\0timeSlot()\0"
};

const QMetaObject Form::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Form,
      qt_meta_data_Form, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Form::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Form::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Form::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Form))
        return static_cast<void*>(const_cast< Form*>(this));
    return QWidget::qt_metacast(_clname);
}

int Form::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButton_4_clicked(); break;
        case 1: on_pushButton_5_clicked(); break;
        case 2: on_pushButton_6_clicked(); break;
        case 3: on_pushButton_clicked(); break;
        case 4: on_pushButton_3_clicked(); break;
        case 5: on_pushButton_2_clicked(); break;
        case 6: on_toolButton_3_clicked(); break;
        case 7: on_toolButton_2_clicked(); break;
        case 8: on_toolButton_clicked(); break;
        case 9: update_string((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: update_string2((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: backSpaceText(); break;
        case 12: backSpaceText2(); break;
        case 13: timeSlot(); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'toolbarsearch.h'
**
** Created: Wed May 5 14:20:04 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../toolbarsearch.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'toolbarsearch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ToolbarSearch[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      32,   14,   14,   14, 0x0a,
      40,   14,   14,   14, 0x0a,
      52,   14,   14,   14, 0x08,
      59,   14,   14,   14, 0x08,
      84,   77,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ToolbarSearch[] = {
    "ToolbarSearch\0\0url\0search(QUrl)\0clear()\0"
    "searchNow()\0save()\0aboutToShowMenu()\0"
    "action\0triggeredMenuAction(QAction*)\0"
};

const QMetaObject ToolbarSearch::staticMetaObject = {
    { &SearchLineEdit::staticMetaObject, qt_meta_stringdata_ToolbarSearch,
      qt_meta_data_ToolbarSearch, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ToolbarSearch::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ToolbarSearch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ToolbarSearch::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ToolbarSearch))
        return static_cast<void*>(const_cast< ToolbarSearch*>(this));
    return SearchLineEdit::qt_metacast(_clname);
}

int ToolbarSearch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SearchLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: search((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 1: clear(); break;
        case 2: searchNow(); break;
        case 3: save(); break;
        case 4: aboutToShowMenu(); break;
        case 5: triggeredMenuAction((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ToolbarSearch::search(const QUrl & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
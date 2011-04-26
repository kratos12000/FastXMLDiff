/****************************************************************************
** Meta object code from reading C++ file 'DiffWidget.h'
**
** Created: Tue Apr 26 17:36:50 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DiffWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DiffWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DiffWidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      23,   11,   11,   11, 0x0a,
      44,   11,   11,   11, 0x08,
      57,   11,   11,   11, 0x08,
      75,   70,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DiffWidget[] = {
    "DiffWidget\0\0canceled()\0applyButtonPressed()\0"
    "getOldPath()\0getNewPath()\0text\0"
    "enableOkButton(QString)\0"
};

const QMetaObject DiffWidget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DiffWidget,
      qt_meta_data_DiffWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DiffWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DiffWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DiffWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DiffWidget))
        return static_cast<void*>(const_cast< DiffWidget*>(this));
    return QDialog::qt_metacast(_clname);
}

int DiffWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: canceled(); break;
        case 1: applyButtonPressed(); break;
        case 2: getOldPath(); break;
        case 3: getNewPath(); break;
        case 4: enableOkButton((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DiffWidget::canceled()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE

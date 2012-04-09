/****************************************************************************
** Meta object code from reading C++ file 'connector.h'
**
** Created: Mon 9. Apr 16:34:48 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../connector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Connector[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      27,   10,   10,   10, 0x05,
      44,   10,   10,   10, 0x05,
      65,   10,   10,   10, 0x05,
      79,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     106,   94,   10,   10, 0x0a,
     168,  136,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Connector[] = {
    "Connector\0\0logInFinished()\0taxonomyLoaded()\0"
    "fileUploadFinished()\0loginNeeded()\0"
    "syncComplete()\0id,responce\0"
    "processResponse(int,QVariant)\0"
    "requestId,faultCode,faultString\0"
    "failed(int,int,QString)\0"
};

const QMetaObject Connector::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Connector,
      qt_meta_data_Connector, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Connector::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Connector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Connector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Connector))
        return static_cast<void*>(const_cast< Connector*>(this));
    return QObject::qt_metacast(_clname);
}

int Connector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: logInFinished(); break;
        case 1: taxonomyLoaded(); break;
        case 2: fileUploadFinished(); break;
        case 3: loginNeeded(); break;
        case 4: syncComplete(); break;
        case 5: processResponse((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 6: failed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Connector::logInFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Connector::taxonomyLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Connector::fileUploadFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Connector::loginNeeded()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Connector::syncComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE

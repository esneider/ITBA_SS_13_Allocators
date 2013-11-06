/****************************************************************************
** Meta object code from reading C++ file 'simulationplot.h'
**
** Created: Wed Nov 6 18:10:22 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../simulationplot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simulationplot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SimulationPlot[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      33,   30,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      46,   15,   15,   15, 0x0a,
      54,   15,   15,   15, 0x0a,
      64,   61,   15,   15, 0x0a,
      79,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SimulationPlot[] = {
    "SimulationPlot\0\0running(bool)\0ms\0"
    "elapsed(int)\0clear()\0stop()\0ts\0"
    "append(double)\0appendPoint()\0"
};

void SimulationPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SimulationPlot *_t = static_cast<SimulationPlot *>(_o);
        switch (_id) {
        case 0: _t->running((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->elapsed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->clear(); break;
        case 3: _t->stop(); break;
        case 4: _t->append((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->appendPoint(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SimulationPlot::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SimulationPlot::staticMetaObject = {
    { &IncrementalPlot::staticMetaObject, qt_meta_stringdata_SimulationPlot,
      qt_meta_data_SimulationPlot, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SimulationPlot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SimulationPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SimulationPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimulationPlot))
        return static_cast<void*>(const_cast< SimulationPlot*>(this));
    return IncrementalPlot::qt_metacast(_clname);
}

int SimulationPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IncrementalPlot::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SimulationPlot::running(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SimulationPlot::elapsed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

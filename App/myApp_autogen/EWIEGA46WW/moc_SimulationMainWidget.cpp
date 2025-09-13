/****************************************************************************
** Meta object code from reading C++ file 'SimulationMainWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../SimulationMainWidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SimulationMainWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN20SimulationMainWidgetE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN20SimulationMainWidgetE = QtMocHelpers::stringData(
    "SimulationMainWidget",
    "clockChanged",
    "",
    "newValue",
    "stagnationChanged",
    "waitingTimeChanged",
    "taskAddedToWaitingPool",
    "const Task*",
    "task",
    "taskAddedToSendingPool",
    "const SimulationEnvironment::SendingTask*",
    "sTask",
    "taskAddedToRunningPool",
    "allPoolsCleared",
    "updateClock",
    "updateStagnation",
    "updateWaitingTime",
    "addTaskToWaitingPool",
    "addTaskToSendingPool",
    "addTaskToRunningPool",
    "clearAllPools"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN20SimulationMainWidgetE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   98,    2, 0x06,    1 /* Public */,
       4,    1,  101,    2, 0x06,    3 /* Public */,
       5,    1,  104,    2, 0x06,    5 /* Public */,
       6,    1,  107,    2, 0x06,    7 /* Public */,
       9,    1,  110,    2, 0x06,    9 /* Public */,
      12,    1,  113,    2, 0x06,   11 /* Public */,
      13,    0,  116,    2, 0x06,   13 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      14,    1,  117,    2, 0x0a,   14 /* Public */,
      15,    1,  120,    2, 0x0a,   16 /* Public */,
      16,    1,  123,    2, 0x0a,   18 /* Public */,
      17,    1,  126,    2, 0x0a,   20 /* Public */,
      18,    1,  129,    2, 0x0a,   22 /* Public */,
      19,    1,  132,    2, 0x0a,   24 /* Public */,
      20,    0,  135,    2, 0x0a,   26 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SimulationMainWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN20SimulationMainWidgetE.offsetsAndSizes,
    qt_meta_data_ZN20SimulationMainWidgetE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN20SimulationMainWidgetE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SimulationMainWidget, std::true_type>,
        // method 'clockChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'stagnationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'waitingTimeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'taskAddedToWaitingPool'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Task *, std::false_type>,
        // method 'taskAddedToSendingPool'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const SimulationEnvironment::SendingTask *, std::false_type>,
        // method 'taskAddedToRunningPool'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Task *, std::false_type>,
        // method 'allPoolsCleared'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateClock'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateStagnation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'updateWaitingTime'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'addTaskToWaitingPool'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Task *, std::false_type>,
        // method 'addTaskToSendingPool'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const SimulationEnvironment::SendingTask *, std::false_type>,
        // method 'addTaskToRunningPool'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Task *, std::false_type>,
        // method 'clearAllPools'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SimulationMainWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SimulationMainWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->clockChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->stagnationChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 2: _t->waitingTimeChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 3: _t->taskAddedToWaitingPool((*reinterpret_cast< std::add_pointer_t<const Task*>>(_a[1]))); break;
        case 4: _t->taskAddedToSendingPool((*reinterpret_cast< std::add_pointer_t<const SimulationEnvironment::SendingTask*>>(_a[1]))); break;
        case 5: _t->taskAddedToRunningPool((*reinterpret_cast< std::add_pointer_t<const Task*>>(_a[1]))); break;
        case 6: _t->allPoolsCleared(); break;
        case 7: _t->updateClock((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->updateStagnation((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 9: _t->updateWaitingTime((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 10: _t->addTaskToWaitingPool((*reinterpret_cast< std::add_pointer_t<const Task*>>(_a[1]))); break;
        case 11: _t->addTaskToSendingPool((*reinterpret_cast< std::add_pointer_t<const SimulationEnvironment::SendingTask*>>(_a[1]))); break;
        case 12: _t->addTaskToRunningPool((*reinterpret_cast< std::add_pointer_t<const Task*>>(_a[1]))); break;
        case 13: _t->clearAllPools(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (SimulationMainWidget::*)(int );
            if (_q_method_type _q_method = &SimulationMainWidget::clockChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (SimulationMainWidget::*)(float );
            if (_q_method_type _q_method = &SimulationMainWidget::stagnationChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (SimulationMainWidget::*)(float );
            if (_q_method_type _q_method = &SimulationMainWidget::waitingTimeChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (SimulationMainWidget::*)(const Task * );
            if (_q_method_type _q_method = &SimulationMainWidget::taskAddedToWaitingPool; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (SimulationMainWidget::*)(const SimulationEnvironment::SendingTask * );
            if (_q_method_type _q_method = &SimulationMainWidget::taskAddedToSendingPool; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (SimulationMainWidget::*)(const Task * );
            if (_q_method_type _q_method = &SimulationMainWidget::taskAddedToRunningPool; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (SimulationMainWidget::*)();
            if (_q_method_type _q_method = &SimulationMainWidget::allPoolsCleared; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject *SimulationMainWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SimulationMainWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN20SimulationMainWidgetE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SimulationMainWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void SimulationMainWidget::clockChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SimulationMainWidget::stagnationChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SimulationMainWidget::waitingTimeChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SimulationMainWidget::taskAddedToWaitingPool(const Task * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SimulationMainWidget::taskAddedToSendingPool(const SimulationEnvironment::SendingTask * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SimulationMainWidget::taskAddedToRunningPool(const Task * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SimulationMainWidget::allPoolsCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP

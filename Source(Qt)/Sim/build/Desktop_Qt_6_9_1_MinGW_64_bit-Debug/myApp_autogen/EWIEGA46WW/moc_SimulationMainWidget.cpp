/****************************************************************************
** Meta object code from reading C++ file 'SimulationMainWidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
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
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
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

template <> constexpr inline auto SimulationMainWidget::qt_create_metaobjectdata<qt_meta_tag_ZN20SimulationMainWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
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
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'clockChanged'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'stagnationChanged'
        QtMocHelpers::SignalData<void(float)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 3 },
        }}),
        // Signal 'waitingTimeChanged'
        QtMocHelpers::SignalData<void(float)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 3 },
        }}),
        // Signal 'taskAddedToWaitingPool'
        QtMocHelpers::SignalData<void(const Task *)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Signal 'taskAddedToSendingPool'
        QtMocHelpers::SignalData<void(const SimulationEnvironment::SendingTask *)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Signal 'taskAddedToRunningPool'
        QtMocHelpers::SignalData<void(const Task *)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Signal 'allPoolsCleared'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateClock'
        QtMocHelpers::SlotData<void(int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'updateStagnation'
        QtMocHelpers::SlotData<void(float)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 3 },
        }}),
        // Slot 'updateWaitingTime'
        QtMocHelpers::SlotData<void(float)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 3 },
        }}),
        // Slot 'addTaskToWaitingPool'
        QtMocHelpers::SlotData<void(const Task *)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'addTaskToSendingPool'
        QtMocHelpers::SlotData<void(const SimulationEnvironment::SendingTask *)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Slot 'addTaskToRunningPool'
        QtMocHelpers::SlotData<void(const Task *)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'clearAllPools'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SimulationMainWidget, qt_meta_tag_ZN20SimulationMainWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SimulationMainWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20SimulationMainWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20SimulationMainWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20SimulationMainWidgetE_t>.metaTypes,
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
        if (QtMocHelpers::indexOfMethod<void (SimulationMainWidget::*)(int )>(_a, &SimulationMainWidget::clockChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (SimulationMainWidget::*)(float )>(_a, &SimulationMainWidget::stagnationChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (SimulationMainWidget::*)(float )>(_a, &SimulationMainWidget::waitingTimeChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (SimulationMainWidget::*)(const Task * )>(_a, &SimulationMainWidget::taskAddedToWaitingPool, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (SimulationMainWidget::*)(const SimulationEnvironment::SendingTask * )>(_a, &SimulationMainWidget::taskAddedToSendingPool, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (SimulationMainWidget::*)(const Task * )>(_a, &SimulationMainWidget::taskAddedToRunningPool, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (SimulationMainWidget::*)()>(_a, &SimulationMainWidget::allPoolsCleared, 6))
            return;
    }
}

const QMetaObject *SimulationMainWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SimulationMainWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20SimulationMainWidgetE_t>.strings))
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
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void SimulationMainWidget::stagnationChanged(float _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void SimulationMainWidget::waitingTimeChanged(float _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void SimulationMainWidget::taskAddedToWaitingPool(const Task * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void SimulationMainWidget::taskAddedToSendingPool(const SimulationEnvironment::SendingTask * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void SimulationMainWidget::taskAddedToRunningPool(const Task * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void SimulationMainWidget::allPoolsCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP

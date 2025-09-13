/****************************************************************************
** Meta object code from reading C++ file 'ResourcesDockWidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../ResourcesDockWidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ResourcesDockWidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN19ResourcesDockWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto ResourcesDockWidget::qt_create_metaobjectdata<qt_meta_tag_ZN19ResourcesDockWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ResourcesDockWidget",
        "resAddedToResPannel",
        "",
        "const Resource*",
        "res",
        "taskAddedToResPannel",
        "const Task*",
        "task",
        "resTreeCleared",
        "addResToResPool",
        "addTaskToResPool",
        "clearResTree"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'resAddedToResPannel'
        QtMocHelpers::SignalData<void(const Resource *)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'taskAddedToResPannel'
        QtMocHelpers::SignalData<void(const Task *)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Signal 'resTreeCleared'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'addResToResPool'
        QtMocHelpers::SlotData<void(const Resource *)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'addTaskToResPool'
        QtMocHelpers::SlotData<void(const Task *)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'clearResTree'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ResourcesDockWidget, qt_meta_tag_ZN19ResourcesDockWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ResourcesDockWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19ResourcesDockWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19ResourcesDockWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19ResourcesDockWidgetE_t>.metaTypes,
    nullptr
} };

void ResourcesDockWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ResourcesDockWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->resAddedToResPannel((*reinterpret_cast< std::add_pointer_t<const Resource*>>(_a[1]))); break;
        case 1: _t->taskAddedToResPannel((*reinterpret_cast< std::add_pointer_t<const Task*>>(_a[1]))); break;
        case 2: _t->resTreeCleared(); break;
        case 3: _t->addResToResPool((*reinterpret_cast< std::add_pointer_t<const Resource*>>(_a[1]))); break;
        case 4: _t->addTaskToResPool((*reinterpret_cast< std::add_pointer_t<const Task*>>(_a[1]))); break;
        case 5: _t->clearResTree(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ResourcesDockWidget::*)(const Resource * )>(_a, &ResourcesDockWidget::resAddedToResPannel, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ResourcesDockWidget::*)(const Task * )>(_a, &ResourcesDockWidget::taskAddedToResPannel, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ResourcesDockWidget::*)()>(_a, &ResourcesDockWidget::resTreeCleared, 2))
            return;
    }
}

const QMetaObject *ResourcesDockWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ResourcesDockWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19ResourcesDockWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ResourcesDockWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ResourcesDockWidget::resAddedToResPannel(const Resource * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ResourcesDockWidget::taskAddedToResPannel(const Task * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void ResourcesDockWidget::resTreeCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP

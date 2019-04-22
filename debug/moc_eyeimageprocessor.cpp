/****************************************************************************
** Meta object code from reading C++ file 'eyeimageprocessor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../EyeAiming/eyeimageprocessor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eyeimageprocessor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EyeImageProcessor_t {
    QByteArrayData data[14];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EyeImageProcessor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EyeImageProcessor_t qt_meta_stringdata_EyeImageProcessor = {
    {
QT_MOC_LITERAL(0, 0, 17), // "EyeImageProcessor"
QT_MOC_LITERAL(1, 18, 7), // "newData"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 7), // "EyeData"
QT_MOC_LITERAL(4, 35, 4), // "data"
QT_MOC_LITERAL(5, 40, 7), // "process"
QT_MOC_LITERAL(6, 48, 9), // "Timestamp"
QT_MOC_LITERAL(7, 58, 1), // "t"
QT_MOC_LITERAL(8, 60, 7), // "cv::Mat"
QT_MOC_LITERAL(9, 68, 5), // "frame"
QT_MOC_LITERAL(10, 74, 12), // "updateConfig"
QT_MOC_LITERAL(11, 87, 6), // "newROI"
QT_MOC_LITERAL(12, 94, 4), // "sROI"
QT_MOC_LITERAL(13, 99, 4) // "eROI"

    },
    "EyeImageProcessor\0newData\0\0EyeData\0"
    "data\0process\0Timestamp\0t\0cv::Mat\0frame\0"
    "updateConfig\0newROI\0sROI\0eROI"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EyeImageProcessor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   37,    2, 0x0a /* Public */,
      10,    0,   42,    2, 0x0a /* Public */,
      11,    2,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 8,    7,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPointF, QMetaType::QPointF,   12,   13,

       0        // eod
};

void EyeImageProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EyeImageProcessor *_t = static_cast<EyeImageProcessor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newData((*reinterpret_cast< EyeData(*)>(_a[1]))); break;
        case 1: _t->process((*reinterpret_cast< Timestamp(*)>(_a[1])),(*reinterpret_cast< const cv::Mat(*)>(_a[2]))); break;
        case 2: _t->updateConfig(); break;
        case 3: _t->newROI((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< EyeData >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Timestamp >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< cv::Mat >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (EyeImageProcessor::*)(EyeData );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EyeImageProcessor::newData)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject EyeImageProcessor::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_EyeImageProcessor.data,
    qt_meta_data_EyeImageProcessor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *EyeImageProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EyeImageProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EyeImageProcessor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int EyeImageProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void EyeImageProcessor::newData(EyeData _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'mainUI.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../mainUI.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_mainUI_t {
    QByteArrayData data[82];
    char stringdata0[1934];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mainUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mainUI_t qt_meta_stringdata_mainUI = {
    {
QT_MOC_LITERAL(0, 0, 6), // "mainUI"
QT_MOC_LITERAL(1, 7, 26), // "on_portRefreshBtn_released"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 25), // "on_openSerialBtn_released"
QT_MOC_LITERAL(4, 61, 19), // "on_beginBtn_clicked"
QT_MOC_LITERAL(5, 81, 16), // "SerialTimerStart"
QT_MOC_LITERAL(6, 98, 8), // "ReadData"
QT_MOC_LITERAL(7, 107, 11), // "DataProcess"
QT_MOC_LITERAL(8, 119, 8), // "uint8_t*"
QT_MOC_LITERAL(9, 128, 4), // "data"
QT_MOC_LITERAL(10, 133, 10), // "UpdateData"
QT_MOC_LITERAL(11, 144, 22), // "on_saveDataBtn_clicked"
QT_MOC_LITERAL(12, 167, 8), // "DrawLine"
QT_MOC_LITERAL(13, 176, 26), // "on_bodyPoseZeroBtn_clicked"
QT_MOC_LITERAL(14, 203, 25), // "on_endPoseZeroBtn_clicked"
QT_MOC_LITERAL(15, 229, 19), // "on_SModeBtn_pressed"
QT_MOC_LITERAL(16, 249, 19), // "on_TModeBtn_pressed"
QT_MOC_LITERAL(17, 269, 22), // "on_DragModeBtn_pressed"
QT_MOC_LITERAL(18, 292, 20), // "on_SCModeBtn_pressed"
QT_MOC_LITERAL(19, 313, 21), // "on_scStartBtn_clicked"
QT_MOC_LITERAL(20, 335, 20), // "on_scStopBtn_clicked"
QT_MOC_LITERAL(21, 356, 24), // "on_scNextStepBtn_clicked"
QT_MOC_LITERAL(22, 381, 24), // "on_scSaveDataBtn_clicked"
QT_MOC_LITERAL(23, 406, 25), // "on_scClearDataBtn_clicked"
QT_MOC_LITERAL(24, 432, 25), // "on_scRecordPosBtn_clicked"
QT_MOC_LITERAL(25, 458, 27), // "on_dragModeStartBtn_pressed"
QT_MOC_LITERAL(26, 486, 26), // "on_dragModeStopBtn_clicked"
QT_MOC_LITERAL(27, 513, 28), // "on_dragRecordPos1Btn_clicked"
QT_MOC_LITERAL(28, 542, 28), // "on_dragRecordPos2Btn_clicked"
QT_MOC_LITERAL(29, 571, 23), // "on_dragClearBtn_clicked"
QT_MOC_LITERAL(30, 595, 26), // "on_dragSaveDataBtn_clicked"
QT_MOC_LITERAL(31, 622, 29), // "on_MeasureRopeModeBtn_pressed"
QT_MOC_LITERAL(32, 652, 35), // "on_measureRopeLengthStopBtn_c..."
QT_MOC_LITERAL(33, 688, 34), // "on_tightRopeBtn_Measure_RF_cl..."
QT_MOC_LITERAL(34, 723, 34), // "on_tightRopeBtn_Measure_LF_cl..."
QT_MOC_LITERAL(35, 758, 34), // "on_tightRopeBtn_Measure_LB_cl..."
QT_MOC_LITERAL(36, 793, 34), // "on_tightRopeBtn_Measure_RB_cl..."
QT_MOC_LITERAL(37, 828, 30), // "on_recordInitialBtn_RF_clicked"
QT_MOC_LITERAL(38, 859, 30), // "on_recordInitialBtn_LF_clicked"
QT_MOC_LITERAL(39, 890, 30), // "on_recordInitialBtn_LB_clicked"
QT_MOC_LITERAL(40, 921, 30), // "on_recordInitialBtn_RB_clicked"
QT_MOC_LITERAL(41, 952, 30), // "on_recordCurrentBtn_RF_clicked"
QT_MOC_LITERAL(42, 983, 30), // "on_recordCurrentBtn_LF_clicked"
QT_MOC_LITERAL(43, 1014, 30), // "on_recordCurrentBtn_LB_clicked"
QT_MOC_LITERAL(44, 1045, 30), // "on_recordCurrentBtn_RB_clicked"
QT_MOC_LITERAL(45, 1076, 24), // "on_exportDataBtn_clicked"
QT_MOC_LITERAL(46, 1101, 23), // "on_torqueBtn_RF_clicked"
QT_MOC_LITERAL(47, 1125, 23), // "on_torqueBtn_LF_clicked"
QT_MOC_LITERAL(48, 1149, 23), // "on_torqueBtn_LB_clicked"
QT_MOC_LITERAL(49, 1173, 23), // "on_torqueBtn_RB_clicked"
QT_MOC_LITERAL(50, 1197, 26), // "on_tightRopeBtn_RF_pressed"
QT_MOC_LITERAL(51, 1224, 27), // "on_tightRopeBtn_RF_released"
QT_MOC_LITERAL(52, 1252, 26), // "on_looseRopeBtn_RF_pressed"
QT_MOC_LITERAL(53, 1279, 27), // "on_looseRopeBtn_RF_released"
QT_MOC_LITERAL(54, 1307, 26), // "on_tightRopeBtn_LF_pressed"
QT_MOC_LITERAL(55, 1334, 27), // "on_tightRopeBtn_LF_released"
QT_MOC_LITERAL(56, 1362, 26), // "on_looseRopeBtn_LF_pressed"
QT_MOC_LITERAL(57, 1389, 27), // "on_looseRopeBtn_LF_released"
QT_MOC_LITERAL(58, 1417, 26), // "on_tightRopeBtn_LB_pressed"
QT_MOC_LITERAL(59, 1444, 27), // "on_tightRopeBtn_LB_released"
QT_MOC_LITERAL(60, 1472, 26), // "on_looseRopeBtn_LB_pressed"
QT_MOC_LITERAL(61, 1499, 27), // "on_looseRopeBtn_LB_released"
QT_MOC_LITERAL(62, 1527, 26), // "on_tightRopeBtn_RB_pressed"
QT_MOC_LITERAL(63, 1554, 27), // "on_tightRopeBtn_RB_released"
QT_MOC_LITERAL(64, 1582, 26), // "on_looseRopeBtn_RB_pressed"
QT_MOC_LITERAL(65, 1609, 27), // "on_looseRopeBtn_RB_released"
QT_MOC_LITERAL(66, 1637, 24), // "on_tensionTrjBtn_clicked"
QT_MOC_LITERAL(67, 1662, 26), // "on_cableTensionBtn_clicked"
QT_MOC_LITERAL(68, 1689, 21), // "on_trjStopBtn_clicked"
QT_MOC_LITERAL(69, 1711, 22), // "on_trjRisejBtn_clicked"
QT_MOC_LITERAL(70, 1734, 22), // "on_trjLinejBtn_clicked"
QT_MOC_LITERAL(71, 1757, 22), // "on_trjPolyjBtn_clicked"
QT_MOC_LITERAL(72, 1780, 23), // "on_trjCircleBtn_clicked"
QT_MOC_LITERAL(73, 1804, 21), // "on_trjFallBtn_clicked"
QT_MOC_LITERAL(74, 1826, 18), // "on_grabBtn_clicked"
QT_MOC_LITERAL(75, 1845, 19), // "on_looseBtn_clicked"
QT_MOC_LITERAL(76, 1865, 12), // "onOpenCamera"
QT_MOC_LITERAL(77, 1878, 14), // "onCaptureImage"
QT_MOC_LITERAL(78, 1893, 13), // "onCloseCamera"
QT_MOC_LITERAL(79, 1907, 15), // "onImageCaptured"
QT_MOC_LITERAL(80, 1923, 2), // "id"
QT_MOC_LITERAL(81, 1926, 7) // "preview"

    },
    "mainUI\0on_portRefreshBtn_released\0\0"
    "on_openSerialBtn_released\0on_beginBtn_clicked\0"
    "SerialTimerStart\0ReadData\0DataProcess\0"
    "uint8_t*\0data\0UpdateData\0"
    "on_saveDataBtn_clicked\0DrawLine\0"
    "on_bodyPoseZeroBtn_clicked\0"
    "on_endPoseZeroBtn_clicked\0on_SModeBtn_pressed\0"
    "on_TModeBtn_pressed\0on_DragModeBtn_pressed\0"
    "on_SCModeBtn_pressed\0on_scStartBtn_clicked\0"
    "on_scStopBtn_clicked\0on_scNextStepBtn_clicked\0"
    "on_scSaveDataBtn_clicked\0"
    "on_scClearDataBtn_clicked\0"
    "on_scRecordPosBtn_clicked\0"
    "on_dragModeStartBtn_pressed\0"
    "on_dragModeStopBtn_clicked\0"
    "on_dragRecordPos1Btn_clicked\0"
    "on_dragRecordPos2Btn_clicked\0"
    "on_dragClearBtn_clicked\0"
    "on_dragSaveDataBtn_clicked\0"
    "on_MeasureRopeModeBtn_pressed\0"
    "on_measureRopeLengthStopBtn_clicked\0"
    "on_tightRopeBtn_Measure_RF_clicked\0"
    "on_tightRopeBtn_Measure_LF_clicked\0"
    "on_tightRopeBtn_Measure_LB_clicked\0"
    "on_tightRopeBtn_Measure_RB_clicked\0"
    "on_recordInitialBtn_RF_clicked\0"
    "on_recordInitialBtn_LF_clicked\0"
    "on_recordInitialBtn_LB_clicked\0"
    "on_recordInitialBtn_RB_clicked\0"
    "on_recordCurrentBtn_RF_clicked\0"
    "on_recordCurrentBtn_LF_clicked\0"
    "on_recordCurrentBtn_LB_clicked\0"
    "on_recordCurrentBtn_RB_clicked\0"
    "on_exportDataBtn_clicked\0"
    "on_torqueBtn_RF_clicked\0on_torqueBtn_LF_clicked\0"
    "on_torqueBtn_LB_clicked\0on_torqueBtn_RB_clicked\0"
    "on_tightRopeBtn_RF_pressed\0"
    "on_tightRopeBtn_RF_released\0"
    "on_looseRopeBtn_RF_pressed\0"
    "on_looseRopeBtn_RF_released\0"
    "on_tightRopeBtn_LF_pressed\0"
    "on_tightRopeBtn_LF_released\0"
    "on_looseRopeBtn_LF_pressed\0"
    "on_looseRopeBtn_LF_released\0"
    "on_tightRopeBtn_LB_pressed\0"
    "on_tightRopeBtn_LB_released\0"
    "on_looseRopeBtn_LB_pressed\0"
    "on_looseRopeBtn_LB_released\0"
    "on_tightRopeBtn_RB_pressed\0"
    "on_tightRopeBtn_RB_released\0"
    "on_looseRopeBtn_RB_pressed\0"
    "on_looseRopeBtn_RB_released\0"
    "on_tensionTrjBtn_clicked\0"
    "on_cableTensionBtn_clicked\0"
    "on_trjStopBtn_clicked\0on_trjRisejBtn_clicked\0"
    "on_trjLinejBtn_clicked\0on_trjPolyjBtn_clicked\0"
    "on_trjCircleBtn_clicked\0on_trjFallBtn_clicked\0"
    "on_grabBtn_clicked\0on_looseBtn_clicked\0"
    "onOpenCamera\0onCaptureImage\0onCloseCamera\0"
    "onImageCaptured\0id\0preview"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mainUI[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      76,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  394,    2, 0x08 /* Private */,
       3,    0,  395,    2, 0x08 /* Private */,
       4,    0,  396,    2, 0x08 /* Private */,
       5,    0,  397,    2, 0x08 /* Private */,
       6,    0,  398,    2, 0x08 /* Private */,
       7,    1,  399,    2, 0x08 /* Private */,
      10,    0,  402,    2, 0x08 /* Private */,
      11,    0,  403,    2, 0x08 /* Private */,
      12,    0,  404,    2, 0x08 /* Private */,
      13,    0,  405,    2, 0x08 /* Private */,
      14,    0,  406,    2, 0x08 /* Private */,
      15,    0,  407,    2, 0x08 /* Private */,
      16,    0,  408,    2, 0x08 /* Private */,
      17,    0,  409,    2, 0x08 /* Private */,
      18,    0,  410,    2, 0x08 /* Private */,
      19,    0,  411,    2, 0x08 /* Private */,
      20,    0,  412,    2, 0x08 /* Private */,
      21,    0,  413,    2, 0x08 /* Private */,
      22,    0,  414,    2, 0x08 /* Private */,
      23,    0,  415,    2, 0x08 /* Private */,
      24,    0,  416,    2, 0x08 /* Private */,
      25,    0,  417,    2, 0x08 /* Private */,
      26,    0,  418,    2, 0x08 /* Private */,
      27,    0,  419,    2, 0x08 /* Private */,
      28,    0,  420,    2, 0x08 /* Private */,
      29,    0,  421,    2, 0x08 /* Private */,
      30,    0,  422,    2, 0x08 /* Private */,
      31,    0,  423,    2, 0x08 /* Private */,
      32,    0,  424,    2, 0x08 /* Private */,
      33,    0,  425,    2, 0x08 /* Private */,
      34,    0,  426,    2, 0x08 /* Private */,
      35,    0,  427,    2, 0x08 /* Private */,
      36,    0,  428,    2, 0x08 /* Private */,
      37,    0,  429,    2, 0x08 /* Private */,
      38,    0,  430,    2, 0x08 /* Private */,
      39,    0,  431,    2, 0x08 /* Private */,
      40,    0,  432,    2, 0x08 /* Private */,
      41,    0,  433,    2, 0x08 /* Private */,
      42,    0,  434,    2, 0x08 /* Private */,
      43,    0,  435,    2, 0x08 /* Private */,
      44,    0,  436,    2, 0x08 /* Private */,
      45,    0,  437,    2, 0x08 /* Private */,
      46,    0,  438,    2, 0x08 /* Private */,
      47,    0,  439,    2, 0x08 /* Private */,
      48,    0,  440,    2, 0x08 /* Private */,
      49,    0,  441,    2, 0x08 /* Private */,
      50,    0,  442,    2, 0x08 /* Private */,
      51,    0,  443,    2, 0x08 /* Private */,
      52,    0,  444,    2, 0x08 /* Private */,
      53,    0,  445,    2, 0x08 /* Private */,
      54,    0,  446,    2, 0x08 /* Private */,
      55,    0,  447,    2, 0x08 /* Private */,
      56,    0,  448,    2, 0x08 /* Private */,
      57,    0,  449,    2, 0x08 /* Private */,
      58,    0,  450,    2, 0x08 /* Private */,
      59,    0,  451,    2, 0x08 /* Private */,
      60,    0,  452,    2, 0x08 /* Private */,
      61,    0,  453,    2, 0x08 /* Private */,
      62,    0,  454,    2, 0x08 /* Private */,
      63,    0,  455,    2, 0x08 /* Private */,
      64,    0,  456,    2, 0x08 /* Private */,
      65,    0,  457,    2, 0x08 /* Private */,
      66,    0,  458,    2, 0x08 /* Private */,
      67,    0,  459,    2, 0x08 /* Private */,
      68,    0,  460,    2, 0x08 /* Private */,
      69,    0,  461,    2, 0x08 /* Private */,
      70,    0,  462,    2, 0x08 /* Private */,
      71,    0,  463,    2, 0x08 /* Private */,
      72,    0,  464,    2, 0x08 /* Private */,
      73,    0,  465,    2, 0x08 /* Private */,
      74,    0,  466,    2, 0x08 /* Private */,
      75,    0,  467,    2, 0x08 /* Private */,
      76,    0,  468,    2, 0x08 /* Private */,
      77,    0,  469,    2, 0x08 /* Private */,
      78,    0,  470,    2, 0x08 /* Private */,
      79,    2,  471,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QImage,   80,   81,

       0        // eod
};

void mainUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<mainUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_portRefreshBtn_released(); break;
        case 1: _t->on_openSerialBtn_released(); break;
        case 2: _t->on_beginBtn_clicked(); break;
        case 3: _t->SerialTimerStart(); break;
        case 4: _t->ReadData(); break;
        case 5: _t->DataProcess((*reinterpret_cast< uint8_t*(*)>(_a[1]))); break;
        case 6: _t->UpdateData(); break;
        case 7: _t->on_saveDataBtn_clicked(); break;
        case 8: _t->DrawLine(); break;
        case 9: _t->on_bodyPoseZeroBtn_clicked(); break;
        case 10: _t->on_endPoseZeroBtn_clicked(); break;
        case 11: _t->on_SModeBtn_pressed(); break;
        case 12: _t->on_TModeBtn_pressed(); break;
        case 13: _t->on_DragModeBtn_pressed(); break;
        case 14: _t->on_SCModeBtn_pressed(); break;
        case 15: _t->on_scStartBtn_clicked(); break;
        case 16: _t->on_scStopBtn_clicked(); break;
        case 17: _t->on_scNextStepBtn_clicked(); break;
        case 18: _t->on_scSaveDataBtn_clicked(); break;
        case 19: _t->on_scClearDataBtn_clicked(); break;
        case 20: _t->on_scRecordPosBtn_clicked(); break;
        case 21: _t->on_dragModeStartBtn_pressed(); break;
        case 22: _t->on_dragModeStopBtn_clicked(); break;
        case 23: _t->on_dragRecordPos1Btn_clicked(); break;
        case 24: _t->on_dragRecordPos2Btn_clicked(); break;
        case 25: _t->on_dragClearBtn_clicked(); break;
        case 26: _t->on_dragSaveDataBtn_clicked(); break;
        case 27: _t->on_MeasureRopeModeBtn_pressed(); break;
        case 28: _t->on_measureRopeLengthStopBtn_clicked(); break;
        case 29: _t->on_tightRopeBtn_Measure_RF_clicked(); break;
        case 30: _t->on_tightRopeBtn_Measure_LF_clicked(); break;
        case 31: _t->on_tightRopeBtn_Measure_LB_clicked(); break;
        case 32: _t->on_tightRopeBtn_Measure_RB_clicked(); break;
        case 33: _t->on_recordInitialBtn_RF_clicked(); break;
        case 34: _t->on_recordInitialBtn_LF_clicked(); break;
        case 35: _t->on_recordInitialBtn_LB_clicked(); break;
        case 36: _t->on_recordInitialBtn_RB_clicked(); break;
        case 37: _t->on_recordCurrentBtn_RF_clicked(); break;
        case 38: _t->on_recordCurrentBtn_LF_clicked(); break;
        case 39: _t->on_recordCurrentBtn_LB_clicked(); break;
        case 40: _t->on_recordCurrentBtn_RB_clicked(); break;
        case 41: _t->on_exportDataBtn_clicked(); break;
        case 42: _t->on_torqueBtn_RF_clicked(); break;
        case 43: _t->on_torqueBtn_LF_clicked(); break;
        case 44: _t->on_torqueBtn_LB_clicked(); break;
        case 45: _t->on_torqueBtn_RB_clicked(); break;
        case 46: _t->on_tightRopeBtn_RF_pressed(); break;
        case 47: _t->on_tightRopeBtn_RF_released(); break;
        case 48: _t->on_looseRopeBtn_RF_pressed(); break;
        case 49: _t->on_looseRopeBtn_RF_released(); break;
        case 50: _t->on_tightRopeBtn_LF_pressed(); break;
        case 51: _t->on_tightRopeBtn_LF_released(); break;
        case 52: _t->on_looseRopeBtn_LF_pressed(); break;
        case 53: _t->on_looseRopeBtn_LF_released(); break;
        case 54: _t->on_tightRopeBtn_LB_pressed(); break;
        case 55: _t->on_tightRopeBtn_LB_released(); break;
        case 56: _t->on_looseRopeBtn_LB_pressed(); break;
        case 57: _t->on_looseRopeBtn_LB_released(); break;
        case 58: _t->on_tightRopeBtn_RB_pressed(); break;
        case 59: _t->on_tightRopeBtn_RB_released(); break;
        case 60: _t->on_looseRopeBtn_RB_pressed(); break;
        case 61: _t->on_looseRopeBtn_RB_released(); break;
        case 62: _t->on_tensionTrjBtn_clicked(); break;
        case 63: _t->on_cableTensionBtn_clicked(); break;
        case 64: _t->on_trjStopBtn_clicked(); break;
        case 65: _t->on_trjRisejBtn_clicked(); break;
        case 66: _t->on_trjLinejBtn_clicked(); break;
        case 67: _t->on_trjPolyjBtn_clicked(); break;
        case 68: _t->on_trjCircleBtn_clicked(); break;
        case 69: _t->on_trjFallBtn_clicked(); break;
        case 70: _t->on_grabBtn_clicked(); break;
        case 71: _t->on_looseBtn_clicked(); break;
        case 72: _t->onOpenCamera(); break;
        case 73: _t->onCaptureImage(); break;
        case 74: _t->onCloseCamera(); break;
        case 75: _t->onImageCaptured((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QImage(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject mainUI::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_mainUI.data,
    qt_meta_data_mainUI,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *mainUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mainUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_mainUI.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int mainUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 76)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 76;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 76)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 76;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

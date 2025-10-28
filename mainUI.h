#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <vector>
#include <QMessageBox>

#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QValueAxis>

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class mainUI; }
QT_END_NAMESPACE

// 图像坐标轴的最大显示范围
#define AXIS_MAX_X 500
#define AXIS_MAX_Y 5


class mainUI : public QWidget
{
    Q_OBJECT

public:
    mainUI(QWidget *parent = nullptr);
    ~mainUI();

    QTimer *SerialTimer;   //用于串口接收数据的定时器

    QChart *poseXChart;    //用于显示X轴姿态的图像
    QTimer *drawLineTimer; //用于画图的定时器
    QSplineSeries *poseXLineSeries; //用于绘制X轴姿态的曲线

    // 自标定模式相关成员变量
    QTimer *scTimer;       //自标定定时器
    int scCurrentCycle;    //当前循环次数
    int scTotalCycles;     //总循环次数
    bool scIsRunning;      //标定是否正在运行
    QVector<QString> scCalibrationData; //标定数据存储

    void SerialInit();
    void poseXChartInit();

private slots:
    void scProcessCalibrationData(); //处理标定数据
    void on_portRefreshBtn_released();

    void on_openSerialBtn_released();

    void on_beginBtn_clicked();

    // 串口读取数据，解析，保存
    void SerialTimerStart(); // 串口接收数据定时器

    void ReadData(); //读取串口接收到的完整数据帧

    void DataProcess(uint8_t *data); //数据处理

    void UpdateData(); //更新用于保存的数据

    void on_saveDataBtn_clicked(); //保存数据，导出csv文件

    // 更新曲线函数
    void DrawLine();

    // 九轴姿态传感器XY角度归零
    void on_bodyPoseZeroBtn_clicked();

    void on_endPoseZeroBtn_clicked();

    // 运动模式选择
    void on_SModeBtn_pressed();

    void on_TModeBtn_pressed();

    void on_RModeBtn_pressed();

    void on_SCModeBtn_pressed();

    // 自标定模式相关槽函数
    void on_scStartBtn_clicked();
    void on_scStopBtn_clicked();
    void on_scSaveDataBtn_clicked();
    void on_scClearDataBtn_clicked();

    // 单关节点动模式----电机恒定转矩模式，用于初始阶段抽绳
    void on_torqueBtn_RF_clicked();

    void on_torqueBtn_LF_clicked();

    void on_torqueBtn_LB_clicked();

    void on_torqueBtn_RB_clicked();

    // 单关节点动模式----RF电机收放绳
    void on_tightRopeBtn_RF_pressed();

    void on_tightRopeBtn_RF_released();

    void on_looseRopeBtn_RF_pressed();

    void on_looseRopeBtn_RF_released();

    // 单关节点动模式----LF电机收放绳
    void on_tightRopeBtn_LF_pressed();

    void on_tightRopeBtn_LF_released();

    void on_looseRopeBtn_LF_pressed();

    void on_looseRopeBtn_LF_released();

    // 单关节点动模式----LB电机收放绳
    void on_tightRopeBtn_LB_pressed();

    void on_tightRopeBtn_LB_released();

    void on_looseRopeBtn_LB_pressed();

    void on_looseRopeBtn_LB_released();

    // 单关节点动模式----RB电机收放绳
    void on_tightRopeBtn_RB_pressed();

    void on_tightRopeBtn_RB_released();

    void on_looseRopeBtn_RB_pressed();

    void on_looseRopeBtn_RB_released();

    // 轨迹规划模式
    void on_tensionTrjBtn_clicked();

    void on_cableTensionBtn_clicked();

    void on_trjStopBtn_clicked();

    void on_trjRisejBtn_clicked();

    void on_trjLinejBtn_clicked();

    void on_trjPolyjBtn_clicked();

    void on_trjCircleBtn_clicked();

    void on_trjFallBtn_clicked();


    void on_grabBtn_clicked();

    void on_looseBtn_clicked();

private:
    Ui::mainUI *ui;
    QSerialPort *MySerialPort;
    QVector<QString> saveData;
    int count = 0; //用于绘制曲线的计数
};
#endif // MAINUI_H

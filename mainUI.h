#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QList>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <vector>
#include <QMessageBox>

#include <QStackedLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QValueAxis>
#include <QWebEngineView>  // 引入 WebEngine 视图头文件

//#include <Qcamera>
//#include <QCameraImageCapture>
//#include <qcamerainfo>
//#include <QCameraViewfinder>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
// class QCamera;
// class QCameraViewfinder;
// class QCameraImageCapture;
// class QCameraInfo;
// class QCameraViewfinderSettings;

// QT_CHARTS_USE_NAMESPACE

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
    
    // 拖动模式相关成员变量
    bool dragModeActive;   //拖动模式是否激活
    int dragPositionIndex; //当前拖动位置索引（0或1，共记录两个位置）
    float dragPos1[4];     //拖动位置1：4个电机的角度
    float dragPos2[4];     //拖动位置2：4个电机的角度
    float dragPose1[3];    //拖动位置1：姿态（X/Y/Z）
    float dragPose2[3];   //拖动位置2：姿态（X/Y/Z）
    
    // 测量初始绳长模式相关成员变量
    bool measureRopeLengthActive; //测量初始绳长模式是否激活
    float initialRopeAngle[4];    //4个电机的初始角度
    float currentRopeAngle[4];    //4个电机的当前角度

    void SerialInit();
    void poseXChartInit();
    void webCam();

private slots:
    // void scProcessCalibrationData(); //处理标定数据
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

    void on_DragModeBtn_pressed();

    void on_SCModeBtn_pressed();

    // 自标定模式相关槽函数
    void on_scStartBtn_clicked();
    void on_scStopBtn_clicked();
    void on_scNextStepBtn_clicked(); // 下一步标定
    void on_scSaveDataBtn_clicked();
    void on_scClearDataBtn_clicked();
    void on_scRecordPosBtn_clicked(); // 手动记录角度位置
    
    // 拖动模式相关槽函数
    void on_dragModeStartBtn_pressed();      // 进入拖动模式
    void on_dragModeStopBtn_clicked();       // 退出拖动模式
    void on_dragRecordPos1Btn_clicked();     // 记录位置1
    void on_dragRecordPos2Btn_clicked();     // 记录位置2
    void on_dragClearBtn_clicked();          // 清空拖动位置记录
    void on_dragSaveDataBtn_clicked();       // 保存位置数据到CSV文件
    
    // 测量初始绳长模式相关槽函数
    // void on_measureRopeLengthBtn_clicked();  // 进入测量初始绳长模式
    void on_MeasureRopeModeBtn_pressed();
    void on_measureRopeLengthStopBtn_clicked(); // 退出测量初始绳长模式
    void on_tightRopeBtn_Measure_RF_clicked(); // RF电机收绳
    void on_tightRopeBtn_Measure_LF_clicked(); // LF电机收绳
    void on_tightRopeBtn_Measure_LB_clicked(); // LB电机收绳
    void on_tightRopeBtn_Measure_RB_clicked(); // RB电机收绳
    void on_recordInitialBtn_RF_clicked();   // 记录RF电机初始位置
    void on_recordInitialBtn_LF_clicked();   // 记录LF电机初始位置
    void on_recordInitialBtn_LB_clicked();   // 记录LB电机初始位置
    void on_recordInitialBtn_RB_clicked();   // 记录RB电机初始位置
    void on_recordCurrentBtn_RF_clicked();   // 记录RF电机当前位置
    void on_recordCurrentBtn_LF_clicked();   // 记录LF电机当前位置
    void on_recordCurrentBtn_LB_clicked();   // 记录LB电机当前位置
    void on_recordCurrentBtn_RB_clicked();   // 记录RB电机当前位置
    void on_exportDataBtn_clicked();

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

    // 平台升降模式
    void on_LiftModeBtn_pressed();
    void on_liftUpBtn_clicked();
    void on_liftDownBtn_clicked();
    void on_liftStopBtn_clicked();

    //单关节电动，升降平台电机控制
    void on_tightRopeBtn_LIFT_pressed();

    void on_tightRopeBtn_LIFT_released();

    void on_looseRopeBtn_LIFT_pressed();

    void on_looseRopeBtn_LIFT_released();

    void on_grabBtn_clicked();

    void on_looseBtn_clicked();

    void on_iploadBtn_clicked();
    void on_ipreloadBtn_clicked();

    // void onOpenCamera();
    // void onCaptureImage();
    // void onCloseCamera();
    // void onImageCaptured(int id, const QImage &preview);
private:
    Ui::mainUI *ui;
    QSerialPort *MySerialPort;
    QVector<QString> saveData;
    int count = 0; //用于绘制曲线的计数

    QWebEngineView *m_webView;

    // QCamera *camera;
    // QCameraImageCapture *capture;
    // QCameraViewfinder *viewfinder;
    // QCameraImageCapture *imageCapture;
    // QList<QCameraInfo> cameras;
    // QList<QSize> mResSize;

    // QCamera *m_camera = nullptr;
    // QCameraViewfinder *m_viewfinder = nullptr;
    // QCameraImageCapture *m_imageCapture = nullptr;

    // QPushButton *m_btnOpen;
    // QPushButton *m_btnCapture;
    // QPushButton *m_btnClose;
    // QLabel *m_labelPhoto; // 指向UI设计器中创建的photoLabel控件

};
#endif // MAINUI_H

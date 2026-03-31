#include "mainUI.h"
#include "ui_mainUI.h"
#include <QUrl>
#include <QDebug>

mainUI::mainUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainUI)
{
    ui->setupUi(this);
    ui->mainStackedWidget->setCurrentWidget(ui->homePage);
    MySerialPort = new QSerialPort;
    SerialTimer = new QTimer();

    SerialInit();
    connect(MySerialPort, &QSerialPort::readyRead, this, &mainUI::SerialTimerStart);
    connect(SerialTimer, SIGNAL(timeout()), this, SLOT(ReadData()));

    poseXChart = new QChart();
    drawLineTimer = new QTimer();
    poseXLineSeries = new QSplineSeries();

    // 初始化自标定模式相关变量
    // scTimer = new QTimer();
    scCurrentCycle = 0;
    scTotalCycles = 10;
    scIsRunning = false; 
    
    // 初始化拖动模式相关变量
    dragModeActive = false;
    dragPositionIndex = 0;
    for(int i = 0; i < 4; i++)
    {
        dragPos1[i] = 0.0f;
        dragPos2[i] = 0.0f;
    }
    for(int i = 0; i < 3; i++)
    {
        dragPose1[i] = 0.0f;
        dragPose2[i] = 0.0f;
    }
    
    // 初始化测量初始绳长模式相关变量
    measureRopeLengthActive = false;
    for(int i = 0; i < 4; i++)
    {
        initialRopeAngle[i] = 0.0f;
        currentRopeAngle[i] = 0.0f;
    }

    poseXChartInit();
    connect(drawLineTimer, SIGNAL(timeout()), this, SLOT(DrawLine()));
    // connect(scTimer, SIGNAL(timeout()), this, SLOT(scProcessCalibrationData()));

    // // 初始化相机预览控件，将其放置在cameraPage中
    // m_viewfinder = new QCameraViewfinder(ui->cameraPage);
    // m_viewfinder->setGeometry(10, 10, 640, 480);
    // m_viewfinder->setMinimumSize(640, 480);

    // // 使用UI设计器中创建的标签控件（假设在设计器中已添加名为photoLabel的QLabel）
    // // 注意：需要在ui文件中确保photoLabel已放置在cameraPage页面中并设置了合适的几何属性
    // m_labelPhoto = ui->photoLabel;

    // // 使用现有的UI控件进行信号槽连接
    // connect(ui->shootBtn, &QPushButton::clicked, this, &mainUI::onCaptureImage);

    // // 创建临时的打开/关闭按钮（也可以在UI设计器中添加）
    // m_btnOpen = new QPushButton("打开摄像头", ui->cameraPage);
    // m_btnOpen->setGeometry(100, 540, 100, 30);
    // m_btnClose = new QPushButton("关闭摄像头", ui->cameraPage);
    // m_btnClose->setGeometry(250, 540, 100, 30);
    
    // // 连接打开/关闭摄像头按钮的信号
    // connect(m_btnOpen, &QPushButton::clicked, this, &mainUI::onOpenCamera);
    // connect(m_btnClose, &QPushButton::clicked, this, &mainUI::onCloseCamera);

    // // 获取并显示可用摄像头列表
    // cameras = QCameraInfo::availableCameras();
    // for(const QCameraInfo &cameraInfo : cameras)
    // {
    //     ui->cameraSel->addItem(cameraInfo.description());
    // }
    
    // // 摄像头选择变更的连接
    // connect(ui->cameraSel, QOverload<int>::of(&QComboBox::activated),[=](int index){
    //     if(m_camera) {
    //         m_camera->stop();
    //         delete m_camera;
    //         m_camera = nullptr;
    //         delete m_imageCapture;
    //         m_imageCapture = nullptr;
    //     }
        
    //     m_camera = new QCamera(cameras[index], this);
    //     m_camera->setViewfinder(m_viewfinder);
        
    //     m_imageCapture = new QCameraImageCapture(m_camera, this);
    //     connect(m_imageCapture, &QCameraImageCapture::imageCaptured,
    //             this, &mainUI::onImageCaptured);
        
    //     m_camera->start();
        
    //     // 更新分辨率选项
    //     ui->resolutionSel->clear();
    //     QList<QSize> resSize = m_camera->supportedViewfinderResolutions();
    //     for(const QSize &size : resSize)
    //     {
    //         ui->resolutionSel->addItem(QString::number(size.width()) + "*" + QString::number(size.height()));
    //     }
    //     ui->resolutionSel->setCurrentIndex(0);
    // });
    
    // // 分辨率选择变更的连接
    // connect(ui->resolutionSel, QOverload<int>::of(&QComboBox::activated), [=](int index){
    //     if(!m_camera) return;
        
    //     QList<QSize> resSize = m_camera->supportedViewfinderResolutions();
    //     if(index >= 0 && index < resSize.size()) {
    //         QCameraViewfinderSettings settings;
    //         settings.setResolution(resSize[index]);
    //         m_camera->setViewfinderSettings(settings);
    //     }
    // });
}

// void mainUI::onOpenCamera()
// {
//     if (m_camera) return; // 已经打开

//     // 获取当前选择的摄像头
//     int currentIndex = ui->cameraSel->currentIndex();
//     if(currentIndex >= 0 && currentIndex < cameras.size()) {
//         m_camera = new QCamera(cameras[currentIndex], this);
//     } else if(!cameras.isEmpty()) {
//         // 如果没有选择或索引无效，使用第一个摄像头
//         m_camera = new QCamera(cameras.first(), this);
//         ui->cameraSel->setCurrentIndex(0);
//     } else {
//         qDebug() << "未找到可用摄像头";
//         return;
//     }
    
//     m_camera->setViewfinder(m_viewfinder);

//     m_imageCapture = new QCameraImageCapture(m_camera, this);
//     connect(m_imageCapture, &QCameraImageCapture::imageCaptured,
//             this, &mainUI::onImageCaptured);

//     m_camera->start();
//     qDebug() << "摄像头已打开";
    
//     // 更新分辨率选项
//     ui->resolutionSel->clear();
//     QList<QSize> resSize = m_camera->supportedViewfinderResolutions();
//     for(const QSize &size : resSize)
//     {
//         ui->resolutionSel->addItem(QString::number(size.width()) + "*" + QString::number(size.height()));
//     }
//     ui->resolutionSel->setCurrentIndex(0);
// }

// void mainUI::onCaptureImage()
// {
//     if (!m_imageCapture) return;

//     QString filename = QString("photo_%1.jpg")
//                            .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
//     m_imageCapture->capture(filename);
//     qDebug() << "已拍照保存到:" << filename;
// }

// void mainUI::onCloseCamera()
// {
//     if (m_camera) {
//         m_camera->stop();
//         delete m_camera;
//         m_camera = nullptr;

//         delete m_imageCapture;
//         m_imageCapture = nullptr;

//         qDebug() << "摄像头已关闭";
//     }
// }

// void mainUI::onImageCaptured(int, const QImage &preview)
// {
//     m_labelPhoto->setPixmap(QPixmap::fromImage(preview).scaled(m_labelPhoto->size(),
//                                                                Qt::KeepAspectRatio,
//                                                                Qt::SmoothTransformation));
// }


mainUI::~mainUI()
{
    // 若串口打开，关闭串口
    if(MySerialPort->isOpen())
    {
        MySerialPort->close();
    }
    delete ui;
}

void mainUI::webCam()
{
    // 1. 初始化 WebEngine 视图
    m_webView = new QWebEngineView(this);

    QStackedLayout *layout = new QStackedLayout(ui->webcamFrame);
    ui->webcamFrame->setLayout(layout);
    layout->addWidget(m_webView);

    // 2. 设置树莓派 WEB 端地址（二选一）
    // 🔴 方式A：树莓派 AP 模式（Windows 连 AccessPopup 热点）
    QUrl webUrl("https://www.baidu.com");
    // QUrl webUrl("http://192.168.50.5:5000");
    // 🔴 方式B：树莓派 STA 模式（替换为实际 IP，如 192.168.10.50）
    // QUrl webUrl("http://192.168.10.50:5000");

    // 3. 加载 WEB 页面
    m_webView->load(webUrl);

    m_webView->show();
    // 4. 调试：打印加载状态（可选）
    connect(m_webView, &QWebEngineView::loadStarted, []() {
        qDebug() << "开始加载树莓派 WEB 端...";
    });
    connect(m_webView, &QWebEngineView::loadFinished, [](bool success) {
        if (success) {
            qDebug() << "WEB 端加载成功！";
        } else {
            qDebug() << "WEB 端加载失败，请检查网络/地址！";
        }
    });


    // 5. 将 WebView 设置为主窗口中央部件（占满窗口）
    // setCentralWidget(m_webView);

    // // 6. 设置窗口属性（可选）
    // setWindowTitle("树莓派机器人 WEB 控制客户端");
    // resize(800, 600);  // 适配视频流分辨率（640x480）
}


void mainUI::on_iploadBtn_clicked()
{
    //加载按钮槽函数，加载地址栏里面显示的链接。
    QString url = ui->addressEdit->text();
    if (!url.isEmpty())
    {
        m_webView->load(url);

    }
}

void mainUI::on_ipreloadBtn_clicked()
{
    //刷新按钮槽函数，刷新地址栏里显示的链接
    m_webView->reload();
}

void mainUI::SerialInit()
{
    // 串口选择Cmb
    auto SerialPortInfo = QSerialPortInfo::availablePorts(); // 返回可用串口信息列表
    for (auto& PortInfo : SerialPortInfo) // 遍历可用串口信息列表
    {
        qInfo()<<PortInfo.portName()<<PortInfo.description(); // 输出可用的串口名和串口描述
        ui->portChooseCmb->addItem(PortInfo.portName()+":"+PortInfo.description(),PortInfo.portName()); // 在串口选择Cmb中添加串口信息，并将串口号作为返回值
    }

    // 波特率Cmb
    auto BaudRatesInfo = QSerialPortInfo::standardBaudRates(); // 获取标准波特率列表
    for (auto BaudRates : BaudRatesInfo)
    {
        ui->baudCmb->addItem(QString::number(BaudRates),BaudRates); // QStirng::number() 整型转字符串
    }
    ui->baudCmb->setCurrentText("115200"); // 设置baudCmb默认值

    //connect(MySerialPort, &QSerialPort::readyRead, this, &mainUI::ReadData); // 连接MySerialPort和槽函数ReadData
}

void mainUI::on_portRefreshBtn_released()
{
    // 清除之前内容
    ui->portChooseCmb->clear();

    // 刷新串口信息列表
    auto SerialPortInfo = QSerialPortInfo::availablePorts(); // 返回可用串口信息列表
    for (auto& PortInfo : SerialPortInfo) // 遍历可用串口信息列表
    {
        qInfo()<<PortInfo.portName()<<PortInfo.description(); // 输出可用的串口名和串口描述
        ui->portChooseCmb->addItem(PortInfo.portName()+":"+PortInfo.description(),PortInfo.portName()); // 在串口选择Cmb中添加串口信息，并将串口号作为返回值
    }
}

void mainUI::on_openSerialBtn_released()
{
    if(MySerialPort->isOpen()) // 判断串口是否打开，如果打开则按钮作用变为关闭串口
    {
        MySerialPort->close();
        ui->openSerialBtn->setText("打开串口");
        return;
    }

    // 获取Cmb中选择的串口信息
    auto portName = ui->portChooseCmb->currentData().toString();
    auto baudRate = ui->baudCmb->currentData().value<QSerialPort::BaudRate>();

    // 设置串口参数
    MySerialPort->setPortName(portName);
    MySerialPort->setBaudRate(baudRate);
    MySerialPort->setDataBits(QSerialPort::Data8);
    MySerialPort->setParity(QSerialPort::NoParity);
    MySerialPort->setStopBits(QSerialPort::OneStop);

    // 打开绘图定时器
    drawLineTimer->start(100);

    // 打开串口
    if (!MySerialPort->open(QIODevice::ReadWrite)) // 判断串口是否打开读写模式
    {
        QMessageBox::warning(this, "warning", portName+"连接失败\n"+MySerialPort->errorString());
        return;
    }
    else
    {
        ui->openSerialBtn->setText("关闭串口");
        return;
    }
}

void mainUI::on_beginBtn_clicked()
{
    if(MySerialPort->isOpen()) // 若串口打开，切换至主页面
    {
        ui->mainStackedWidget->setCurrentWidget(ui->mainPage);
        return;
    }
    else
    {
        QMessageBox::warning(this, "warning", "请先打开串口！");
        return;
    }
}


// 串口接收数据并解析
QByteArray rxbuff;
float angle[5], bodyPose[3], velocityPose[3];
uint8_t trj_index = 0;
QString angleStr[5], bodyPoseStr[3], velocityPoseStr[3];

uint8_t Res;  //读取到的每一字节数据
int16_t USART_RX_STA = 0; //接收状态标志
#define USART_REC_LEN 45 //定义最大接收字节数
uint8_t USART_RX_BUF[USART_REC_LEN]; //接收缓冲区
// 串口接收定时器
void mainUI::SerialTimerStart()
{
    SerialTimer->start(50);
    rxbuff.append(MySerialPort->readAll());
}

void mainUI::ReadData()
{
    for(int i =0; i < rxbuff.size(); i++)
    {
        Res = rxbuff[i];
        // qDebug() << "data[" << i << "] =" << QString::number(Res).rightJustified(2, '0');
        if((USART_RX_STA&0x8000))//接收已开始
        {
            if((USART_RX_STA&0x4000))
            {
                USART_RX_BUF[USART_RX_STA&0X3FFF]=rxbuff[i];
                //USART_RX_BUF.append(data[i]) ;
                //qDebug()<<"接收到第数据位"<<USART_RX_STA<<USART_RX_BUF[0]<<endl;
                USART_RX_STA++;
                if((USART_RX_STA&0X3FFF)>(USART_REC_LEN-1))
                {
                    USART_RX_STA=0;
                    //USART_RX_BUF.clear();
                }//接收完成
            }
            else
            {
                if(Res == 0x55)
                {
                    USART_RX_STA = 0;//先给后面的计数值清零
                    USART_RX_STA|=0x8000;
                    USART_RX_STA|=0x4000;
                    //qDebug()<<"接收到第二位"<<USART_RX_STA<<endl;
                }
                else USART_RX_STA=0;//接收错误,重新开始
            }
        }
        else
        {
            if(Res == 0xAA)
            {USART_RX_STA|=0x8000;
            //qDebug()<<"接收到第一位"<<USART_RX_STA<<endl;
            }
            else USART_RX_STA=0;//接收错误,重新开始
        }
    }

    if(USART_RX_STA == 0)
    {
        // qDebug() << "Complete packet received:";
        // for(int i = 0; i < USART_REC_LEN; i++)
        // {
        //     qDebug() << "USART_RX_BUF[" << i << "] = 0x" << QString::number(USART_RX_BUF[i], 16).rightJustified(2, '0');
        // }
        DataProcess(USART_RX_BUF);
    }

    rxbuff.clear();

}

// 对数据帧进行解析
void mainUI::DataProcess(uint8_t *data) //处理的数据不包含包头
{
    // 输出接收到的全部数据
    // qDebug() << "Received data:";
    // for(int i = 0; i < USART_REC_LEN; i++) {
    //     qDebug() << "data[" << i << "] =" << QString::number(data[i], 16).rightJustified(2, '0');
    // }
    
    bodyPose[0] = ((float)((int32_t)(data[0] | (((uint32_t)data[1]) << 8) | (((uint32_t)data[2]) << 16) | (((uint32_t)data[3]) << 24)))) / 1000.0f;
    bodyPose[1] = ((float)((int32_t)(data[4] | (((uint32_t)data[5]) << 8) | (((uint32_t)data[6]) << 16) | (((uint32_t)data[7]) << 24)))) / 1000.0f;
    bodyPose[2] = ((float)((int32_t)(data[8] | (((uint32_t)data[9]) << 8) | (((uint32_t)data[10]) << 16) | (((uint32_t)data[11]) << 24)))) / 1000.0f;

    angle[0] = ((float)((int32_t)(data[12] | (((uint32_t)data[13]) << 8) | (((uint32_t)data[14]) << 16) | (((uint32_t)data[15]) << 24)))) / 1000.0f;
    angle[1] = ((float)((int32_t)(data[16] | (((uint32_t)data[17]) << 8) | (((uint32_t)data[18]) << 16) | (((uint32_t)data[19]) << 24)))) / 1000.0f;
    angle[2] = ((float)((int32_t)(data[20] | (((uint32_t)data[21]) << 8) | (((uint32_t)data[22]) << 16) | (((uint32_t)data[23]) << 24)))) / 1000.0f;
    angle[3] = ((float)((int32_t)(data[24] | (((uint32_t)data[25]) << 8) | (((uint32_t)data[26]) << 16) | (((uint32_t)data[27]) << 24)))) / 1000.0f;
    angle[4] = ((float)((int32_t)(data[28] | (((uint32_t)data[29]) << 8) | (((uint32_t)data[30]) << 16) | (((uint32_t)data[31]) << 24)))) / 1000.0f;
    // qDebug() << QString::number(angle[4]).rightJustified(2, '0');
    velocityPose[0] = ((float)((int32_t)(data[32] | (((uint32_t)data[33]) << 8) | (((uint32_t)data[34]) << 16) | (((uint32_t)data[35]) << 24)))) / 1000.0f;
    velocityPose[1] = ((float)((int32_t)(data[36] | (((uint32_t)data[37]) << 8) | (((uint32_t)data[38]) << 16) | (((uint32_t)data[39]) << 24)))) / 1000.0f;
    velocityPose[2] = ((float)((int32_t)(data[40] | (((uint32_t)data[41]) << 8) | (((uint32_t)data[42]) << 16) | (((uint32_t)data[43]) << 24)))) / 1000.0f;

    trj_index = data[44];

    for(int i=0; i<3; i++)
    {
        bodyPoseStr[i] = QString::number(bodyPose[i],'f',3);
        velocityPoseStr[i] = QString::number(velocityPose[i],'f',3);
    }

    for(int i=0; i<5; i++)
    {
        angleStr[i] = QString::number(angle[i],'f',3);
    }

    ui->angleRFEdit->setText(angleStr[0]);
    ui->angleLFEdit->setText(angleStr[1]);
    ui->angleLBEdit->setText(angleStr[2]);
    ui->angleRBEdit->setText(angleStr[3]);
    ui->angleLIFTEdit->setText(angleStr[4]);

    ui->bodyPoseXEdit->setText(bodyPoseStr[0]);
    ui->bodyPoseYEdit->setText(bodyPoseStr[1]);
    ui->bodyPoseZEdit->setText(bodyPoseStr[2]);

    ui->velocityPoseXEdit->setText(velocityPoseStr[0]);
    ui->velocityPoseYEdit->setText(velocityPoseStr[1]);
    ui->velocityPoseZEdit->setText(velocityPoseStr[2]);

    UpdateData();
}

void mainUI::UpdateData()
{
    // 获取当前系统时间
    QDateTime currentTime = QDateTime::currentDateTime();
    QString currentTimeStr = currentTime.toString(Qt::ISODate);

    // 新建一个QString，保存每一次更新的数据
    QString dataStr = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12")
            .arg(currentTimeStr)
            .arg(trj_index)
            .arg(angle[0])
            .arg(angle[1])
            .arg(angle[2])
            .arg(angle[3])
            .arg(bodyPose[0])
            .arg(bodyPose[1])
            .arg(bodyPose[2])
            .arg(velocityPoseStr[0])
            .arg(velocityPoseStr[1])
            .arg(velocityPoseStr[2]);

    //qDebug() << dataStr;

    saveData.append(dataStr);

    if(saveData.size() > 8000) //数据缓冲区最大容量为5000组
    {
        saveData.remove(0); //超过5000组数据后，每次删除首项数据进行覆盖
    }
}

void mainUI::on_saveDataBtn_clicked()
{
    if(saveData.size()<1)
    {
        QMessageBox::information(this, "提示", "当前数据为空，无法保存");
        return;
    }

    QString csvFile = QFileDialog::getExistingDirectory(this); //获取当前项目所在目录
    if(csvFile.isEmpty())
        return;

    QDateTime current_date_time = QDateTime::currentDateTime(); //获取当前日期时间
    QString current_date = current_date_time.toString("yyyy_MM_dd_hh_mm");
    csvFile += tr("%1.csv").arg(current_date);
    qDebug() << csvFile;

    QFile file(csvFile);
    if(file.exists())
    {
        //如果文件存在执行的操作，此处为空，因为文件不可能存在
    }
    file.open(QIODevice::ReadWrite | QIODevice::Text); //打开文本读写模式
    QTextStream out(&file);
    out << tr("Time,") << tr("trj_index,") << tr("angleRF,") << tr("angleLF,") << tr("angleLB,") << tr("angleRB,")
        << tr("bodyPoseX,") << tr("bodyPoseY,") << tr("bodyPoseZ,")
        << tr("velocityPoseX,") << tr("velocityPoseY,") << tr("velocityPoseZ,") << "\n";
    for(const auto &data : saveData)
    {
        out << data << "\n";
    }
    file.close();
    QVector<QString>().swap(saveData); //保存数据后清空缓冲区
    QMessageBox::information(this, "提示", "保存数据成功");
}


// 绘制实时曲线相关函数
void mainUI::poseXChartInit()
{
    // 设置曲线名称
    poseXLineSeries->setName("X轴倾角");
    poseXChart->addSeries(poseXLineSeries);

    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();

    // 设置坐标轴显示范围，X轴为时间，Y轴为倾角
    axisX->setMin(0);
    axisX->setMax(AXIS_MAX_X);
    axisY->setMin(-AXIS_MAX_Y);
    axisY->setMax(AXIS_MAX_Y);

    // 设置坐标轴格点
    axisX->setTickCount(5);
    axisY->setTickCount(5);

    // 设置坐标轴显示名称
    QFont font("Microsoft YaHei", 8, QFont::Normal); //微软雅黑，大小为8
    axisX->setTitleFont(font);
    axisY->setTitleFont(font);
    axisX->setTitleText("时间/s");
    axisY->setTitleText("X轴倾角/°");

    // 设置网格不显示
    axisY->setGridLineVisible(false);

    // 添加坐标轴到图表
    poseXChart->addAxis(axisX, Qt::AlignBottom);
    poseXChart->addAxis(axisY, Qt::AlignLeft);

    // 把曲线关联到坐标轴
    poseXLineSeries->attachAxis(axisX);
    poseXLineSeries->attachAxis(axisY);

    // 将图像显示到窗口上
    ui->poseXView->setChart(poseXChart);
    ui->poseXView->setRenderHint(QPainter::Antialiasing); //设置渲染：抗锯齿，如果不设置那么曲线就显得不平滑
}

void mainUI::DrawLine()
{
    if(count > AXIS_MAX_X)
    {
        //当曲线上最早的点超出X轴的范围时，剔除最早的点，
        poseXLineSeries->removePoints(0,poseXLineSeries->count() - AXIS_MAX_X);
        poseXChart->axisX()->setMin(count - AXIS_MAX_X);
        poseXChart->axisX()->setMax(count);
    }
    else{
        poseXChart->axisX()->setMin(0);
        poseXChart->axisX()->setMax(AXIS_MAX_X);
    }
    poseXLineSeries->append(count, (int)bodyPose[0]);
    count ++;
}



// 九轴姿态传感器XY角度归零
void mainUI::on_bodyPoseZeroBtn_clicked()
{
    QByteArray TxData;
    TxData = "$Wzero";
    MySerialPort->write(TxData);
}

void mainUI::on_wheelStartBtn_clicked()
{
    QByteArray TxData;
    TxData = "$Wstart";
    MySerialPort->write(TxData);
}

void mainUI::on_wheelStopBtn_clicked()
{
    QByteArray TxData;
    TxData = "$Wstop";
    MySerialPort->write(TxData);
}

// 运动模式选择
void mainUI::on_SModeBtn_pressed()
{
    ui->modeStackedWidget->setCurrentWidget(ui->SModePage);
    QString TxData = "SMode";
    MySerialPort->write(TxData.toLocal8Bit());
    measureRopeLengthActive = false;
}

void mainUI::on_TModeBtn_pressed()
{
    ui->modeStackedWidget->setCurrentWidget(ui->TModePage);
    QString TxData = "TMode";
    MySerialPort->write(TxData.toLocal8Bit());
    measureRopeLengthActive = false;
}

void mainUI::on_DragModeBtn_pressed()
{
    ui->modeStackedWidget->setCurrentWidget(ui->DragModePage);
    QString TxData = "CTMode";
    MySerialPort->write(TxData.toLocal8Bit());
    measureRopeLengthActive = false;
}

void mainUI::on_SCModeBtn_pressed()
{
    ui->modeStackedWidget->setCurrentWidget(ui->SCModePage);
    QString TxData = "SCMode";
    MySerialPort->write(TxData.toLocal8Bit());
    measureRopeLengthActive = false;
}

// 测量初始绳长模式单选按钮槽函数
void mainUI::on_MeasureRopeModeBtn_pressed()
{
    if(!MySerialPort->isOpen())
    {
        QMessageBox::warning(this, "警告", "请先打开串口！");
        return;
    }
    
    if(scIsRunning || dragModeActive)
    {
        QMessageBox::warning(this, "警告", "其他模式正在运行中，无法进入测量初始绳长模式！");
        return;
    }
    
    if(measureRopeLengthActive)
    {
        QMessageBox::information(this, "提示", "测量初始绳长模式已经激活！");
        return;
    }
    
    // 激活测量初始绳长模式
    measureRopeLengthActive = true;
    
    // 更新UI状态
    ui->modeStackedWidget->setCurrentWidget(ui->MeasureRopeModePage);
    
    QByteArray TxData;
    TxData = "CTMode";  // 切换到恒力矩模式，便于手动拖动
    MySerialPort->write(TxData);
    
    // QMessageBox::information(this, "提示", "测量初始绳长模式已激活！现在可以控制各电机收绳并记录位置。");
}

// 自由抽绳
void mainUI::on_cableTensionBtn_clicked()
{
    QByteArray TxData;
    TxData = "CTMode";
    MySerialPort->write(TxData);
}

// 单关节点动模式----电机恒定转矩模式，用于初始阶段抽绳
void mainUI::on_torqueBtn_RF_clicked()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('T');
    TxData.append('\x01');
    MySerialPort->write(TxData);
}

void mainUI::on_torqueBtn_LF_clicked()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('T');
    TxData.append('\x02');
    MySerialPort->write(TxData);
}

void mainUI::on_torqueBtn_LB_clicked()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('T');
    TxData.append('\x03');
    MySerialPort->write(TxData);
}

void mainUI::on_torqueBtn_RB_clicked()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('T');
    TxData.append('\x04');
    MySerialPort->write(TxData);
}


// 单关节点动模式----RF电机收放绳
void mainUI::on_tightRopeBtn_RF_pressed()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x01');
    TxData.append('\x00');
    int16_t speed = ui->speedEdit_RF->text().toInt();
    TxData.append((speed >> 8));
    TxData.append(speed);
    MySerialPort->write(TxData);
}

void mainUI::on_tightRopeBtn_RF_released()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x01');
    TxData.append('\x00');
    TxData.append('\x00');
    TxData.append('\x00');
    MySerialPort->write(TxData);
}

void mainUI::on_looseRopeBtn_RF_pressed()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x01');
    TxData.append('\x01');
    int16_t speed = ui->speedEdit_RF->text().toInt();
    TxData.append((speed >> 8));
    TxData.append(speed);
    MySerialPort->write(TxData);
}

void mainUI::on_looseRopeBtn_RF_released()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x01');
    TxData.append('\x01');
    TxData.append('\x00');
    TxData.append('\x00');
    MySerialPort->write(TxData);
}

// 单关节点动模式----LF电机收放绳
void mainUI::on_tightRopeBtn_LF_pressed()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x02');
    TxData.append('\x00');
    int16_t speed = ui->speedEdit_LF->text().toInt();
    TxData.append((speed >> 8));
    TxData.append(speed);
    MySerialPort->write(TxData);
}

void mainUI::on_tightRopeBtn_LF_released()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x02');
    TxData.append('\x00');
    TxData.append('\x00');
    TxData.append('\x00');
    MySerialPort->write(TxData);
}

void mainUI::on_looseRopeBtn_LF_pressed()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x02');
    TxData.append('\x01');
    int16_t speed = ui->speedEdit_LF->text().toInt();
    TxData.append((speed >> 8));
    TxData.append(speed);
    MySerialPort->write(TxData);
}

void mainUI::on_looseRopeBtn_LF_released()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x02');
    TxData.append('\x01');
    TxData.append('\x00');
    TxData.append('\x00');
    MySerialPort->write(TxData);
}

// 单关节点动模式----LB电机收放绳
void mainUI::on_tightRopeBtn_LB_pressed()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x03');
    TxData.append('\x00');
    int16_t speed = ui->speedEdit_LB->text().toInt();
    TxData.append((speed >> 8));
    TxData.append(speed);
    MySerialPort->write(TxData);
}

void mainUI::on_tightRopeBtn_LB_released()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x03');
    TxData.append('\x00');
    TxData.append('\x00');
    TxData.append('\x00');
    MySerialPort->write(TxData);
}

void mainUI::on_looseRopeBtn_LB_pressed()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x03');
    TxData.append('\x01');
    int16_t speed = ui->speedEdit_LB->text().toInt();
    TxData.append((speed >> 8));
    TxData.append(speed);
    MySerialPort->write(TxData);
}

void mainUI::on_looseRopeBtn_LB_released()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x03');
    TxData.append('\x01');
    TxData.append('\x00');
    TxData.append('\x00');
    MySerialPort->write(TxData);
}

// 单关节点动模式----RB电机收放绳
void mainUI::on_tightRopeBtn_RB_pressed()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x04');
    TxData.append('\x00');
    int16_t speed = ui->speedEdit_RB->text().toInt();
    TxData.append((speed >> 8));
    TxData.append(speed);
    MySerialPort->write(TxData);
}

void mainUI::on_tightRopeBtn_RB_released()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x04');
    TxData.append('\x00');
    TxData.append('\x00');
    TxData.append('\x00');
    MySerialPort->write(TxData);
}

void mainUI::on_looseRopeBtn_RB_pressed()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x04');
    TxData.append('\x01');
    int16_t speed = ui->speedEdit_RB->text().toInt();
    TxData.append((speed >> 8));
    TxData.append(speed);
    MySerialPort->write(TxData);
}

void mainUI::on_looseRopeBtn_RB_released()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x04');
    TxData.append('\x01');
    TxData.append('\x00');
    TxData.append('\x00');
    MySerialPort->write(TxData);
}


void mainUI::on_tightRopeBtn_LIFT_pressed()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x05');
    TxData.append('\x00');
    int16_t speed = ui->speedEdit_LIFT->text().toInt();
    TxData.append((speed >> 8));
    TxData.append(speed);
    MySerialPort->write(TxData);
}

void mainUI::on_tightRopeBtn_LIFT_released()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x05');
    TxData.append('\x00');
    TxData.append('\x00');
    TxData.append('\x00');
    MySerialPort->write(TxData);
}

void mainUI::on_looseRopeBtn_LIFT_pressed()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x05');
    TxData.append('\x01');
    int16_t speed = ui->speedEdit_LIFT->text().toInt();
    TxData.append((speed >> 8));
    TxData.append(speed);
    MySerialPort->write(TxData);
}

void mainUI::on_looseRopeBtn_LIFT_released()
{
    QByteArray TxData;
    TxData.append('#');
    TxData.append('S');
    TxData.append('M');
    TxData.append('\x05');
    TxData.append('\x01');
    TxData.append('\x00');
    TxData.append('\x00');
    MySerialPort->write(TxData);
}

// 轨迹规划模式
void mainUI::on_tensionTrjBtn_clicked()
{
    QByteArray TxData;
    TxData = "#Ttension";
    MySerialPort->write(TxData);
}

void mainUI::on_trjStopBtn_clicked()
{
    QByteArray TxData;
    TxData = "#Tstop";
    MySerialPort->write(TxData);
}


void mainUI::on_trjRisejBtn_clicked()
{
    QByteArray TxData;
    TxData = "#Trise";
    MySerialPort->write(TxData);
}


void mainUI::on_trjLinejBtn_clicked()
{
    QByteArray TxData;
    TxData = "#Tline";
    MySerialPort->write(TxData);
}

void mainUI::on_trjPolyjBtn_clicked()
{
    QByteArray TxData;
    TxData = "#Tpoly";
    MySerialPort->write(TxData);
}

void mainUI::on_trjCircleBtn_clicked()
{
    QByteArray TxData;
    TxData = "#Tcircle";
    MySerialPort->write(TxData);
}

void mainUI::on_trjFallBtn_clicked()
{
    QByteArray TxData;
    TxData = "#Tfall";
    MySerialPort->write(TxData);
}

// 夹爪命令
void mainUI::on_grabBtn_clicked()
{
    QByteArray TxData;
    TxData = "Grab";
    MySerialPort->write(TxData);
}

void mainUI::on_looseBtn_clicked()
{
    QByteArray TxData;
    TxData = "Loose";
    MySerialPort->write(TxData);
}

// 平台升降模式
void mainUI::on_LiftModeBtn_pressed()
{
    ui->modeStackedWidget->setCurrentWidget(ui->LiftModePage);
    QByteArray TxData;
    TxData = "LIFT";
    MySerialPort->write(TxData);
}

void mainUI::on_liftUpBtn_clicked()
{
    QByteArray TxData;
    TxData = "#Tup";
    MySerialPort->write(TxData);

}

void mainUI::on_liftDownBtn_clicked()
{
    QByteArray TxData;
    TxData = "#Tdown";
    MySerialPort->write(TxData);
}

void mainUI::on_liftStopBtn_clicked()
{
    QByteArray TxData;
    TxData = "#Tstop";
    MySerialPort->write(TxData);
}

// 自标定模式相关函数实现
void mainUI::on_scStartBtn_clicked()
{
    if(!MySerialPort->isOpen())
    {
        QMessageBox::warning(this, "警告", "请先打开串口！");
        return;
    }

    if(scIsRunning)
    {
        QMessageBox::information(this, "提示", "标定已经开始，请使用记录位置按钮进行标定！");
        return;
    }

    // 从UI读取总循环次数
    if(ui->scCyclesSpinBox)
    {
        scTotalCycles = ui->scCyclesSpinBox->value();
    }

    // 初始化标定状态
    scCurrentCycle = 0;
    scIsRunning = true;

    // 更新UI状态
    ui->scStatusLabel->setText("标定准备就绪");
    ui->scCurrentLabel->setText(QString::number(scCurrentCycle));
    ui->scStartBtn->setEnabled(false);
    ui->scStopBtn->setEnabled(true);
    
    // 启用下一步按钮（假设UI中已有此按钮）
    if(ui->scNextStepBtn)
        ui->scNextStepBtn->setEnabled(true);
            // 启用记录位置按钮
    if(ui->scRecordPosBtn)
        ui->scRecordPosBtn->setEnabled(true);
        
    QMessageBox::information(this, "提示", QString("标定已准备就绪，共设置 %1 次循环，请调整到目标位置后点击'记录位置'按钮开始标定流程").arg(scTotalCycles));
}

// 下一步标定按钮处理函数
void mainUI::on_scNextStepBtn_clicked()
{
    if(!scIsRunning)
    {
        QMessageBox::warning(this, "警告", "请先点击开始标定按钮！");
        return;
    }
    
    // 检查是否已达到循环次数上限
    if(scCurrentCycle >= scTotalCycles)
    {
        QMessageBox::information(this, "提示", QString("已达到设置的循环次数上限（%1 次），请点击'停止标定'结束标定流程").arg(scTotalCycles));
        // 禁用下一步按钮
        if(ui->scNextStepBtn)
            ui->scNextStepBtn->setEnabled(false);
        // 更新状态显示
        ui->scStatusLabel->setText("标定完成，已达到上限");
        return;
    }
    
    // 禁用下一步按钮，防止重复点击
    if(ui->scNextStepBtn)
        ui->scNextStepBtn->setEnabled(false);
    
    // 更新状态显示
    ui->scStatusLabel->setText(QString("执行标定步骤... (%1/%2)").arg(scCurrentCycle + 1).arg(scTotalCycles));
    
    // 发送标定指令
    QByteArray TxData;
    TxData = "#Tscmode";
    MySerialPort->write(TxData);

    // 启动定时器，等待位置到达后处理数据
    // scTimer->start(6000); // 等待6秒让机器人完成轨迹并稳定
}

void mainUI::on_scStopBtn_clicked()
{
    scIsRunning = false;
    // scTimer->stop();

    // 更新UI状态
    ui->scStatusLabel->setText("已停止");
    ui->scStartBtn->setEnabled(true);
    ui->scStopBtn->setEnabled(false);
    
    // 禁用下一步按钮和记录位置按钮
    if(ui->scNextStepBtn)
        ui->scNextStepBtn->setEnabled(false);
    if(ui->scRecordPosBtn)
        ui->scRecordPosBtn->setEnabled(false);

    // 发送停止指令
    QByteArray TxData;
    TxData = "#Tstop";
    MySerialPort->write(TxData);
    
    QMessageBox::information(this, "提示", QString("标定已停止！共完成 %1 次标定").arg(scCurrentCycle));
}

// 手动记录自标定角度位置的按钮处理函数
void mainUI::on_scRecordPosBtn_clicked()
{
    if(!scIsRunning)
    {
        QMessageBox::warning(this, "警告", "请先点击开始标定按钮！");
        return;
    }
    
    // 检查是否已达到循环次数上限
    if(scCurrentCycle >= scTotalCycles)
    {
        QMessageBox::information(this, "提示", QString("已达到设置的循环次数上限（%1 次），请点击'停止标定'结束标定流程").arg(scTotalCycles));
        // 禁用记录位置按钮
        if(ui->scRecordPosBtn)
            ui->scRecordPosBtn->setEnabled(false);
        // 更新状态显示
        ui->scStatusLabel->setText("标定完成，已达到上限");
        return;
    }

    if(ui->scNextStepBtn)
        ui->scNextStepBtn->setEnabled(true);
    // 启用记录位置按钮
    
    // 记录当前角度位置信息
    QDateTime currentTime = QDateTime::currentDateTime();
    QString currentTimeStr = currentTime.toString(Qt::ISODate);
    
    QString dataStr = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11")
            .arg(scCurrentCycle + 1)
            .arg(angleStr[0])
            .arg(angleStr[1])
            .arg(angleStr[2])
            .arg(angleStr[3])
            .arg(bodyPoseStr[0])
            .arg(bodyPoseStr[1])
            .arg(bodyPoseStr[2])
            .arg(velocityPoseStr[0])
            .arg(velocityPoseStr[1])
            .arg(velocityPoseStr[2]);

    scCalibrationData.append(dataStr);
    scCurrentCycle++;

    // 更新UI显示
    ui->scCurrentLabel->setText(QString::number(scCurrentCycle));
    
    // 检查是否已达到循环次数上限
    if(scCurrentCycle >= scTotalCycles)
    {
        ui->scStatusLabel->setText(QString("标定完成，已达到上限 (%1/%2)").arg(scCurrentCycle).arg(scTotalCycles));
        // 禁用记录位置按钮
        if(ui->scRecordPosBtn)
            ui->scRecordPosBtn->setEnabled(false);
        if(ui->scNextStepBtn)
            ui->scNextStepBtn->setEnabled(false);
        QMessageBox::information(this, "提示", QString("第 %1 次标定完成！已达到设置的循环次数上限（%2 次），标定流程结束").arg(scCurrentCycle).arg(scTotalCycles));
    }
    else
    {
        ui->scStatusLabel->setText(QString("标定步骤完成，等待记录下一个位置 (%1/%2)").arg(scCurrentCycle).arg(scTotalCycles));
        QMessageBox::information(this, "提示", QString("第 %1 次标定完成！可以继续调整位置后点击'记录位置'，或点击'停止标定'结束 (剩余 %2 次)").arg(scCurrentCycle).arg(scTotalCycles - scCurrentCycle));
    }
}


void mainUI::on_scSaveDataBtn_clicked()
{
    if(scCalibrationData.size() < 1)
    {
        QMessageBox::information(this, "提示", "当前标定数据为空，无法保存");
        return;
    }

    QString csvFile = QFileDialog::getSaveFileName(this, "保存标定数据", "", "CSV文件 (*.csv)");
    if(csvFile.isEmpty())
        return;

    QFile file(csvFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "错误", "无法创建文件！");
        return;
    }

    QTextStream out(&file);
    out << tr("Cycle,") << tr("angleRF,") << tr("angleLF,") << tr("angleLB,") << tr("angleRB,")
        << tr("bodyPoseX,") << tr("bodyPoseY,") << tr("bodyPoseZ,")
        << tr("velocityPoseX,") << tr("velocityPoseY,") << tr("velocityPoseZ,") << "\n";
    
    for(const auto &data : scCalibrationData)
    {
        out << data << "\n";
    }
    file.close();
    
    QMessageBox::information(this, "提示", "标定数据保存成功");
}

void mainUI::on_scClearDataBtn_clicked()
{
    if(scIsRunning)
    {
        QMessageBox::warning(this, "警告", "标定正在进行中，无法清空数据！");
        return;
    }

    int ret = QMessageBox::question(this, "确认", "确定要清空所有标定数据吗？", 
                                   QMessageBox::Yes | QMessageBox::No);
    if(ret == QMessageBox::Yes)
    {
        scCalibrationData.clear();
        scCurrentCycle = 0;
        ui->scCurrentLabel->setText("0");
        ui->scStatusLabel->setText("待机");
        QMessageBox::information(this, "提示", "数据已清空");
    }
}

// void mainUI::scProcessCalibrationData()
// {
//     // 现在这个函数不再用于记录数据，因为我们使用手动按钮来记录
//     // 仅保留基本的定时器停止逻辑，以防定时器仍被意外触发
//     if(scTimer->isActive())
//     {
//         scTimer->stop();
//     }
    
//     // 如果定时器被意外触发，可以提示用户使用手动记录按钮
//     if(scIsRunning)
//     {
//         QMessageBox::information(this, "提示", "请使用'记录位置'按钮来手动记录标定数据！");
//     }
// }

// 拖动模式相关函数实现
void mainUI::on_dragModeStartBtn_pressed()
{
    if(!MySerialPort->isOpen())
    {
        QMessageBox::warning(this, "警告", "请先打开串口！");
        return;
    }
    
    if(scIsRunning)
    {
        QMessageBox::warning(this, "警告", "标定正在进行中，无法进入拖动模式！");
        return;
    }
    
    // if(dragModeActive)
    // {
    //     QMessageBox::information(this, "提示", "拖动模式已经激活！");
    //     return;
    // }
    
    // 激活拖动模式
    dragModeActive = true;
    dragPositionIndex = 0;
    
    // 发送恒力矩模式指令，让电机进入拖动模式
    QByteArray TxData;
    TxData = "#CT5";  // 发送LF电机收绳指令（假设2对应LF电机）
    MySerialPort->write(TxData);
    
    // 更新UI状态
    if(ui->dragModeStartBtn)
        ui->dragModeStartBtn->setEnabled(false);
    if(ui->dragModeStopBtn)
        ui->dragModeStopBtn->setEnabled(true);
    if(ui->dragRecordPos1Btn)
        ui->dragRecordPos1Btn->setEnabled(true);
    if(ui->dragRecordPos2Btn)
        ui->dragRecordPos2Btn->setEnabled(true);
    
    // QMessageBox::information(this, "提示", "拖动模式已激活！现在可以手动拖动机器人。\n请拖动机器人到第一个位置后，点击'记录位置1'按钮。");
}


void mainUI::on_dragModeStopBtn_clicked()
{
    if(!dragModeActive)
    {
        QMessageBox::information(this, "提示", "拖动模式未激活！");
        return;
    }
    
    // 退出拖动模式
    dragModeActive = false;
    
    // 发送停止指令
    QByteArray TxData;
    TxData = "SMode";  // 发送停止指令
    MySerialPort->write(TxData);
    
    // 更新UI状态
    if(ui->dragModeStartBtn)
        ui->dragModeStartBtn->setEnabled(true);
    if(ui->dragModeStopBtn)
        ui->dragModeStopBtn->setEnabled(false);
    if(ui->dragRecordPos1Btn)
        ui->dragRecordPos1Btn->setEnabled(false);
    if(ui->dragRecordPos2Btn)
        ui->dragRecordPos2Btn->setEnabled(false);
    
    QMessageBox::information(this, "提示", "拖动模式已退出！");
}

void mainUI::on_dragRecordPos1Btn_clicked()
{
    if(!dragModeActive)
    {
        QMessageBox::warning(this, "警告", "请先激活拖动模式！");
        return;
    }
    
    // 记录当前位置到位置1
    for(int i = 0; i < 4; i++)
    {
        dragPos1[i] = angle[i];  // 使用当前读取到的角度值
    }
    for(int i = 0; i < 3; i++)
    {
        dragPose1[i] = bodyPose[i];  // 使用当前读取到的姿态值
    }
    
    // 更新UI显示（如果有显示控件）
    if(ui->dragPos1Label)
    {
        QString posStr = QString("位置1 - 电机角度: [%1, %2, %3, %4] | 姿态: [%5, %6, %7]")
            .arg(dragPos1[0], 0, 'f', 3)
            .arg(dragPos1[1], 0, 'f', 3)
            .arg(dragPos1[2], 0, 'f', 3)
            .arg(dragPos1[3], 0, 'f', 3)
            .arg(dragPose1[0], 0, 'f', 3)
            .arg(dragPose1[1], 0, 'f', 3)
            .arg(dragPose1[2], 0, 'f', 3);
        ui->dragPos1Label->setText(posStr);
    }
    
    QMessageBox::information(this, "提示", 
        QString("位置1已记录！\n电机角度: [%1, %2, %3, %4]\n姿态: [%5, %6, %7]\n\n请拖动机器人到第二个位置后，点击'记录位置2'按钮。")
        .arg(dragPos1[0], 0, 'f', 3)
        .arg(dragPos1[1], 0, 'f', 3)
        .arg(dragPos1[2], 0, 'f', 3)
        .arg(dragPos1[3], 0, 'f', 3)
        .arg(dragPose1[0], 0, 'f', 3)
        .arg(dragPose1[1], 0, 'f', 3)
        .arg(dragPose1[2], 0, 'f', 3));
}

void mainUI::on_dragRecordPos2Btn_clicked()
{
    if(!dragModeActive)
    {
        QMessageBox::warning(this, "警告", "请先激活拖动模式！");
        return;
    }
    
    // 记录当前位置到位置2
    for(int i = 0; i < 4; i++)
    {
        dragPos2[i] = angle[i];  // 使用当前读取到的角度值
    }
    for(int i = 0; i < 3; i++)
    {
        dragPose2[i] = bodyPose[i];  // 使用当前读取到的姿态值
    }
    
    // 更新UI显示（如果有显示控件）
    if(ui->dragPos2Label)
    {
        QString posStr = QString("位置2 - 电机角度: [%1, %2, %3, %4] | 姿态: [%5, %6, %7]")
            .arg(dragPos2[0], 0, 'f', 3)
            .arg(dragPos2[1], 0, 'f', 3)
            .arg(dragPos2[2], 0, 'f', 3)
            .arg(dragPos2[3], 0, 'f', 3)
            .arg(dragPose2[0], 0, 'f', 3)
            .arg(dragPose2[1], 0, 'f', 3)
            .arg(dragPose2[2], 0, 'f', 3);
        ui->dragPos2Label->setText(posStr);
    }
    
    QMessageBox::information(this, "提示", 
        QString("位置2已记录！\n电机角度: [%1, %2, %3, %4]\n姿态: [%5, %6, %7]\n\n两个位置均已记录完成！可以退出拖动模式或继续使用。")
        .arg(dragPos2[0], 0, 'f', 3)
        .arg(dragPos2[1], 0, 'f', 3)
        .arg(dragPos2[2], 0, 'f', 3)
        .arg(dragPos2[3], 0, 'f', 3)
        .arg(dragPose2[0], 0, 'f', 3)
        .arg(dragPose2[1], 0, 'f', 3)
        .arg(dragPose2[2], 0, 'f', 3));
}

void mainUI::on_dragClearBtn_clicked()
{
    if(dragModeActive)
    {
        QMessageBox::warning(this, "警告", "拖动模式正在运行中，无法清空位置记录！\n请先退出拖动模式。");
        return;
    }
    
    int ret = QMessageBox::question(this, "确认", "确定要清空所有拖动位置记录吗？", 
                                   QMessageBox::Yes | QMessageBox::No);
    if(ret == QMessageBox::Yes)
    {
        // 清空位置记录
        for(int i = 0; i < 4; i++)
        {
            dragPos1[i] = 0.0f;
            dragPos2[i] = 0.0f;
        }
        for(int i = 0; i < 3; i++)
        {
            dragPose1[i] = 0.0f;
            dragPose2[i] = 0.0f;
        }
        
        // 更新UI显示
        if(ui->dragPos1Label)
            ui->dragPos1Label->setText("位置1: 未记录");
        if(ui->dragPos2Label)
            ui->dragPos2Label->setText("位置2: 未记录");
        
        QMessageBox::information(this, "提示", "拖动位置记录已清空");
    }
}

// 保存拖动模式记录的位置到CSV文件
void mainUI::on_dragSaveDataBtn_clicked()
{
    if(!dragModeActive)
    {
        QMessageBox::warning(this, "警告", "请先激活拖动模式并记录位置！");
        return;
    }

    // 检查是否有数据可以保存
    bool hasData = false;
    for(int i = 0; i < 4; i++)
    {
        if(dragPos1[i] != 0.0f || dragPos2[i] != 0.0f)
        {
            hasData = true;
            break;
        }
    }

    if(!hasData)
    {
        QMessageBox::warning(this, "警告", "没有可保存的数据！请先记录位置。");
        return;
    }

    // 获取保存路径
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存位置数据"), ".", tr("CSV Files (*.csv);;All Files (*)"));
    if(fileName.isEmpty())
        return;

    // 创建文件
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "警告", "无法创建文件！");
        return;
    }

    // 写入数据
    QTextStream out(&file);
    // 写入表头
    out << "position,angleRF,angleLF,angleLB,angleRB,bodyPoseX,bodyPoseY,bodyPoseZ" << Qt::endl;

    // 写入位置1数据
    out << "pos1," << dragPos1[0] << "," << dragPos1[1] << "," << dragPos1[2] << "," << dragPos1[3] << ","
        << dragPose1[0] << "," << dragPose1[1] << "," << dragPose1[2] << Qt::endl;
    
    // 写入位置2数据
    out << "pos2," << dragPos2[0] << "," << dragPos2[1] << "," << dragPos2[2] << "," << dragPos2[3] << ","
        << dragPose2[0] << "," << dragPose2[1] << "," << dragPose2[2] << Qt::endl;

    // 关闭文件
    file.close();

    QMessageBox::information(this, "提示", "位置数据已成功保存！");
}

// 测量初始绳长模式相关函数实现
// void mainUI::on_measureRopeLengthBtn_pressed()
// {
//     if(!MySerialPort->isOpen())
//     {
//         QMessageBox::warning(this, "警告", "请先打开串口！");
//         return;
//     }
    
//     if(scIsRunning || dragModeActive)
//     {
//         QMessageBox::warning(this, "警告", "其他模式正在运行中，无法进入测量初始绳长模式！");
//         return;
//     }
    
//     if(measureRopeLengthActive)
//     {
//         QMessageBox::information(this, "提示", "测量初始绳长模式已经激活！");
//         return;
//     }
    
//     // 激活测量初始绳长模式
//     measureRopeLengthActive = true;
    
//     // 更新UI状态
//     ui->modeStackedWidget->setCurrentWidget(ui->MeasureRopeModePage);

//     QByteArray TxData;
//     TxData = "CTMode";  // 切换到恒力矩模式，便于手动拖动
//     MySerialPort->write(TxData);
    
//     QMessageBox::information(this, "提示", "测量初始绳长模式已激活！现在可以控制各电机收绳并记录位置。");
// }

void mainUI::on_measureRopeLengthStopBtn_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::information(this, "提示", "测量初始绳长模式未激活！");
        return;
    }
    
    // 发送停止指令
    QByteArray TxData;
    TxData = "#CT0";  // 发送停止指令
    MySerialPort->write(TxData);
    
    QMessageBox::information(this, "提示", "测量初始绳长模式已退出！");
}

// 电机收绳按钮实现
void mainUI::on_tightRopeBtn_Measure_RF_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量初始绳长模式！");
        return;
    }
    
    // 发送RF电机收绳指令
    QByteArray TxData;
    TxData = "#CT1";  // 发送RF电机收绳指令（假设1对应RF电机）
    MySerialPort->write(TxData);
}

void mainUI::on_tightRopeBtn_Measure_LF_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量初始绳长模式！");
        return;
    }
    
    // 发送LF电机收绳指令
    QByteArray TxData;
    TxData = "#CT2";  // 发送LF电机收绳指令（假设2对应LF电机）
    MySerialPort->write(TxData);
}

void mainUI::on_tightRopeBtn_Measure_LB_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量初始绳长模式！");
        return;
    }
    
    // 发送LB电机收绳指令
    QByteArray TxData;
    TxData = "#CT3";  // 发送LB电机收绳指令（假设3对应LB电机）
    MySerialPort->write(TxData);
}

void mainUI::on_tightRopeBtn_Measure_RB_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量初始绳长模式！");
        return;
    }
    
    // 发送RB电机收绳指令
    QByteArray TxData;
    TxData = "#CT4";  // 发送RB电机收绳指令（假设4对应RB电机）
    MySerialPort->write(TxData);
}

// 记录初始位置按钮实现
void mainUI::on_recordInitialBtn_RF_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量初始绳长模式！");
        return;
    }
    
    // 记录当前RF电机角度作为初始位置
    initialRopeAngle[0] = angle[0];
    
    // 更新UI显示
    ui->initialAngleLabel_RF->setText(QString("初始角度: %1").arg(initialRopeAngle[0], 0, 'f', 3));
    
    QMessageBox::information(this, "提示", QString("RF电机初始位置已记录！角度值: %1").arg(initialRopeAngle[0], 0, 'f', 3));
}

void mainUI::on_recordInitialBtn_LF_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量初始绳长模式！");
        return;
    }
    
    // 记录当前LF电机角度作为初始位置
    initialRopeAngle[1] = angle[1];
    
    // 更新UI显示
    ui->initialAngleLabel_LF->setText(QString("初始角度: %1").arg(initialRopeAngle[1], 0, 'f', 3));
    
    QMessageBox::information(this, "提示", QString("LF电机初始位置已记录！角度值: %1").arg(initialRopeAngle[1], 0, 'f', 3));
}

void mainUI::on_recordInitialBtn_LB_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量初始绳长模式！");
        return;
    }
    
    // 记录当前LB电机角度作为初始位置
    initialRopeAngle[2] = angle[2];
    
    // 更新UI显示
    ui->initialAngleLabel_LB->setText(QString("初始角度: %1").arg(initialRopeAngle[2], 0, 'f', 3));
    
    QMessageBox::information(this, "提示", QString("LB电机初始位置已记录！角度值: %1").arg(initialRopeAngle[2], 0, 'f', 3));
}

void mainUI::on_recordInitialBtn_RB_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量初始绳长模式！");
        return;
    }
    
    // 记录当前RB电机角度作为初始位置
    initialRopeAngle[3] = angle[3];
    
    // 更新UI显示
    ui->initialAngleLabel_RB->setText(QString("初始角度: %1").arg(initialRopeAngle[3], 0, 'f', 3));
    
    QMessageBox::information(this, "提示", QString("RB电机初始位置已记录！角度值: %1").arg(initialRopeAngle[3], 0, 'f', 3));
}

// 记录当前位置按钮实现
void mainUI::on_recordCurrentBtn_RF_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量初始绳长模式！");
        return;
    }
    
    // 记录当前RF电机角度作为当前位置

    float temp = angle[0];
    currentRopeAngle[0] = temp;
    
    // 更新UI显示
    ui->currentAngleLabel_RF->setText(QString("现在角度: %1").arg(currentRopeAngle[0], 0, 'f', 3));
    
    QMessageBox::information(this, "提示", QString("RF电机当前位置已记录！角度值: %1").arg(currentRopeAngle[0], 0, 'f', 3));
}

void mainUI::on_recordCurrentBtn_LF_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量初始绳长模式！");
        return;
    }
    
    // 记录当前LF电机角度作为当前位置
    float temp = angle[1];
    currentRopeAngle[1] = temp;
    
    // 更新UI显示
    ui->currentAngleLabel_LF->setText(QString("现在角度: %1").arg(currentRopeAngle[1], 0, 'f', 3));
    
    QMessageBox::information(this, "提示", QString("LF电机当前位置已记录！角度值: %1").arg(currentRopeAngle[1], 0, 'f', 3));
}

void mainUI::on_recordCurrentBtn_LB_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量初始绳长模式！");
        return;
    }
    
    // 记录当前LB电机角度作为当前位置
    float temp = angle[2];
    currentRopeAngle[2] = temp;
    
    // 更新UI显示
    ui->currentAngleLabel_LB->setText(QString("现在角度: %1").arg(currentRopeAngle[2], 0, 'f', 3));
    
    QMessageBox::information(this, "提示", QString("LB电机当前位置已记录！角度值: %1").arg(currentRopeAngle[2], 0, 'f', 3));
}

void mainUI::on_recordCurrentBtn_RB_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量初始绳长模式！");
        return;
    }
    
    // 记录当前RB电机角度作为当前位置
    float temp = angle[3];
    currentRopeAngle[3] = temp;
    
    // 更新UI显示
    ui->currentAngleLabel_RB->setText(QString("现在角度: %1").arg(currentRopeAngle[3], 0, 'f', 3));
    
    QMessageBox::information(this, "提示", QString("RB电机当前位置已记录！角度值: %1").arg(currentRopeAngle[3], 0, 'f', 3));
}

void mainUI::on_exportDataBtn_clicked()
{
    if(!measureRopeLengthActive)
    {
        QMessageBox::warning(this, "警告", "请先激活测量绳长模式并记录位置！");
        return;
    }

    // 检查是否有数据可以保存
    bool hasData = false;
    for(int i = 0; i < 4; i++)
    {
        if(initialRopeAngle[i] != 0.0f || currentRopeAngle[i] != 0.0f)
        {
            hasData = true;
            break;
        }
    }

    if(!hasData)
    {
        QMessageBox::warning(this, "警告", "没有可保存的数据！请先记录位置。");
        return;
    }

    // 获取保存路径
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存位置数据"), ".", tr("CSV Files (*.csv);;All Files (*)"));
    if(fileName.isEmpty())
        return;

    // 创建文件
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "警告", "无法创建文件！");
        return;
    }

    // 写入数据
    QTextStream out(&file);
    // 写入表头
    out << "angleRF_init," << "angleLF_init," << "angleLB_init," << "angleRB_init,"<< Qt::endl;

    // 写入位置1数据
    out << initialRopeAngle[0] << "," << initialRopeAngle[1] << "," << initialRopeAngle[2] << "," << initialRopeAngle[3] << Qt::endl;

    out << "angleRF_curr," << "angleLF_curr,"<< "angleLB_curr," << "angleRB_curr," << Qt::endl;

    // 写入位置2数据
    out << currentRopeAngle[0] << "," << currentRopeAngle[1] << "," << currentRopeAngle[2] << "," << currentRopeAngle[3] << Qt::endl;

    // 关闭文件
    file.close();

    QMessageBox::information(this, "提示", "位置数据已成功保存！");

}

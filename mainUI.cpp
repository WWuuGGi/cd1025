#include "mainUI.h"
#include "ui_mainUI.h"

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
    scTimer = new QTimer();
    scCurrentCycle = 0;
    scTotalCycles = 10;
    scIsRunning = false;

    poseXChartInit();
    connect(drawLineTimer, SIGNAL(timeout()), this, SLOT(DrawLine()));
    connect(scTimer, SIGNAL(timeout()), this, SLOT(scProcessCalibrationData()));
}

mainUI::~mainUI()
{
    // 若串口打开，关闭串口
    if(MySerialPort->isOpen())
    {
        MySerialPort->close();
    }
    delete ui;
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
float angle[4], bodyPose[3], velocityPose[3];
uint8_t trj_index = 0;
QString angleStr[4], bodyPoseStr[3], velocityPoseStr[3];

uint8_t Res;  //读取到的每一字节数据
int16_t USART_RX_STA = 0; //接收状态标志
#define USART_REC_LEN 41 //定义最大接收字节数
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
        DataProcess(USART_RX_BUF);
    }

    rxbuff.clear();
}

// 对数据帧进行解析
void mainUI::DataProcess(uint8_t *data) //处理的数据不包含包头
{
    bodyPose[0] = ((float)((int32_t)(data[0] | (((uint32_t)data[1]) << 8) | (((uint32_t)data[2]) << 16) | (((uint32_t)data[3]) << 24)))) / 1000.0f;
    bodyPose[1] = ((float)((int32_t)(data[4] | (((uint32_t)data[5]) << 8) | (((uint32_t)data[6]) << 16) | (((uint32_t)data[7]) << 24)))) / 1000.0f;
    bodyPose[2] = ((float)((int32_t)(data[8] | (((uint32_t)data[9]) << 8) | (((uint32_t)data[10]) << 16) | (((uint32_t)data[11]) << 24)))) / 1000.0f;

    angle[0] = ((float)((int32_t)(data[12] | (((uint32_t)data[13]) << 8) | (((uint32_t)data[14]) << 16) | (((uint32_t)data[15]) << 24)))) / 1000.0f;
    angle[1] = ((float)((int32_t)(data[16] | (((uint32_t)data[17]) << 8) | (((uint32_t)data[18]) << 16) | (((uint32_t)data[19]) << 24)))) / 1000.0f;
    angle[2] = ((float)((int32_t)(data[20] | (((uint32_t)data[21]) << 8) | (((uint32_t)data[22]) << 16) | (((uint32_t)data[23]) << 24)))) / 1000.0f;
    angle[3] = ((float)((int32_t)(data[24] | (((uint32_t)data[25]) << 8) | (((uint32_t)data[26]) << 16) | (((uint32_t)data[27]) << 24)))) / 1000.0f;

    velocityPose[0] = ((float)((int32_t)(data[28] | (((uint32_t)data[29]) << 8) | (((uint32_t)data[30]) << 16) | (((uint32_t)data[31]) << 24)))) / 1000.0f;
    velocityPose[1] = ((float)((int32_t)(data[32] | (((uint32_t)data[33]) << 8) | (((uint32_t)data[34]) << 16) | (((uint32_t)data[35]) << 24)))) / 1000.0f;
    velocityPose[2] = ((float)((int32_t)(data[36] | (((uint32_t)data[37]) << 8) | (((uint32_t)data[38]) << 16) | (((uint32_t)data[39]) << 24)))) / 1000.0f;

    trj_index = data[40];

    for(int i=0; i<3; i++)
    {
        bodyPoseStr[i] = QString::number(bodyPose[i],'f',3);
        velocityPoseStr[i] = QString::number(velocityPose[i],'f',3);
    }

    for(int i=0; i<4; i++)
    {
        angleStr[i] = QString::number(angle[i],'f',3);
    }

    ui->angleRFEdit->setText(angleStr[0]);
    ui->angleLFEdit->setText(angleStr[1]);
    ui->angleLBEdit->setText(angleStr[2]);
    ui->angleRBEdit->setText(angleStr[3]);

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
    TxData = "Pose";
    MySerialPort->write(TxData);
}

void mainUI::on_endPoseZeroBtn_clicked()
{
    QByteArray TxData;
    TxData = "End";
    MySerialPort->write(TxData);
}

// 运动模式选择
void mainUI::on_SModeBtn_pressed()
{
    ui->modeStackedWidget->setCurrentWidget(ui->SModePage);
    QString TxData = "SMode";
    MySerialPort->write(TxData.toLocal8Bit());
}

void mainUI::on_TModeBtn_pressed()
{
    ui->modeStackedWidget->setCurrentWidget(ui->TModePage);
    QString TxData = "TMode";
    MySerialPort->write(TxData.toLocal8Bit());
}

void mainUI::on_RModeBtn_pressed()
{
    ui->modeStackedWidget->setCurrentWidget(ui->RModePage);
    QString TxData = "RMode";
    MySerialPort->write(TxData.toLocal8Bit());
}

void mainUI::on_SCModeBtn_pressed()
{
    ui->modeStackedWidget->setCurrentWidget(ui->SCModePage);
    QString TxData = "SCMode";
    MySerialPort->write(TxData.toLocal8Bit());
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

//夹爪命令
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
        QMessageBox::information(this, "提示", "标定正在进行中，请先停止当前标定！");
        return;
    }

    // 获取用户设置的循环次数
    scTotalCycles = ui->scCyclesSpinBox->value();
    scCurrentCycle = 0;
    scIsRunning = true;

    // 更新UI状态
    ui->scStatusLabel->setText("标定中...");
    ui->scCurrentLabel->setText(QString::number(scCurrentCycle));
    ui->scStartBtn->setEnabled(false);
    ui->scStopBtn->setEnabled(true);

    // 发送第一个标定指令
    QByteArray TxData;
    TxData = "#Tscmode";
    MySerialPort->write(TxData);

    // 启动定时器，等待位置到达后处理数据
    scTimer->start(6000); // 2秒后处理数据
}

void mainUI::on_scStopBtn_clicked()
{
    scIsRunning = false;
    scTimer->stop();

    // 更新UI状态
    ui->scStatusLabel->setText("已停止");
    ui->scStartBtn->setEnabled(true);
    ui->scStopBtn->setEnabled(false);

    // 发送停止指令
    QByteArray TxData;
    TxData = "#SCSTOP";
    MySerialPort->write(TxData);
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

void mainUI::scProcessCalibrationData()
{
    if(!scIsRunning)
    {
        scTimer->stop();
        return;
    }

    // 停止定时器
    scTimer->stop();

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

    // 检查是否完成所有循环
    if(scCurrentCycle >= scTotalCycles)
    {
        // 标定完成
        scIsRunning = false;
        ui->scStatusLabel->setText("标定完成");
        ui->scStartBtn->setEnabled(true);
        ui->scStopBtn->setEnabled(false);
        
        QMessageBox::information(this, "提示", QString("标定完成！共完成 %1 次循环").arg(scTotalCycles));
        return;
    }

    // 继续下一个循环
    QByteArray TxData;
    TxData = "#Tscmode";
    MySerialPort->write(TxData);

    // 重新启动定时器
    scTimer->start(6000); // 2秒后处理下一个位置的数据
}

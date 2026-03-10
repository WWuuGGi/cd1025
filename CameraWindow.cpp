#include "CameraWindow.h"
#include <QDebug>
#include <QDateTime>

CameraWindow::CameraWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Qt5 摄像头示例");
    resize(800, 600);

    // 预览控件
    m_viewfinder = new QCameraViewfinder(this);
    m_viewfinder->setMinimumSize(640, 480);

    // 按钮
    m_btnOpen = new QPushButton("打开摄像头");
    m_btnCapture = new QPushButton("拍照");
    m_btnClose = new QPushButton("关闭摄像头");

    // 拍照结果显示
    m_labelPhoto = new QLabel;
    m_labelPhoto->setFixedSize(200, 150);
    m_labelPhoto->setStyleSheet("border: 1px solid gray;");

    // 布局
    auto vLayout = new QVBoxLayout(this);
    vLayout->addWidget(m_viewfinder, 1);

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(m_btnOpen);
    hLayout->addWidget(m_btnCapture);
    hLayout->addWidget(m_btnClose);
    hLayout->addWidget(m_labelPhoto);
    vLayout->addLayout(hLayout);

    // 信号槽
    connect(m_btnOpen, &QPushButton::clicked, this, &CameraWindow::onOpenCamera);
    connect(m_btnCapture, &QPushButton::clicked, this, &CameraWindow::onCaptureImage);
    connect(m_btnClose, &QPushButton::clicked, this, &CameraWindow::onCloseCamera);
}

void CameraWindow::onOpenCamera()
{
    if (m_camera) return; // 已经打开

    m_camera = new QCamera(this);
    m_camera->setViewfinder(m_viewfinder);

    m_imageCapture = new QCameraImageCapture(m_camera, this);
    connect(m_imageCapture, &QCameraImageCapture::imageCaptured,
            this, &CameraWindow::onImageCaptured);

    m_camera->start();
    qDebug() << "摄像头已打开";
}

void CameraWindow::onCaptureImage()
{
    if (!m_imageCapture) return;

    QString filename = QString("photo_%1.jpg")
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    m_imageCapture->capture(filename);
    qDebug() << "已拍照保存到:" << filename;
}

void CameraWindow::onCloseCamera()
{
    if (m_camera) {
        m_camera->stop();
        delete m_camera;
        m_camera = nullptr;

        delete m_imageCapture;
        m_imageCapture = nullptr;

        qDebug() << "摄像头已关闭";
    }
}

void CameraWindow::onImageCaptured(int, const QImage &preview)
{
    m_labelPhoto->setPixmap(QPixmap::fromImage(preview).scaled(m_labelPhoto->size(),
                                                               Qt::KeepAspectRatio,
                                                               Qt::SmoothTransformation));
}

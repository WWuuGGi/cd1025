#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#pragma once

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class CameraWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWindow(QWidget *parent = nullptr);

private slots:
    void onOpenCamera();
    void onCaptureImage();
    void onCloseCamera();
    void onImageCaptured(int id, const QImage &preview);

private:
    QCamera *m_camera = nullptr;
    QCameraViewfinder *m_viewfinder = nullptr;
    QCameraImageCapture *m_imageCapture = nullptr;

    QPushButton *m_btnOpen;
    QPushButton *m_btnCapture;
    QPushButton *m_btnClose;
    QLabel *m_labelPhoto;
};



#endif // CAMERAWINDOW_H

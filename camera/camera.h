#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QThread>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QCamera>
#include <QCameraViewfinder>

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);

    void start();
    void stop();

signals:
    void newFrameAvailable(const QImage&);

private slots:
    void callback(QVideoFrame frame);
    void processFrame(const QImage& image);

private:
    QThread thread_;
    QCamera* camera_;
    QCameraViewfinder* viewfinder_;
    QGraphicsVideoItem* videoItem_;
    QGraphicsScene* scene_;
    bool running_;

};

#endif // CAMERA_H

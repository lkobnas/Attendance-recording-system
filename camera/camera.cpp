#include "camera.h"

Camera::Camera(QObject *parent) : QObject(parent), running_(false)
{
    // Create a new QGraphicsScene to hold the video
    scene_ = new QGraphicsScene(this);

    // Create a new QCamera and QCameraViewfinder
    camera_ = new QCamera(this);
    viewfinder_ = new QCameraViewfinder(this);

    // Add the viewfinder to the scene as a QGraphicsVideoItem
    videoItem_ = new QGraphicsVideoItem;
    videoItem_->setSize(QSizeF(640, 480)); // Set the size of the video
    videoItem_->setAspectRatioMode(Qt::KeepAspectRatio); // Keep the aspect ratio of the video
    scene_->addItem(videoItem_);
    viewfinder_->setAspectRatioMode(Qt::KeepAspectRatio); // Keep the aspect ratio of the viewfinder
    viewfinder_->setVisible(false); // Hide the viewfinder

    // Set the viewfinder as the viewfinder for the camera
    camera_->setViewfinder(viewfinder_);

    // Connect the signal from the viewfinder to the callback slot
    connect(viewfinder_, SIGNAL(newFrameAvailable(QVideoFrame)), this, SLOT(callback(QVideoFrame)));

    // Start the thread
    moveToThread(&thread_);
    thread_.start();
}

void Camera::start()
{
    // Start the camera
    camera_->start();
    running_ = true;
}

void Camera::stop()
{
    // Stop the camera
    camera_->stop();
    running_ = false;

    // Remove the video item from the scene
    scene_->removeItem(videoItem_);
}

void Camera::callback(QVideoFrame frame)
{
    // Convert the QVideoFrame to a QImage
    QImage image = QImage(frame.bits(), frame.width(), frame.height(), QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));

    // Emit the newFrameAvailable signal with the image
    emit newFrameAvailable(image);
}

void Camera::processFrame(const QImage& image)
{
    // Set the image as the pixmap for the video item
    videoItem_->setPixmap(QPixmap::fromImage(image));

    // Update the graphics scene
    scene_->update();
}

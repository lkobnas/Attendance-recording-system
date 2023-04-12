#ifndef CAMERA_CAPTURE_H
#define CAMERA_CAPTURE_H

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <opencv2/opencv.hpp>

class CameraCapture {
public:
    CameraCapture();
    ~CameraCapture();

    std::string imageToString(const cv::Mat& frame);
    cv::Mat stringToImage(const std::string& base64_str);
    
    void run();
    void stop();

private:
    void captureFrame();
    void saveImage(const cv::Mat& frame, int& image_count);

    cv::VideoCapture cap;
    cv::Mat frame;
    std::thread capture_thread;
    std::mutex frame_mutex;
    std::atomic<bool> is_running;

};

#endif // CAMERA_CAPTURE_H

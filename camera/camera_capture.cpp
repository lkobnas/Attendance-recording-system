#include "camera_capture.h"
#include <cppcodec/base64_rfc4648.hpp> 

CameraCapture::CameraCapture() : is_running(true) {
    if (!cap.open(0)) {
        std::cerr << "无法打开摄像头。" << std::endl;
        std::exit(-1);
    }
    capture_thread = std::thread(&CameraCapture::captureFrame, this);
}

CameraCapture::~CameraCapture() {
    stop();
}

void CameraCapture::captureFrame() {
    while (is_running) {
        cv::Mat temp_frame;
        cap >> temp_frame;
        if (temp_frame.empty()) {
            std::cerr << "无法捕获图像。" << std::endl;
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(frame_mutex);
            frame = temp_frame.clone();
        }
    }
}

// void CameraCapture::saveImage(const cv::Mat& frame, int& image_count) {
//     std::string filename = "capture_" + std::to_string(image_count) + ".png";
//     cv::imwrite(filename, frame);
//     std::cout << "已保存当前图像: " << filename << std::endl;
//     image_count++;
// }
void CameraCapture::saveImage(const cv::Mat& frame, int& image_count) {
    std::string filename = "capture_" + std::to_string(image_count) + ".png";
    cv::imwrite(filename, frame);
    std::cout << "已保存当前图像: " << filename << std::endl;

    // 将图像转换为字符串
    std::string image_str = imageToString(frame);
    std::cout<<image_str<<std::endl;

    // 将字符串转换回图像
    cv::Mat restored_image = stringToImage(image_str);

    // 显示恢复的图像
    cv::imshow("恢复的图像", restored_image);

    image_count++;
}

std::string CameraCapture::imageToString(const cv::Mat& frame) {
    std::vector<uchar> buffer;
    cv::imencode(".png", frame, buffer);
    return cppcodec::base64_rfc4648::encode(buffer);
}

cv::Mat CameraCapture::stringToImage(const std::string& base64_str) {
    std::vector<uchar> buffer = cppcodec::base64_rfc4648::decode<std::vector<uchar>>(base64_str);
    return cv::imdecode(buffer, cv::IMREAD_COLOR);
}

void CameraCapture::run() {
    int image_count = 0;
    while (true) {
        {
            std::lock_guard<std::mutex> lock(frame_mutex);
            if (!frame.empty()) {
                cv::imshow("摄像头图像", frame);
            }
        }

        int key = cv::waitKey(1);
        // 按下'q'键退出程序
        if (key == 'q') {
            break;
        }
        // 按下'c'键保存当前图像
        else if (key == 'c') {
            saveImage(frame, image_count);
        }
    }
}

void CameraCapture::stop() {
    if (is_running) {
        is_running = false;
        if (capture_thread.joinable()) {
            capture_thread.join();
        }
        cap.release();
        cv::destroyAllWindows();
    }
}

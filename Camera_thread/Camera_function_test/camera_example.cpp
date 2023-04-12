#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <opencv2/opencv.hpp>

std::mutex frame_mutex;
std::atomic<bool> is_running(true);

void captureFrame(cv::VideoCapture& cap, cv::Mat& frame) {
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

void saveImage(const cv::Mat& frame, int& image_count) {
    std::string filename = "capture_" + std::to_string(image_count) + ".png";
    cv::imwrite(filename, frame);
    std::cout << "已保存当前图像: " << filename << std::endl;
    image_count++;
}

int main() {
    cv::VideoCapture cap(0); // 打开默认摄像头
    if (!cap.isOpened()) {
        std::cerr << "无法打开摄像头。" << std::endl;
        return -1;
    }

    cv::Mat frame;
    std::thread capture_thread(captureFrame, std::ref(cap), std::ref(frame));

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

    is_running = false;
    capture_thread.join();
    cap.release();
    cv::destroyAllWindows();

    return 0;
}

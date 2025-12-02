#include <opencv2/opencv.hpp>
#include <iostream>

class PinCounter {
public:
    PinCounter(int thresholdValue = 50, double minArea = 1000.0)
        : thresholdValue(thresholdValue), minArea(minArea) {
    }

    int countPins(const cv::Mat& frame, cv::Mat& output) {
        cv::Mat gray, thresh;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::threshold(gray, thresh, thresholdValue, 255, cv::THRESH_BINARY_INV);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        int pinCount = 0;
        output = frame.clone();

        for (const auto& contour : contours) {
            double area = cv::contourArea(contour);
            double perimeter = cv::arcLength(contour, true);
            double circularity = 4 * CV_PI * area / (perimeter * perimeter);
            if (area < minArea || area > 2500.0) continue;
            if (circularity < 0.4) continue;
            pinCount++;
            cv::Rect box = cv::boundingRect(contour);
            cv::rectangle(output, box, cv::Scalar(0, 255, 0), 2);
        }

        cv::putText(output, "Pins Down: " + std::to_string(pinCount),
            cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1,
            cv::Scalar(255, 0, 0), 2);

        return pinCount;
    }

private:
    int thresholdValue;
    double minArea;
};
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
class PinCounter {
public:
    PinCounter(int thresholdValue = 50, double minArea = 1000.0)
        : thresholdValue(thresholdValue), minArea(minArea) {
    }

    int countPins(const cv::Mat& frame, cv::Mat& output) {
        Mat gray, thresh;
        cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        threshold(gray, thresh, thresholdValue, 255, cv::THRESH_BINARY_INV);

        std::vector<std::vector<cv::Point>> contours;
        findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        int pinCount = 0;
        output = frame.clone();

        for (const auto& contour : contours) {
            double area = contourArea(contour);
            double perimeter = arcLength(contour, true);
            double circularity = 4 * CV_PI * area / (perimeter * perimeter);
            if (area < minArea || area > 3000.0) continue;
            if (circularity < 0.6) continue;
            pinCount++;
            Rect box = boundingRect(contour);
            rectangle(output, box, cv::Scalar(0, 255, 0), 2);
        }

        putText(output, "Pins Down: " + std::to_string(pinCount),
            Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1,
            Scalar(255, 0, 0), 2);
        return pinCount;
    }

private:
    int thresholdValue;
    double minArea;
};
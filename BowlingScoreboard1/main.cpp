#include <iostream>
#include "ScoringEngine.cpp"
#include "PinCounter.cpp"
#include <opencv2/opencv.hpp>

using namespace std;

int main() {
    ScoringEngine game;

    cout << "Welcome to the Automatic Bowling Scoreboard!!!" << endl;

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Error: Could not open camera." << endl;
        return -1;
    }

    PinCounter counter(80, 1000.0); // tuned for black dots on board
    cv::Mat frame, output;
    int prevRoll = 0;
    int lastFrame = game.currentFrame();
    int lastRollCount = 0;

    while (!game.isFinish()) {
        char cmd;
        cout << "Enter command (c = capture the pins, q = quit the game): ";
        cin >> cmd;

        if (cmd == 'c') {
            cap >> frame;
            if (frame.empty()) continue;

            int frameNum = game.currentFrame();
            int currentRollCount = game.getFrame(frameNum - 1).rollCount();

            // Reset prevRoll if frame advanced
            if (lastFrame != frameNum) {
                prevRoll = 0;
                lastFrame = frameNum;
                lastRollCount = 0;
            }

            if (frameNum == 10) {
                if (currentRollCount > lastRollCount) {
                    prevRoll = 0;
                    lastRollCount = currentRollCount;
                }
            }


            int pinsDown = counter.countPins(frame, output);
            int pinsHitThisRoll = pinsDown - prevRoll;

            cout << "Pins hit this roll: " << pinsHitThisRoll << endl;

            int frameIndex = game.roll(pinsHitThisRoll);

            cout << "Captured: " << pinsDown << " pins down in Frame "
                << (frameIndex + 1) << endl;
            cout << "Current score: " << game.score() << endl;

            prevRoll = pinsDown;
        }

        if (cmd == 'q') break;
    }

    cout << "Game complete!!! Your total score is " << game.score() << endl;

    cap.release();
    cv::destroyAllWindows();
    return 0;
}

// Test PinCounter independently, showing dots on board.
/*int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    PinCounter counter(80, 1000.0); // adjust threshold and area as needed
    cv::Mat frame, output;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        int dots = counter.countPins(frame, output);
        cv::imshow("Dot Detection", output);

        if (cv::waitKey(1) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}*/

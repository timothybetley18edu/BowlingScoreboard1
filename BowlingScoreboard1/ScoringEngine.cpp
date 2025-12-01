#include <iostream>
#include <vector>
#include "Frame.cpp"

using namespace std;

class ScoringEngine {
private:
	vector<Frame> frames;

public:
	ScoringEngine() {
		for(int i = 0; i < 9; i++) {
			frames.emplace_back(false);
		}
		frames.emplace_back(true); // Tenth frame
	}
	
	int roll(int pins) {
		for (size_t i = 0; i < frames.size(); ++i) {
			if (!frames[i].isComplete()) {
				frames[i].addRoll(pins);
				return i; // Return the frame index where the roll was added
			}
		}
		return -1; // All frames are complete
	}

	int score() {
		int total = 0;
		for (size_t i = 0; i < frames.size(); ++i) {
			total += frames[i].getScore();
			if (i < 9) {
				if (frames[i].isStrike()) {
					total += strikeBonus(i);
				}
				else if (frames[i].isSpare()) {
					total += spareBonus(i);
				}
			}
		}
		return total;
	}

	bool isFinish() {
		for (const auto& frame : frames) {
			if (!frame.isComplete()) return false;
		}
		return true;
	}

	int currentFrame() const {
		for (size_t i = 0; i < frames.size(); ++i) {
			if (!frames[i].isComplete()) {
				return static_cast<int>(i + 1); // 1-based frame number
			}
		}
		return 10; // if all complete, we’re in the 10th
	}

	const Frame& getFrame(int index) const {
		return frames[index];
	}

private:
	int strikeBonus(size_t frameIndex) {
		int bonus = 0;
		if (frameIndex + 1 < frames.size()) {
			const Frame& next = frames[frameIndex + 1];
			bonus += next.getRoll(0);
			if (next.isStrike() && frameIndex + 2 < frames.size()) {
				bonus += frames[frameIndex + 2].getRoll(0);
			}
			else {
				bonus += next.getRoll(1);
			}
		}
		return bonus;

	}
	int spareBonus(size_t frameIndex) {
		if (frameIndex + 1 >= frames.size()) return 0;
		const Frame& next = frames[frameIndex + 1];
		return next.getRoll(0); // works for frames 1–8

		// Special case: if frameIndex == 8 (9th frame), and 10th frame is a spare
		// then bonus should be the third roll of the 10th frame
		if (frameIndex == 8 && next.getRolls().size() >= 3) {
			return next.getRoll(2);
		}
		return next.getRoll(0);

	}
};
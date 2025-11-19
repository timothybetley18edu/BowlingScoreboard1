#include <iostream>
#include <vector>

using namespace std;

class Frame {
private:
	vector<int> rolls;
	bool isTenthFrame;

public:
	Frame(bool tenth = false) : isTenthFrame(tenth) {}

	void addRoll(int pins) {
		rolls.push_back(pins);
	}
	bool isStrike() const {
		return rolls.size() >= 1 && rolls[0] == 10;
	}
	bool isSpare() const {
		return rolls.size() >= 2 && (rolls[0] + rolls[1] == 10) && !isStrike();
	}
	bool isComplete() const {
		if (!isTenthFrame) {
			return isStrike() || rolls.size() == 2;
		}
		else {
			if (rolls.size() < 2) {
				return false;
			}
			if (rolls.size() == 2 && (rolls[0] + rolls[1] < 10)) {
				return true;
			}
			return rolls.size() >= 2 && (rolls[0] + rolls[1] < 10 || rolls.size() == 3);
		}
	}
	int getRoll(int index) const {
		if (index < rolls.size()) {
			return rolls[index];
		}
		return 0;
	}
	int getScore() const {
		int sum = 0;
		for (int pins : rolls) {
			sum += pins;
		}
		return sum;
	}
	const vector<int>& getRolls() const {
		return rolls;
	}
};
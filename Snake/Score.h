#ifndef SCORE_H
#define SCORE_H

#include <string>

namespace snake {

	struct Score {
	public:
		Score(std::string name, unsigned score) :
			name(name), score(score) {}

		const std::string name;
		const unsigned score;

		bool operator==(const Score& other) {
			return this->score == other.score;
		}
		bool operator<=(const Score& other) {
			return this->score <= other.score;
		}
		bool operator>=(const Score& other) {
			return this->score >= other.score;
		}
		bool operator<(const Score& other) {
			return this->score < other.score;
		}
		bool operator>(const Score& other) {
			return this->score > other.score;
		}
	};
}

#endif
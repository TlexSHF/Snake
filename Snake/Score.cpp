#include "Score.h"

namespace snake {

	snake::Score::Score(std::string name, unsigned score) :
		m_name(name), m_score(score) {}

	std::string snake::Score::getName() const {
		return m_name;
	}

	unsigned snake::Score::getScore() const {
		return m_score;
	}

	Score& snake::Score::operator=(Score other) {

		m_name = other.getName();
		m_score = other.getScore();

		return *this;
	}

	bool Score::operator==(const Score& other) const {
		return m_score == other.getScore();
	}

	bool Score::operator<=(const Score& other) const {
		return m_score <= other.getScore();
	}

	bool Score::operator>=(const Score& other) const {
		return m_score >= other.getScore();
	}

	bool Score::operator<(const Score& other) const {
		return m_score < other.getScore();
	}

	bool Score::operator>(const Score& other) const {
		return m_score > other.getScore();
	}

	bool Score::operator<=(int i) const {
		return m_score <= i;
	}

	bool Score::operator>=(int i) const {
		return m_score >= i;
	}

	bool Score::operator<(int i) const {
		return m_score < i;
	}

	bool Score::operator>(int i) const {
		return m_score > i;
	}

	bool operator<=(int i, const Score& s) {
		return i <= s.m_score;
	}

	bool operator>=(int i, const Score& s) {
		return i >= s.m_score;
	}

	bool operator<(int i, const Score& s) {
		return i < s.m_score;
	}

	bool operator>(int i, const Score& s) {
		return i > s.m_score;
	}

}
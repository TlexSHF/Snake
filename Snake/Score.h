#ifndef SCORE_H
#define SCORE_H

#include <string>

namespace snake {

	struct Score {
	public:
		Score(std::string name, unsigned score);

		/* Getters */
		std::string getName() const;
		unsigned getScore() const;

		/* Compare with same type object */
		Score& operator=(Score other);
		bool operator==(const Score& other) const;
		bool operator<=(const Score& other) const;
		bool operator>=(const Score& other) const;
		bool operator<(const Score& other) const;
		bool operator>(const Score& other) const;

		/* Compare object with int */
		bool operator<=(int i) const;
		bool operator>=(int i) const;
		bool operator<(int i) const;
		bool operator>(int i) const;

		/* Compare int with object */
		friend bool operator<=(int i, const Score& s);
		friend bool operator>=(int i, const Score& s);
		friend bool operator<(int i, const Score& s);
		friend bool operator>(int i, const Score& s);

	private:
		std::string m_name;
		unsigned m_score;
	};
}

#endif
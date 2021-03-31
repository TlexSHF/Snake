#ifndef CELL_H
#define CELL_H

#include <ostream>

struct Cell {
public:
	char getType();
	char getPrevType();
	void setType(char t);

	friend std::ostream& operator<<(std::ostream& os, Cell& c) {
		return os << c.type;
	}
private:
	char type = '0';
	char prevType = type;
};

#endif
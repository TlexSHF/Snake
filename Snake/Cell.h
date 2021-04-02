#ifndef CELL_H
#define CELL_H

#include <ostream>

struct Cell {
public:
	char getType();
	void setType(char t);
	bool hasChanged();

	friend std::ostream& operator<<(std::ostream& os, Cell& c) {
		return os << c.type;
	}
private:
	char type = 0;
	bool changed = true;
};

#endif
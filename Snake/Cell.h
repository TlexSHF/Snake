#ifndef CELL_H
#define CELL_H

#include <ostream>

namespace snake {

	struct Cell {
	public:
		char getType();
		void setType(char type);
		bool hasChanged();

		friend std::ostream& operator<<(std::ostream& os, Cell& c);

	private:
		char m_type = 0;
		bool m_changed = true;
	};
}

#endif
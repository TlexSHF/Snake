#ifndef COORDS_H
#define COORDS_H

namespace snake {

	struct Coords {
	public:
		Coords(unsigned x, unsigned y);
		unsigned getX();
		unsigned getY();
		void setCoords(unsigned x, unsigned y);

		bool operator==(const Coords& other);

	private:
		unsigned m_x;
		unsigned m_y;
	};
}

#endif

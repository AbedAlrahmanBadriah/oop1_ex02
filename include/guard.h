#pragma once

#include "Location.h"
#include "board.h"

class guard
{
public:
	guard(Location start);

	Location position() const;
	void reset();
	void move_to(Location location);

	Location choose_move(const board& b, Location digger_location) const;

private:
	Location m_location;
	Location m_start;
};

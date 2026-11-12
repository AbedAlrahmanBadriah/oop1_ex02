#include "guard.h"

#include <cmath>

guard::guard(Location start)
	: m_location(start)
	, m_start(start)
{
}

Location guard::position() const
{
	return m_location;
}

void guard::reset()
{
	m_location = m_start;
}

void guard::move_to(Location location)
{
	m_location = location;
}

Location guard::choose_move(const board& b, Location digger_location) const
{
	const int row_diff = digger_location.row - m_location.row;
	const int col_diff = digger_location.col - m_location.col;

	const int row_step = (row_diff > 0) - (row_diff < 0);
	const int col_step = (col_diff > 0) - (col_diff < 0);

	Location dirs[4];

	if (std::abs(row_diff) >= std::abs(col_diff))
	{
		dirs[0] = Location{ row_step, 0 };
		dirs[1] = Location{ 0, col_step };
		dirs[2] = Location{ -row_step, 0 };
		dirs[3] = Location{ 0, -col_step };
	}
	else
	{
		dirs[0] = Location{ 0, col_step };
		dirs[1] = Location{ row_step, 0 };
		dirs[2] = Location{ 0, -col_step };
		dirs[3] = Location{ -row_step, 0 };
	}

	for (const Location& dir : dirs)
	{
		if (dir.row == 0 && dir.col == 0)
			continue;

		const Location next{ m_location.row + dir.row, m_location.col + dir.col };
		const char tile = b.get_tile(next.row, next.col);

		if (tile != board::k_wall && tile != board::k_stone)
			return next;
	}

	return m_location;
}

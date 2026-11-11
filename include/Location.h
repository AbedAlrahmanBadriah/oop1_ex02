#pragma once

struct Location
{
	int row;
	int col;
};

inline bool operator==(const Location& lhs, const Location& rhs)
{
	return lhs.row == rhs.row && lhs.col == rhs.col;
}

inline bool operator!=(const Location& lhs, const Location& rhs)
{
	return !(lhs == rhs);
}

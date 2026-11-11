#include "board.h"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <limits>
#include <string>


board::board(const std::string& path)
{
	m_file.open(path);
	if (!m_file.is_open())
		std::cerr << "Error opening file: " << path << '\n';
}

bool board::load_next_level()
{
	if (!m_file.is_open())
	{
		std::cerr << "File not open.\n";
		return false;
	}

	if (!(m_file >> m_size >> m_max_stones))
	{
		if (m_file.eof())
			return false;

		std::cerr << "Error reading level data.\n";
		return false;
	}

	if (m_size <= 0 || m_max_stones < 0)
	{
		std::cerr << "Invalid level header.\n";
		return false;
	}

	// Consume the rest of the header line.
	m_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	m_board.assign(static_cast<std::size_t>(m_size),
		std::vector<char>(static_cast<std::size_t>(m_size), board::k_wall));

	std::string line;
	for (int row = 0; row < m_size; ++row)
	{
		if (!std::getline(m_file, line))
			return false;

		if (!line.empty() && line.back() == '\r')
			line.pop_back();

		if (static_cast<int>(line.size()) != m_size)
		{
			std::cerr << "Row " << row << " has length " << line.size()
				<< ", expected " << m_size << ".\n";
			return false;
		}

		std::copy_n(line.begin(), m_size,
			m_board[static_cast<std::size_t>(row)].begin());
	}

	++m_current_level;
	return true;
}

void board::print_board() const
{
	for (const auto& row : m_board)
	{
		for (char tile : row)
			std::cout << tile;

		std::cout << '\n';
	}
}

bool board::is_inside(int row, int col) const
{
	return row >= 0 && col >= 0
		&& row < static_cast<int>(m_board.size())
		&& col < static_cast<int>(m_board[static_cast<std::size_t>(row)].size());
}

bool board::is_wall(int row, int col) const
{
	return get_tile(row, col) == board::k_wall;
}

char board::get_tile(int row, int col) const
{
	if (!is_inside(row, col))
		return board::k_wall;

	return m_board[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
}

void board::set_tile(int row, int col, char tile)
{
	if (!is_inside(row, col))
		return;

	m_board[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)] = tile;
}

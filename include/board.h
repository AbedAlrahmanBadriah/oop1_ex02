#pragma once

#include <fstream>
#include <string>
#include <vector>

class board
{
public:
	static constexpr char k_empty = ' ';
	static constexpr char k_wall = '#';
	static constexpr char k_digger = '/';
	static constexpr char k_guard = '!';
	static constexpr char k_diamond = 'D';
	static constexpr char k_stone = '@';

	board(const std::string& path);

	bool is_open() const { return m_file.is_open(); }
	bool load_next_level();
	void print_board() const;

	int get_size() const { return m_size; }
	int get_max_stones() const { return m_max_stones; }
	int get_current_level() const { return m_current_level; }

	bool is_inside(int row, int col) const;
	bool is_wall(int row, int col) const;

	// Returns '#' for cells outside the board.
	char get_tile(int row, int col) const;
	void set_tile(int row, int col, char tile);

	const std::vector<std::vector<char>>& get_board() const { return m_board; }

private:
	std::ifstream m_file;
	std::vector<std::vector<char>> m_board;
	int m_size = 0;
	int m_max_stones = 0;
	int m_current_level = 0;
};

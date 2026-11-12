#include "controller.h"
#include "io.h"

#include <cstdlib>
#include <iostream>
#include <conio.h>

controller::controller()
	: m_board("resources/Board.txt")
{
}

void controller::run()
{
	if (!m_board.is_open())
	{
		std::cerr << "Failed to open levels file.\n";
		return;
	}

	if (!m_board.load_next_level())
	{
		std::cerr << "Failed to load the level.\n";
		return;
	}

	place_digger();

	Screen::resetLocation();
	render();

	while (handle_input())
	{
	}
}

void controller::render() const
{
	for (int row = 0; row < m_board.get_size(); ++row)
	{
		for (int col = 0; col < m_board.get_size(); ++col)
		{
			if (m_digger.position() == Location{row, col})
				std::cout << board::k_digger;
			else
				std::cout << m_board.get_tile(row, col);
		}

		std::cout << '\n';
	}

	std::cout << "Score: " << m_score
		<< " | Lives: " << m_digger.get_lives()
		<< " | Stones: " << m_board.get_remaining_stones()
		<< " | Level: " << m_board.get_current_level()
		<< "        \n";
}

bool controller::handle_input()
{
	int key = _getch();

	if (key == Keys::ESCAPE)
		return false;

	if (key == Keys::SPECIAL_KEY)
		key = _getch();

	Location delta{ 0, 0 };

	switch (key)
	{
	case SpecialKeys::UP:    delta = Location{ -1,  0 }; break;
	case SpecialKeys::DOWN:  delta = Location{  1,  0 }; break;
	case SpecialKeys::LEFT:  delta = Location{  0, -1 }; break;
	case SpecialKeys::RIGHT: delta = Location{  0,  1 }; break;
	default:                 return true;
	}

	const Location current = m_digger.position();
	const Location target{ current.row + delta.row, current.col + delta.col };

	return move_digger(target);
}

bool controller::move_digger(Location target)
{
	const char tile = m_board.get_tile(target.row, target.col);

	if (tile == board::k_wall)
		return true;

	if (tile == board::k_stone && m_board.get_remaining_stones() <= 0)
		return true;

	m_digger.move_to(target);

	if (tile == board::k_stone)
	{
		m_board.use_stone();
		m_board.set_tile(target.row, target.col, board::k_empty);
	}
	else if (tile == board::k_diamond)
	{
		m_board.set_tile(target.row, target.col, board::k_empty);
		m_score += k_diamond_points;
	}

	if (m_board.count_tiles(board::k_diamond) == 0)
	{
		m_score += k_level_bonus;

		if (m_board.load_next_level())
		{
			place_digger();
			system("cls");
		}
		else
		{
			system("cls");
			std::cout << "\nYou win! Final score: " << m_score << '\n';
			return false;
		}
	}

	Screen::resetLocation();
	render();
	return true;
}

void controller::place_digger()
{
	for (int row = 0; row < m_board.get_size(); ++row)
	{
		for (int col = 0; col < m_board.get_size(); ++col)
		{
			if (m_board.get_tile(row, col) == board::k_digger)
			{
				m_digger.reset(Location{row, col});
				m_board.set_tile(row, col, board::k_empty);
			}
		}
	}
}
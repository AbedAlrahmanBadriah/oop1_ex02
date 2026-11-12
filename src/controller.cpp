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

	place_characters();

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
			const Location here{row, col};

			if (m_digger.position() == here)
				std::cout << board::k_digger;
			else if (guard_at(here))
				std::cout << board::k_guard;
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

	if (key == k_space)
		return move_digger(m_digger.position());

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

	// A guard on the target cell catches the digger immediately. The hit
	// takes priority over the tile, so the tile is NOT collected here.
	if (digger_caught())
	{
		if (!handle_hit())
			return false;

		Screen::resetLocation();
		render();
		return true;
	}

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
			place_characters();
			system("cls");
			Screen::resetLocation();
			render();
			return true;
		}

		system("cls");
		std::cout << "\nYou win! Final score: " << m_score << '\n';
		return false;
	}

	move_guards();

	if (digger_caught() && !handle_hit())
		return false;

	Screen::resetLocation();
	render();
	return true;
}

void controller::move_guards()
{
	for (guard& g : m_guards)
	{
		const Location next = g.choose_move(m_board, m_digger.position());

		if (next != g.position())
			g.move_to(next);
	}
}

bool controller::digger_caught() const
{
	for (const guard& g : m_guards)
	{
		if (g.position() == m_digger.position())
			return true;
	}

	return false;
}

bool controller::handle_hit()
{
	if (!m_digger.lose_life())
	{
		system("cls");
		std::cout << "\nGame over. Final score: " << m_score << '\n';
		return false;
	}

	reset_characters();
	return true;
}

void controller::place_characters()
{
	m_guards.clear();
	m_digger_start = Location{ -1, -1 };

	for (int row = 0; row < m_board.get_size(); ++row)
	{
		for (int col = 0; col < m_board.get_size(); ++col)
		{
			if (m_board.get_tile(row, col) == board::k_digger)
			{
				m_digger_start = Location{ row, col };
				m_digger.reset(Location{row, col});
				m_board.set_tile(row, col, board::k_empty);
			}
			else if (m_board.get_tile(row, col) == board::k_guard)
			{
				m_guards.emplace_back(Location{ row, col });
				m_board.set_tile(row, col, board::k_empty);
			}
		}
	}
}

void controller::reset_characters()
{
	m_digger.reset(m_digger_start);
	for (auto& guard : m_guards)
	{
		guard.reset();
	}
}

bool controller::guard_at(Location location) const
{
	for (const guard& g : m_guards)
	{
		if (g.position() == location)
			return true;
	}

	return false;
}
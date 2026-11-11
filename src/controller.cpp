#include "controller.h"
#include "io.h"

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

	for (int row = 0; row < m_board.get_size(); ++row)
	{
		for (int col = 0; col < m_board.get_size(); ++col)
		{
			if (m_board.get_tile(row, col) == '/')
			{
				m_digger.reset(Location{row, col});
				m_board.set_tile(row, col, ' ');
			}
		}
	}

	Screen::resetLocation();
	render();
	while (handle_input());

	
}

void controller::render() const
{
	for (int row = 0; row < m_board.get_size(); ++row)
	{
		for (int col = 0; col < m_board.get_size(); ++col)
		{
			if (m_digger.position() == Location{row, col})
				std::cout << '/';
			else
				std::cout << m_board.get_tile(row, col);
		}

		std::cout << '\n';
	}

	
	
}

bool controller::handle_input() {
	
		int key = _getch();

		if(key == Keys::ESCAPE)
			return false;

		if (key == Keys::SPECIAL_KEY)
			key = _getch();

		Location delta{ 0, 0 };
		switch (key)
		{
		case SpecialKeys::UP: delta = Location{ -1, 0 }; break;
		case SpecialKeys::DOWN: delta = Location{ 1, 0 }; break;
		case SpecialKeys::LEFT: delta = Location{ 0, -1 }; break;
		case SpecialKeys::RIGHT: delta = Location{ 0, 1 }; break;
		default: return true;
		}

		const Location current = m_digger.position();
		const Location target{ current.row + delta.row, current.col + delta.col };

		if (m_board.is_wall(target.row, target.col)) {
			return true;
		}

		m_digger.move_to(target);
		Screen::resetLocation();
		render();
		
		return true;
}
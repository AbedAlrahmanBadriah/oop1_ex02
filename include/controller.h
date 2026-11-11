#pragma once

#include "board.h"
#include "digger.h"

class controller
{
public:
	static constexpr int k_diamond_points = 15;
	static constexpr int k_level_bonus = 20;
	
	controller();
	void run();

private:
	void render() const;
	bool handle_input();

	board m_board;
	digger m_digger;
};

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
	bool move_digger(Location target);
	void place_digger();


	board m_board;
	digger m_digger;
	int m_score = 0;
};

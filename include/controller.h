#pragma once

#include <vector>

#include "board.h"
#include "digger.h"
#include "guard.h"

class controller
{
public:
	static constexpr int k_diamond_points = 15;
	static constexpr int k_level_bonus = 20;
	static constexpr int k_space = ' ';

	controller();
	void run();

private:
	void render() const;
	bool handle_input();
	bool move_digger(Location target);
	void place_characters();
	void reset_characters();
	void move_guards();
	bool digger_caught() const;
	bool handle_hit();
	bool guard_at(Location location) const;

	board m_board;
	digger m_digger;
	std::vector<guard> m_guards;
	Location m_digger_start{-1, -1};
	int m_score = 0;
};

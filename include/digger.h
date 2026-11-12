#pragma once

#include "Location.h"

class digger
{
public:
	static constexpr int kInitialLives = 3;

	digger() = default;
	digger(Location start);

	Location position() const;
	void reset(Location start);
	void move_to(Location location);

	int get_lives() const;
	void set_lives(int lives);
	bool lose_life();

private:
	Location m_location{-1, -1};
	int m_lives = kInitialLives;
};

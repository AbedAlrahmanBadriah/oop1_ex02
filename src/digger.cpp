#include "digger.h"

digger::digger(Location start)
	: m_location(start)
{
}

Location digger::position() const
{
	return m_location;
}

void digger::reset(Location start)
{
	m_location = start;
}

void digger::move_to(Location location)
{
	m_location = location;
}

int digger::get_lives() const
{
	return m_lives;
}

void digger::set_lives(int lives)
{
	m_lives = lives;
}

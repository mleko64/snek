#include "Goal.h"

void Goal::Draw(Board & brd) const
{
	brd.DrawCell(loc, c);
}

const Location & Goal::GetLocation() const
{
	return loc;
}

void Goal::SetLocation(Location loc_in)
{
	loc = loc_in;
}

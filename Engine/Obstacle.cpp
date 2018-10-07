#include "Obstacle.h"

void Obstacle::SetLocation(Location loc_in)
{
	loc = loc_in;
}

void Obstacle::Draw(Board & brd) const
{
	brd.DrawCell(loc, c);
}

const Location & Obstacle::GetLocation() const
{
	return loc;
}

#pragma once

#include "Board.h"

class Obstacle
{
public:
	void SetLocation(Location loc_in);
	void Draw(Board& brd) const;
	const Location& GetLocation() const;
private:
	static constexpr Color c = Colors::Gray;
	Location loc;
};
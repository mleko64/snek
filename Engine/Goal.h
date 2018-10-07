#pragma once

#include "Board.h"

class Goal
{
public:
	void Draw(Board& brd) const;
	const Location& GetLocation() const;
	void SetLocation(Location loc_in);
private:
	static constexpr Color c = Colors::Red;
	Location loc;
};
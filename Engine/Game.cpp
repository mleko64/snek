/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	brd(gfx),
	rng(std::random_device()()),
	snek({ 2,2 })
{
	GenerateGoal();
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (gameIsStarted) {
		if (!gameIsOver) {
			if (wnd.kbd.KeyIsPressed(VK_UP)) {
				delta_loc = { 0, -1 };
			}
			else if (wnd.kbd.KeyIsPressed(VK_DOWN)) {
				delta_loc = { 0, 1 };
			}
			else if (wnd.kbd.KeyIsPressed(VK_LEFT)) {
				delta_loc = { -1, 0 };
			}
			else if (wnd.kbd.KeyIsPressed(VK_RIGHT)) {
				delta_loc = { 1, 0 };
			}

			++snekMoveCounter;
			if (snekMoveCounter >= snekMovePeriod) {
				snekMoveCounter = 0;

				const Location next = snek.GetNextHeadLocation(delta_loc);
				bool hitObstacle = false;
				for (int i = 0; i < nObscales; ++i) {
					if (obstacles[i].GetLocation() == next) {
						hitObstacle = true;
					}
				}
				
				if (!brd.IsInsideBoard(next) || snek.IsInTileExceptEnd(next) || hitObstacle) {
					gameIsOver = true;
				}
				else {
					const bool eating = next == goal.GetLocation();
					if (eating) {
						snek.Grow();
					}
					snek.MoveBy(delta_loc);
					if (eating) {
						GenerateGoal();
					}
				}
			}

			++snekSpeedUpCounter;
			if (snekSpeedUpCounter >= snekSpeedUpPeriod) {
				snekSpeedUpCounter = 0;
				snekMovePeriod = std::max(snekMovePeriod - 1, snekMoviePeriodMin);
			}

			++obstacleShowCounter;
			if (obstacleShowCounter >= obstacleShowPeriod) {
				obstacleShowCounter = 0;
				GenerateObstacle();
			}
		}
	}
	else {
		gameIsStarted = wnd.kbd.KeyIsPressed(VK_RETURN);
	}
}

void Game::GenerateObstacle()
{
	std::uniform_int_distribution<int> xDist(0, brd.GetGridWidth() - 1);
	std::uniform_int_distribution<int> yDist(0, brd.GetGridHeight() - 1);

	Location newLoc;
	bool overlapsLocation;
	do {
		overlapsLocation = false;
		newLoc.x = xDist(rng);
		newLoc.y = yDist(rng);

		for (int i = 0; i < nObscales; ++i) {
			if (obstacles[i].GetLocation() == newLoc) {
				overlapsLocation = true;
				break;
			}
		}

	} while (snek.IsInTile(newLoc) || goal.GetLocation() == newLoc || overlapsLocation);

	obstacles[nObscales].SetLocation(newLoc);
	++nObscales;
}

void Game::GenerateGoal()
{
	std::uniform_int_distribution<int> xDist(0, brd.GetGridWidth() - 1);
	std::uniform_int_distribution<int> yDist(0, brd.GetGridHeight() - 1);

	Location newLoc;
	bool overlapsLocation;
	do {
		overlapsLocation = false;
		newLoc.x = xDist(rng);
		newLoc.y = yDist(rng);

		for (int i = 0; i < nObscales; ++i) {
			if (obstacles[i].GetLocation() == newLoc) {
				overlapsLocation = true;
				break;
			}
		}

	} while (snek.IsInTile(newLoc) || overlapsLocation);

	goal.SetLocation(newLoc);
}

void Game::ComposeFrame()
{
	if (gameIsStarted) {
		brd.DrawBorder();
		snek.Draw(brd);
		goal.Draw(brd);

		for (int i = 0; i < nObscales; ++i) {
			obstacles[i].Draw(brd);
		}

		if (gameIsOver) {
			SpriteCodex::DrawGameOver(350, 265, gfx);
		}
	}
	else {
		SpriteCodex::DrawTitle(290, 225, gfx);
	}
}

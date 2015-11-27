/*
 * Copyright (C) 2015 Dmitry Marakasov <amdmi3@amdmi3.ru>
 *
 * This file is part of hoverboard-sdl.
 *
 * hoverboard-sdl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * hoverboard-sdl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hoverboard-sdl.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GAME_HH
#define GAME_HH

#include <map>
#include <set>
#include <vector>

#include <SDL2pp/Rect.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Renderer.hh>

#include "tilecache.hh"

class Game {
public:
	enum ActionFlags {
		UP    = 0x01,
		DOWN  = 0x02,
		LEFT  = 0x04,
		RIGHT = 0x08,
	};

private:
	const static std::vector<SDL2pp::Point> coin_locations_;
	constexpr static int start_player_x_ = 512106;
	constexpr static int start_player_y_ = -549612;

	constexpr static int left_world_bound_ = 475136;
	constexpr static int right_world_bound_ = 567295;

	constexpr static int player_width_ = 29;
	constexpr static int player_height_ = 59;

	constexpr static int coin_size_ = 25;

private:
	SDL2pp::Renderer& renderer_;

	// Resources
	SDL2pp::Texture coin_texture_;
	SDL2pp::Texture player_texture_;

	TileCache tc_;

	// Game state
	int action_flags_;

	float player_x_;
	float player_y_;

public:
	Game(SDL2pp::Renderer& renderer);
	~Game();

	void SetActionFlag(int flag);
	void ClearActionFlag(int flag);

	SDL2pp::Rect GetCameraRect() const;
	SDL2pp::Rect GetPlayerRect() const;
	SDL2pp::Rect GetCoinRect(const SDL2pp::Point& coin) const;

	void Update(float delta_t);
	void Render();//const SDL2pp::Rect& viewport);
};

#endif // GAME_HH

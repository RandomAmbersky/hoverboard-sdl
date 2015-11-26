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

#include <iostream>
#include <string>
#include <sstream>

#include <SDL2/SDL.h>

#include <SDL2pp/SDL.hh>
#include <SDL2pp/Window.hh>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Texture.hh>

#include "game.hh"

int main(int /*argc*/, char** /*argv*/) try {
	// SDL stuff
	SDL2pp::SDL sdl(SDL_INIT_VIDEO);
	SDL2pp::Window window("Hoverboard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
	SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

	Game game(renderer);

	float xpos = 512106;
	float ypos = -549612;

	float xspeed = 0.0;
	float yspeed = 0.0;

	unsigned int prev_ticks = SDL_GetTicks();

	// Main loop
	while (1) {
		unsigned int frame_ticks = SDL_GetTicks();
		unsigned int frame_delta = frame_ticks - prev_ticks;
		prev_ticks = frame_ticks;

		// Process events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return 0;
			} else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE: case SDLK_q:
					return 0;
				case SDLK_LEFT:
					xspeed = -1.0f;
					break;
				case SDLK_RIGHT:
					xspeed = 1.0f;
					break;
				case SDLK_UP:
					yspeed = -1.0f;
					break;
				case SDLK_DOWN:
					yspeed = 1.0f;
					break;
				}
			} else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
				case SDLK_RIGHT:
					xspeed = 0.0f;
					break;
				case SDLK_UP:
				case SDLK_DOWN:
					yspeed = 0.0f;
					break;
				}
			}
		}

		// Move
		xpos += xspeed * frame_delta;
		ypos += yspeed * frame_delta;

		if (xpos < 475136)
			xpos = 475136;
		if (xpos > 567295)
			xpos = 567295;

		// Render
		renderer.SetDrawColor(255, 255, 255);
		renderer.Clear();

		SDL2pp::Rect view_rect(SDL2pp::Point((int)xpos, (int)ypos) - window.GetSize() / 2, window.GetSize());

		game.Render(view_rect);

		renderer.Present();

		// Frame limiter
		SDL_Delay(1);
	}

	return 0;
} catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
	return 1;
} catch (...) {
	std::cerr << "Unknown error" << std::endl;
	return 1;
}


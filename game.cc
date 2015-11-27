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

#include "game.hh"

#include <sys/types.h>
#include <sys/stat.h>

#include <SDL2pp/Surface.hh>

#include <memory>

#include <sstream>

Game::Game(SDL2pp::Renderer& renderer)
	: renderer_(renderer),
	  coin_texture_(renderer_, DATADIR "/coin.png"),
	  player_texture_(renderer_, DATADIR "/all-four.png"),
	  tc_(renderer),
	  action_flags_(0),
	  player_x_(start_player_x_),
	  player_y_(start_player_y_),
	  coins_(coin_locations_) {
}

Game::~Game() {
}

void Game::SetActionFlag(int flag) {
	action_flags_ |= flag;
}

void Game::ClearActionFlag(int flag) {
	action_flags_ &= ~flag;
}

SDL2pp::Rect Game::GetCameraRect() const {
	return SDL2pp::Rect(
			(int)player_x_ - renderer_.GetOutputWidth() / 2,
			(int)player_y_ - renderer_.GetOutputHeight() / 2,
			renderer_.GetOutputWidth(),
			renderer_.GetOutputHeight()
		);
}

SDL2pp::Rect Game::GetPlayerRect() const {
	return SDL2pp::Rect(
			(int)player_x_ - player_width_ + player_width_ / 2,
			(int)player_y_ - player_height_ + player_height_ / 2,
			player_width_,
			player_height_
		);
}

SDL2pp::Rect Game::GetCoinRect(const SDL2pp::Point& coin) const {
	return SDL2pp::Rect(
			(int)coin.x - coin_size_ + coin_size_ / 2,
			(int)coin.y - coin_size_ + coin_size_ / 2,
			coin_size_,
			coin_size_
		);
}

void Game::Update(float delta_t) {
	static const float speed = 1.0;

	float xspeed = 0.0;
	float yspeed = 0.0;

	// Process player movement
	if (action_flags_ & UP)
		yspeed -= speed;
	if (action_flags_ & DOWN)
		yspeed += speed;
	if (action_flags_ & LEFT)
		xspeed -= speed;
	if (action_flags_ & RIGHT)
		xspeed += speed;

	player_x_ += xspeed * delta_t;
	player_y_ += yspeed * delta_t;

	// Limit world
	if (player_x_ < left_world_bound_)
		player_x_ = left_world_bound_;
	if (player_y_ > right_world_bound_)
		player_y_ = right_world_bound_;

	// Player rectangle
	SDL2pp::Rect player_rect = GetPlayerRect();

	// Collect coins
	coins_.remove_if([&](const SDL2pp::Point& coin){ return player_rect.Intersects(GetCoinRect(coin)); } );

	// Update tile cache
	tc_.UpdateCache(GetCameraRect().GetExtension(512));
}

void Game::Render() {
	SDL2pp::Rect camerarect = GetCameraRect();

	tc_.Render(camerarect);

	// draw coins
	for (auto& coin : coins_)
		renderer_.Copy(coin_texture_, SDL2pp::NullOpt, GetCoinRect(coin) - SDL2pp::Point(camerarect.x, camerarect.y));

	// draw player
	renderer_.Copy(player_texture_, SDL2pp::Rect(0, 0, GetPlayerRect().w, GetPlayerRect().h), GetPlayerRect() - SDL2pp::Point(camerarect.x, camerarect.y));
}

void Game::ResetCoins() {
	coins_ = coin_locations_;
};

const Game::CoinList Game::coin_locations_ = {
	{537027, -560249},
	{525689, -560616},
	{526077, -560616},
	{526164, -559291},
	{531494, -559530},
	{531903, -559554},
	{532170, -560864},
	{555130, -562404},
	{503224, -552394},
	{503522, -551049},
	{542481, -560037},
	{542706, -559642},
	{550773, -560041},
	{553076, -567112},
	{553103, -567151},
	{553132, -567183},
	{553173, -565356},
	{553208, -567184},
	{553288, -567149},
	{553329, -567108},
	{554243, -565849},
	{554243, -565780},
	{557912, -558137},
	{526693, -560208},
	{526995, -559135},
	{527086, -559782},
	{528028, -561056},
	{528198, -560225},
	{527061, -557254},
	{483600, -551975},
	{483667, -551977},
	{553345, -563807},
	{519636, -549096},
	{522049, -553195},
	{490124, -554986},
	{552820, -560057},
	{520607, -549047},
	{521392, -549021},
	{538150, -550895},
	{560567, -549975},
	{528618, -549429},
	{553737, -550104},
	{529664, -558476},
	{556369, -556826},
	{523458, -549103},
	{548206, -561787},
	{548271, -561787},
	{542941, -562344},
	{475207, -553683},
	{475257, -553684},
	{475313, -553684},
	{475367, -553680},
	{518169, -560337},
	{519089, -561130},
	{519929, -559544},
	{525306, -561659},
	{535582, -561307},
	{535875, -562506},
	{535900, -563088},
	{534153, -559619},
	{508250, -567578},
	{551586, -563946},
	{552593, -563797},
	{486608, -554809},
	{516591, -560321},
	{517846, -559181},
	{517968, -559859},
	{518041, -561062},
	{548482, -549800},
	{546744, -559592},
	{567063, -550422},
	{567065, -550503},
	{567086, -550464},
	{567086, -550540},
	{567111, -550503},
	{567111, -550422},
	{567130, -550540},
	{567134, -550464},
	{567152, -550503},
	{567154, -550422},
	{539260, -562964},
	{540037, -562347},
	{540302, -562977},
	{540380, -562347},
	{558132, -563858},
	{507154, -568861},
	{541755, -563024},
	{542595, -562588},
	{542595, -562506},
	{518682, -551658},
	{519441, -552229},
	{530791, -558938},
	{531482, -558698},
	{479534, -554932},
	{501990, -549107},
	{481857, -554526},
	{482009, -554496},
	{539605, -558914},
	{542768, -564500},
	{543822, -563314},
	{544521, -564725},
	{544620, -563890},
	{512082, -549750},
	{512093, -549901},
	{512099, -550062},
	{512207, -549881},
	{512209, -550222},
	{512217, -549740},
	{512223, -549647},
	{512258, -549740},
	{512272, -550041},
	{512296, -549739},
	{512323, -549883},
	{512348, -550214},
	{512349, -549642},
	{512385, -549886},
	{512396, -549601},
	{512414, -550061},
	{512417, -549788},
	{549232, -565175},
	{549831, -563260},
	{550487, -564557},
	{477754, -554489},
	{522741, -551458},
	{497597, -551742},
	{525583, -549247},
	{547911, -560431},
	{492147, -553606},
	{556845, -567091},
	{523816, -560628},
	{524078, -560628},
	{524079, -560382},
	{552105, -568488},
	{527039, -549219},
	{528075, -549711},
	{537789, -558007},
	{539524, -559910},
	{540329, -560393},
	{483549, -549417},
	{485094, -549147},
	{551295, -565285},
	{551501, -566387},
	{551546, -566765},
	{552221, -566176},
	{523748, -561433},
	{523778, -561435},
	{523810, -561437},
	{547918, -555114},
	{548026, -556862},
	{541735, -559053},
	{541844, -557595},
	{542018, -558003},
	{542611, -557542},
	{541203, -563852},
	{541641, -563852},
	{542076, -563852},
	{545090, -561997},
	{545524, -562339},
	{546312, -562840},
	{546312, -562732},
	{546600, -562845},
	{546607, -562732},
	{513108, -560669},
	{513738, -559677},
	{549508, -558811},
	{549843, -558016},
	{550152, -558016},
	{528673, -559748},
	{529300, -560287}
};

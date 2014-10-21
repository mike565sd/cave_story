#include "game.h"
#include "graphics.h"
#include "map.h"
#include "player.h"
#include "input.h"
#include "SDL.h"
#include "first_cave_bat.h"
#include "timer.h"

namespace {
	const units::FPS kFps = 60;
	const units::MS kMaxFrameTime = 5 * 1000 / 60;
}

units::Tile Game::kScreenWidth = 20;
units::Tile Game::kScreenHeight = 15;

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	eventLoop();
}

Game::~Game() {
	SDL_Quit();
}

void Game::eventLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;

	player_.reset(new Player(graphics, units::tileToGame(kScreenWidth / 2), units::tileToGame(kScreenHeight / 2)));
	bat_.reset(new FirstCaveBat(graphics, units::tileToGame(7), units::tileToGame(kScreenHeight / 2 + 1)));
	map_.reset(Map::createTestMap(graphics));

	bool running = true;
	units::MS last_update_time = SDL_GetTicks();
	while (running) {
		const units::MS start_time = SDL_GetTicks();
		input.beginNewFrame();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				input.keyDownEvent(event);
				break;
			case SDL_KEYUP:
				input.keyUpEvent(event);
				break;
			default:
				break;
			}
		}

		if (input.wasKeyPressed(SDLK_ESCAPE))
		{
			running = false;
		}

		// Player Movement
		if (input.isKeyHeld(SDLK_LEFT) && input.isKeyHeld(SDLK_RIGHT)) {
			player_->stopMoving();
		} else if (input.isKeyHeld(SDLK_LEFT)) {
			player_->startMovingLeft();
		} else if (input.isKeyHeld(SDLK_RIGHT)) {
			player_->startMovingRight();
		} else {
			player_->stopMoving();
		}

		if (input.isKeyHeld(SDLK_UP) && input.isKeyHeld(SDLK_DOWN)) {
			player_->lookHorizontal();
		} else if (input.isKeyHeld(SDLK_UP)) {
			player_->lookUp();
		} else if (input.isKeyHeld(SDLK_DOWN)) {
			player_->lookDown();
		} else {
			player_->lookHorizontal();
		}

		// Player Jump
		if (input.wasKeyPressed(SDLK_z)) {
			player_->startJump();
		} else if (input.wasKeyReleased(SDLK_z)) {
			player_->stopJump();
		}

		// Player Fire
		if (input.wasKeyPressed(SDLK_x)) {
			player_->startFire();
		}
		else if (input.wasKeyReleased(SDLK_x)) {
			player_->stopFire();
		}

		const units::MS current_time = SDL_GetTicks();
		const units::MS elapsed_time = current_time - last_update_time;
		update(std::min(elapsed_time, kMaxFrameTime));
		last_update_time = current_time;
		draw(graphics);
		const units::MS ms_per_frame = 1000 / kFps;
		const units::MS elapsed_time_ms = SDL_GetTicks() - start_time;
		if (elapsed_time_ms < ms_per_frame) {
			SDL_Delay(ms_per_frame - elapsed_time_ms);
		}
	}
}

void Game::update(units::MS elapsed_time) {
	Timer::updateAll(elapsed_time);

	player_->update(elapsed_time, *map_);

	bat_->update(elapsed_time, player_->center_x());
	
	std::vector<boost::shared_ptr<Projectile> > projectiles(player_->getProjectiles());
	for (size_t i = 0; i < projectiles.size(); ++i) {
		if (bat_->collisionRectangle().collidesWith(projectiles[i]->collisionRectangle())) {
			bat_->takeDamage(projectiles[i]->contactDamage());
			projectiles[i]->collideWithEnemy();
		}
	}

	if (bat_->damageRectangle().collidesWith(player_->damageRectangle())) {
		player_->takeDamage(bat_->contactDamage());
	}
}

void Game::draw(Graphics& graphics) {
	graphics.clear();

	map_->drawBackground(graphics);
	bat_->draw(graphics);
	player_->draw(graphics);
	map_->draw(graphics);

	player_->drawHUD(graphics);
	graphics.flip();
}
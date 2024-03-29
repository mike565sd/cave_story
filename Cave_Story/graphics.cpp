#include "graphics.h"

#include "SDL.h"

#include "game.h"
#include "units.h"

namespace {
	const int kBitsPerPixel = 32;
}

Graphics::Graphics() {
	screen_ = SDL_SetVideoMode(
		units::tileToPixel(Game::kScreenWidth), 
		units::tileToPixel(Game::kScreenHeight), 
		kBitsPerPixel, 0);
	SDL_ShowCursor(SDL_DISABLE);
}

Graphics::~Graphics() {
	for (SpriteMap::iterator iter = sprite_sheets_.begin()
		;
		iter != sprite_sheets_.end();
		++iter) {
			SDL_FreeSurface(iter->second);
	}
	SDL_FreeSurface(screen_);
}

Graphics::SurfaceID Graphics::loadImage(const std::string& file_name, bool black_is_transparent) {
	const std::string file_path = config::getGraphicsQuality() == config::ORIGINAL_QUALITY ?
		"content/original_graphics/" + file_name + ".pbm" :
		"content/" + file_name + ".bmp";
	if (sprite_sheets_.count(file_path) == 0) {
		sprite_sheets_[file_path] = SDL_LoadBMP(file_path.c_str());
		if (black_is_transparent) {
			const Uint32 alphaColor = config::getGraphicsQuality() == config::HIGH_QUALITY ? 
				SDL_MapRGB(sprite_sheets_[file_path]->format, 255, 0, 255) :
				SDL_MapRGB(sprite_sheets_[file_path]->format, 0, 0, 0);
			SDL_SetColorKey(sprite_sheets_[file_path], SDL_SRCCOLORKEY, alphaColor); // 16711935
		}
	}
	return sprite_sheets_[file_path];
}

void Graphics::blitSurface(
		SurfaceID source, 
		SDL_Rect* source_rectangle,
		SDL_Rect* destination_rectangle) {
			SDL_BlitSurface(source, source_rectangle, screen_, destination_rectangle);
}

void Graphics::clear() {
	SDL_FillRect(screen_, NULL, 0);
}

void Graphics::flip() {
	SDL_Flip(screen_);
}

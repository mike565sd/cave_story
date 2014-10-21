#ifndef VARYING_WIDTH_SPRITE_H_
#define VARYING_WIDTH_SPRITE_H_

#include "sprite.h"

struct VaryingWidthSprite : public Sprite {
	VaryingWidthSprite(
		Graphics& graphics,
		const std::string& file_name,
		units::Pixel source_x, units::Pixel source_y,
		units::Pixel initial_width, units::Pixel height) :
	Sprite(graphics, file_name, source_x, source_y, initial_width, height) {}

	void setWidth(units::Pixel width) { source_rect_.w = width; }
};

#endif // VARYING_WIDTH_SPRITE_H_
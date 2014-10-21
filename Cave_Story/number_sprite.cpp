#include "number_sprite.h"
#include "sprite.h"
#include "graphics.h"

namespace {
	const std::string kSpritePath("TextBox");

	const units::Game kSourceX = units::kHalfTile;
	const units::Game kSourceWhiteY = units::kHalfTile * 7;
	const units::Game kSourceRedY = units::kHalfTile * 8;

	const units::Game kPlusSourceX = units::kHalfTile * 4;
	const units::Game kMinusSourceX = units::kHalfTile * 5;
	const units::Game kOpSourceY = units::kHalfTile * 6;

	const units::Game kSourceWidth = units::kHalfTile;
	const units::Game kSourceHeight = units::kHalfTile;

	const size_t kRadix = 10;
}

NumberSprite::NumberSprite(
		Graphics& graphics, int number, int num_digits, ColorType color, OperatorType op) :
	padding_(0.0f) {
	assert(number >= 0);

	units::Game source_y = color == RED ? kSourceRedY : kSourceWhiteY;
	int digit_count = 0;
	do {
		const int digit = number % kRadix;
		reversed_glyphs_.push_back(boost::shared_ptr<Sprite>(new Sprite(
			graphics, kSpritePath,
			units::gameToPixel(kSourceX * digit), units::gameToPixel(source_y),
			units::gameToPixel(kSourceWidth), units::gameToPixel(kSourceHeight))));
		number /= kRadix;
		++digit_count;
	} while (number != 0);
	assert(num_digits == 0 || num_digits >= digit_count);
	padding_ = num_digits == 0 ? 0.0f : units::kHalfTile * (num_digits - digit_count);
	switch (op) {
	case PLUS:
		reversed_glyphs_.push_back(boost::shared_ptr<Sprite>(new Sprite(
			graphics, kSpritePath,
			units::gameToPixel(kPlusSourceX), units::gameToPixel(kOpSourceY),
			units::gameToPixel(kSourceWidth), units::gameToPixel(kSourceHeight))));
		break;
	case MINUS:
		reversed_glyphs_.push_back(boost::shared_ptr<Sprite>(new Sprite(
			graphics, kSpritePath,
			units::gameToPixel(kMinusSourceX), units::gameToPixel(kOpSourceY),
			units::gameToPixel(kSourceWidth), units::gameToPixel(kSourceHeight))));
		break;
	case NONE:
		break;
	}
}
	
void NumberSprite::draw(Graphics& graphics, units::Game x, units::Game y) {
	for (size_t i = 0; i < reversed_glyphs_.size(); ++i) {
		const units::Game offset = units::kHalfTile * (reversed_glyphs_.size() - 1 - i);
		reversed_glyphs_[i]->draw(graphics, x + offset + padding_, y);
	}
}

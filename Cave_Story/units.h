#ifndef UNITS_H_
#define UNITS_H_

#include <boost/math/constants/constants.hpp>
#include "config.h"

namespace units {

	typedef int HP;

	typedef float Game; // Float for extra precision. Intrinsic units of position.
	typedef int Pixel; // Integer for discrete units. Pixel values can be +/-
	typedef unsigned int Tile; // Also discrete, but non-negative;
	typedef unsigned int Frame; // Discrete non-negative.
	typedef float Degrees;

	typedef unsigned int MS; // Discrete Milliseconds. Unsigned int matches SDL
	typedef unsigned int FPS; // Frames per Second (Hz or 1 / Second)

	typedef float Velocity; // Game / MS
	typedef float Acceleration; // Game / MS�
	typedef float AngularVelocity; // Degrees  / MS

	namespace {
		const Game kTileSize = 32.0f;

		int round(double val) {
			return (int)(val > 0.0 ? val + 0.5 : val - 0.5);
		}
		//const double kPi = atan(1) * 4;
		const double kPi = boost::math::constants::pi<double>();
	}
	
	inline double degreesToRadians(Degrees degrees) {
		return degrees * kPi / 180.0f;
	}

	inline Pixel gameToPixel(Game game) {
		return config::getGraphicsQuality() == config::HIGH_QUALITY ? 
			Pixel(round(game)) :
			Pixel(round(game) / 2);
	}

	inline Tile gameToTile(Game game) {
		return Tile(game / kTileSize);
	}

	inline Game tileToGame(Tile tile) {
		return tile * kTileSize;
	}

	inline Pixel tileToPixel(Tile tile) {
		return gameToPixel(tileToGame(tile));
	}

	const Game kHalfTile = tileToGame(1) / 2.0f;
} // units

#endif // UNITS_H_
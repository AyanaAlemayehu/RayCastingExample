#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <string>
#include "../include/Player.h"
#include "../include/ImageFunctions.h"
#include "../include/MapControl.h"
Player::Player(float xv, float yv, float ang, float f) {
	x = xv;
	y = yv;
	angle = ang;
	fov = f;
}
Player::Player() {
	;
}

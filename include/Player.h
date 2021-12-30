#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <string>
#include "ImageFunctions.h"
#include "MapControl.h"

class Player {
public:
	float x, y, angle, fov;
	Player(float x, float y, float angle, float f);
	Player();
};
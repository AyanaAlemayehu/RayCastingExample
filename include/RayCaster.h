#pragma once
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <string>
#include "ImageFunctions.h"
#include "Player.h"


class RayCaster {

public:
	class Ray {
	public:
		float distance;
		char collision;
		Ray();
	};


	int width, height;
	Player player;
	std::vector<RayCaster::Ray> rays;//vector of distances of ray for specific frame

	std::vector<char> map_layout;

	RayCaster(std::vector<char> &ml, std::vector<uint32_t> &mi, int w, int h, Player &p, std::vector<uint32_t> ckey);

	void calcDistances();

	//renders a frame given an empty vector passeed by reference
	void renderFrame(std::vector<uint32_t> &frame);
};
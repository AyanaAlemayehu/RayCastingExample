#pragma once
#include <cstdint>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <string>
#include "ImageFunctions.h"

class Map {
	public:
		int mw, mh, lw, lh;
		size_t mapLayoutSize;
		uint32_t mapLayoutColor;
		std::vector<uint32_t> mapImage;
		std::vector<char> mapLayout;

		Map(int map_w, int map_h, std::vector<uint32_t> chunk_colors);

		void setLayout(std::vector<char> layout, size_t size, uint32_t color);
		void upsizeLayout();

		void adoptLayout();

		void overlay_map(std::vector<uint32_t>& image, const std::vector<char>& map, const size_t w, const size_t h, const size_t s, uint32_t color);


};

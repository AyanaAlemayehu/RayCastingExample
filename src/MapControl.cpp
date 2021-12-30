#include "MapControl.h"
int mw, mh, lw, lh;
size_t mapLayoutSize;
uint32_t mapLayoutColor;
std::vector<uint32_t> mapImage, chunk_color_key;
std::vector<char> mapLayout;
Map::Map(int map_w, int map_h, std::vector<uint32_t> chunk_colors) {
	mw = map_w;
	mh = map_h;
	std::vector<uint32_t> tempImage( mw * mh, chunk_colors[0]);//zeroth color is default background color
	mapImage = tempImage;
	chunk_color_key = chunk_colors;
}

void Map::setLayout(std::vector<char> layout, size_t size, uint32_t color) {
	mapLayout = layout;
	mapLayoutSize = size;
	//size is the reduction factor of the map layout with respect to the new map
	lw = mw / mapLayoutSize;
	lh = mh / mapLayoutSize;
	mapLayoutColor = color;
}
//upsizes the map automatically to the mapImage size (given that mapLayoutSize was correctly calculated/inputted)
void Map::upsizeLayout() {
	std::vector<char> newLayout;
	for (size_t i = 0; i < mw; i++) {
		for (size_t j = 0; j < mh; j++) {
			newLayout.push_back(mapLayout[(j / mapLayoutSize + (i / mapLayoutSize) * (mw / mapLayoutSize))]);//should copy over the correct pixel values
			//no idea why flipping i and j values works here
		}
	}
	mapLayoutSize /= mapLayoutSize;
	mapLayout = newLayout;
}

void Map::adoptLayout() {
	overlay_map(mapImage, mapLayout, mw, mh, mapLayoutSize, mapLayoutColor);
}

void Map::overlay_map(std::vector<uint32_t>& image, const std::vector<char>& map, const size_t w, const size_t h, const size_t s, uint32_t color) {
	assert(image.size() == w * h && map.size() == (w / s) * (h / s));// second part makes sure map is to scale
	for (size_t i = 0; i < w; i++) {
		for (size_t j = 0; j < h; j++) {
			assert((i / s + (j / s) * (w / s)) < map.size());
			if (map[(i / s + (j / s) * (w / s))] != '0')//dividing by s allows us to convert into appropriate index using integer division 
				image[i + w * j] = chunk_color_key[(map[(i / s + (j / s) * (w / s))] - '0')];
		}
	}
}

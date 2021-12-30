#pragma once
#include "ImageFunctions.h"

uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, uint8_t a) {
	return (a << 24) + (b << 16) + (g << 8) + r;
}
void unpack_color(const uint32_t color, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) {
	r = (color >> 0);
	g = (color >> 8);
	b = (color >> 16);
	a = (color >> 24);
}
void transform_color(const uint32_t color, float& r, float& g, float& b, float& a) {
	r *= (color >> 0);
	g *= (color >> 8);
	b *= (color >> 16);
	a *= (color >> 24);
}

void get_ppm_image(const std::string fileName, const std::vector<uint32_t>& image, const size_t w, const size_t h) {
	assert(image.size() == w * h);//i guess simple error checking?
	std::ofstream ofs(fileName, std::ios::binary);
	ofs << "P6\n" << w << " " << h << "\n255\n";
	for (size_t i = 0; i < h * w; ++i) {
		uint8_t r, g, b, a;
		unpack_color(image[i], r, g, b, a);
		ofs << r << g << b;//i didnt do a static cast cause i have no fucking idea why they did in the first place
	}
	ofs.close();
}
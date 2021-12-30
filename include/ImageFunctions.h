#pragma once
#include <cstdint>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <string>


uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, uint8_t a = 255);

void unpack_color(const uint32_t color, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a);

//using a float for r g b a values between 0 and 1, take the color values and multiply by said float
void transform_color(const uint32_t color, float& r, float& g, float& b, float& a);

void get_ppm_image(const std::string fileName, const std::vector<uint32_t>& image, const size_t w, const size_t h);
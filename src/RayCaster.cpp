#include "RayCaster.h"
	float distance;
	char collision;
	RayCaster::Ray::Ray() {
		;
	}

	int width, height;
	float renderDistance = 400;
	float renderNormalize = renderDistance / 10.0;
	std::vector<char> map_layout;
	Player player;
	std::vector<RayCaster::Ray> rays;//vector of distances of ray for specific frame
	std::vector<uint32_t> color_key;
	std::vector<uint32_t> map_image;
	std::vector<uint32_t> mi_alter;

	RayCaster::RayCaster(std::vector<char> &ml, std::vector<uint32_t> &mi, int w, int h, Player &p, std::vector<uint32_t> ckey) {
		map_layout = ml;
		map_image = mi;
		width = w;
		height = h;
		player = p;
		color_key = ckey;
		map_image = mi;
	}

	void RayCaster::calcDistances() {
		//rays is cleared every use
		rays = {};
		for (size_t i = 0; i < width; i++) {
			//shoots a ray for each width pixel...
			//below gives an angle for the current ray (cycles through fov)
			float angle = player.angle - player.fov / float(2) + (float(i) / float(width)) * player.fov;
			float delta = -player.fov / float(2) + (float(i) / float(width)) * player.fov;
			float c = 0;
			RayCaster::Ray ray = RayCaster::Ray::Ray();
			for (;; c += .2)//each ray moves .2 pixels per step (OPTIMIZATION HERE IS INCREASE RAY SPEED)
			{
				float x = player.x + c * cos(angle);
				float y = player.y + c * sin(angle);
				char val;

				if (!(c > renderDistance || x < 0 || y < 0 || x >= width || y >= height)) {
					val = map_layout[(int)x + width * (int)y];//i think the value overflows if i dont cast the positions as x or y here
					if (val != '0') {
						ray.distance = c*(1.0/renderDistance)*renderNormalize*cos(delta);
						ray.collision = val;
						break;
					}
				}
				else {
					val = '0';//out of bounds so set it to 1st color key val
					ray.distance = c * (1.0 / renderDistance)*renderNormalize*cos(delta);
					ray.collision = val;
					break;
				}
			}
			//finally put updated ray into vector
			rays.push_back(ray);
		}
	}

	void RayCaster::renderFrame(std::vector<uint32_t> &image) {			
		uint32_t color;
					
		uint8_t r, g, b, a;
			
		float multiplier;
			
		Ray currentRay;

		int wallHeight;

		int middle;
			
			
		//had to reverse i and j for a quick fix since array is built horizontally
		for (size_t i = 0; i < height; i++) {
			for (size_t j = 0; j < width; j++) {
				//first get verticle line corresponding to the current j value..
				currentRay = rays[j];
				wallHeight = height * (1.0/currentRay.distance);
				middle = (height - wallHeight)/2.0; //threshold on either end VISUALIZATION: --------[                      ]------ the -- is the "middle" threshould whereas the [   ]  is the vertical wall bit		
					
				//COLOR MANIPULATION FOR SHADOWS (change multiplier to 1 for no shadows)
				multiplier = 1.0 / currentRay.distance;

				if (i >= middle && i <= middle + wallHeight) {
					color = color_key[currentRay.collision - '0' + 2];
					unpack_color(color, r, g, b, a);//unpacks and multiplys pointers
					r *= multiplier;
					g *= multiplier;
					b *= multiplier;
					color = pack_color((uint8_t)r, (uint8_t)g, (uint8_t)b);
					image.push_back(color);//only draw the collision if we are in the correct (look up^ for description) range
				}
				else {
					//just draw the ground
					if (i > middle) {
						color = color_key[1];
						image.push_back(color);//using default as sky
					}
					else {
						//drawing sky
						image.push_back(color_key[0]);//using default as sky
					}
				}
			}
		}
	}
		

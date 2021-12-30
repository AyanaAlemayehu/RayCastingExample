#include <vector>
#include <cstdint>
#include <cassert>
#include <string>
#include "ImageFunctions.h"
#include "MapControl.h"
#include "RayCaster.h"
#include "Player.h"
#include <SDL.h>
#undef main

#include <SDL_image.h>
#include <stdio.h>



const double pi = 3.14159265358979323846;
//screen width must equal screen height, only use numbers divisible by 16
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;
//moving variables out to avoid reallocating memory continuously
unsigned char frame_buffer[4 * SCREEN_WIDTH * SCREEN_HEIGHT];



//SDL STUFF BELOW...

//starts up SDL n creates the window
bool init();

//Main loop flag
bool quit = false;

//event handler
SDL_Event e;

//Frees the image and shuts down SDL
void close();

//the window we rendering to (using pointer)
SDL_Window* gWindow = NULL;

//the surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//the image we will load and show on the screen
SDL_Surface* gActualImage = NULL;



//SDL FUNCTIONS BELOW
bool init() {
	//return flag
	bool success = true;

	//initialize the SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		//successful init
		//create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return success;
}

void close() {
	//Deallocate surface
	SDL_FreeSurface(gActualImage);
	gActualImage = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

//this function keeps a static global list of events for me to iterate thru
std::vector<SDL_Event>& GetFrameEvents()
{
	static std::vector<SDL_Event> frame_events;
	return frame_events;
}


int main() {
	const int width = SCREEN_WIDTH;
	const int height = SCREEN_HEIGHT;

	//color key works as follows: 0:sky, 1:ground, 2:first wall, 3:second wall, 4:third etc..
	std::vector<uint32_t> cckey{ pack_color(135, 206, 235, 0), pack_color(0, 255, 0), pack_color(216, 191, 216), pack_color(183, 65, 14), pack_color(100, 100, 100), pack_color(0, 230, 120)};
	Map game_map = Map(width, height, cckey);
	Player player = Player(96, 96, 0, pi/float(2));


	
	std::vector<char> temp_map = { '0','0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1' //sets up the map using a 32 reduction
								 ,'0','0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'
								 ,'0','0', '0', '0', '0', '0', '2', '0', '1', '2', '1', '2', '1', '2', '1', '2'
								 ,'1','1', '0', '2', '2', '3', '3', '0', '0', '0', '0', '0', '0', '0', '0', '1'
								 ,'1','1', '0', '2', '0', '0', '3', '1', '2', '1', '2', '1', '0', '0', '0', '1'
								 ,'1','1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '2', '0', '0', '0', '1'
								 ,'1','1', '0', '0', '0', '0', '0', '0', '2', '0', '0', '1', '0', '0', '0', '1'
								 ,'1','1', '0', '2', '1', '2', '1', '2', '1', '2', '1', '2', '0', '0', '0', '1'
								 ,'0','0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'
								 ,'0','0', '0', '0', '0', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'
								 ,'1','1', '0', '1', '2', '1', '2', '1', '2', '0', '0', '0', '0', '0', '0', '1'
								 ,'1','1', '0', '0', '0', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'
								 ,'1','1', '0', '0', '0', '1', '0', '0', '2', '0', '0', '0', '0', '0', '0', '1'
								 ,'1','1', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '1'
								 ,'1','1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'
								 ,'1','1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1' };

	


	game_map.setLayout(temp_map, SCREEN_WIDTH/16, pack_color(255, 255, 255));
	game_map.upsizeLayout();
	game_map.adoptLayout();


	RayCaster raycast = RayCaster(game_map.mapLayout, game_map.mapImage, width, height, player, cckey);


	//starting sdl...
	if (!init()) {
		std::cout << "\n Failed to Initialize! \n";
	}
	else {
		//succesful initialize
	}



	//main loop
	while (!quit) {


 		int speed = 4;


			//looping through event queue
			while (SDL_PollEvent(&e) != 0) {
				//user requests quit so i dont even add to queue
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else {
					//where im adding to queue
					GetFrameEvents().push_back(e);
				}
			}

			const Uint8* keyboard = SDL_GetKeyboardState(NULL);
			if (keyboard[SDL_SCANCODE_LEFT]) {
				raycast.player.angle -= .1;
			}
			else if (keyboard[SDL_SCANCODE_RIGHT]) {
				raycast.player.angle += .1;
			}
			if (keyboard[SDL_SCANCODE_UP]) {
				raycast.player.x += speed * cos(raycast.player.angle);
				raycast.player.y += speed * sin(raycast.player.angle);
			}
			else if (keyboard[SDL_SCANCODE_DOWN]) {
				raycast.player.x -= speed * cos(raycast.player.angle);
				raycast.player.y -= speed * sin(raycast.player.angle);
			}


		//regular main loop calculations done every frame go here, such as drawing the frame




;		raycast.calcDistances();
		std::vector<uint32_t> frame{};
		raycast.renderFrame(frame);

		//std::cout << "\n" << frame.size() << "\n";
		std::vector<SDL_Event> events = GetFrameEvents();
		while (events.size() != 0) {
			SDL_Event a = events.back();
			events.pop_back();
			//do something with these events
		}
		
		//loading the image to this frame_buffer
		uint8_t r, g, b, a;
		for (size_t i = 0; i < frame.size(); i++) {
			unpack_color(frame[i], r, g, b, a);
			frame_buffer[i * 4] = r;
			frame_buffer[i * 4 + 1] = g;
			frame_buffer[i * 4 + 2] = b;
			frame_buffer[i * 4 + 3] = a;
		}
		



		gActualImage = SDL_CreateRGBSurfaceFrom((void*)frame_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 4 * SCREEN_WIDTH, 0, 0, 0, 0);
		SDL_BlitSurface(gActualImage, NULL, gScreenSurface, NULL);
		SDL_UpdateWindowSurface(gWindow);
		//total events for the frame cleared here
		GetFrameEvents().clear();

		//capped to run at 60 frames per second
		SDL_Delay(16);
	}



	return 0;

}


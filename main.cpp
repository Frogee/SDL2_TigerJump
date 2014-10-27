//Most of this code originates from or was inspired by the Lazy Foo tutorials: http://lazyfoo.net/tutorials/SDL/index.php

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTextureTiger = NULL;
SDL_Texture* gTextureBackground = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Tiger Jump!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	gTextureTiger = loadTexture( "assets/tigerv001.png" );
	gTextureBackground = loadTexture( "assets/tigerbackv001.png");

	if( gTextureTiger == NULL )
	{
		printf( "Failed to load texture Tiger image!\n" );
		success = false;
	}
    if( gTextureBackground == NULL )
	{
		printf( "Failed to load texture Background image!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( gTextureTiger );
	SDL_DestroyTexture( gTextureBackground);
	gTextureTiger = NULL;
	gTextureBackground = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event event;

            SDL_Rect DestR;

            DestR.x = SCREEN_WIDTH / 2; //640 / 2 - SHAPE_SIZE / 2;
            DestR.y = SCREEN_HEIGHT / 3; //580 / 2 - SHAPE_SIZE / 2;
            DestR.w = SCREEN_WIDTH / 4; //SHAPE_SIZE;
            DestR.h = SCREEN_HEIGHT / 2; //SHAPE_SIZE;
            int bottom = SCREEN_HEIGHT - 10;
            printf("Bottom is: %d", bottom);
			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &event ) != 0 )
				{
					//User requests quit
					if( event.type == SDL_QUIT )
					{
						quit = true;
					}
                    if (event.type == SDL_KEYDOWN) {
                        printf( "Keyboard event!\n" );
                        if (event.key.keysym.sym == SDLK_DOWN) {
                            DestR.y = DestR.y + 5;
                        }
                        if (event.key.keysym.sym == SDLK_UP) {
                            DestR.y = DestR.y - 5;
                        }
                        if (event.key.keysym.sym == SDLK_LEFT) {
                            DestR.x = DestR.x - 5;
                        }
                        if (event.key.keysym.sym == SDLK_RIGHT) {
                            DestR.x = DestR.x + 5;
                        }
                        if (event.key.keysym.sym == SDLK_SPACE) {
                            DestR.y = DestR.y - 75;
                        }
                    }
                    if (event.type == SDL_MOUSEBUTTONDOWN) {
                        printf( "Mousebutton event!\n" );
                    }
                    if (event.type == SDL_MOUSEMOTION) {
                        printf( "Mousemotion event!\n" );
                    }
				}

                printf("DestR.y is: %d\n", DestR.y);
                if ((DestR.y + 250) < bottom) {
                    DestR.y = DestR.y + 1;
                    printf("DestR.y after gravity is: %d\n", DestR.y);
                }
				//Clear screen
				SDL_RenderClear( gRenderer );



				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTextureBackground, NULL, NULL );
				SDL_RenderCopy( gRenderer, gTextureTiger, NULL, &DestR );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

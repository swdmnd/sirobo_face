#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <cstdlib>
#include "LTexture.hpp"

//Screen dimension constants
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 720

#define SPRITE_W 600
#define SPRITE_H 600

#define SPRITESHEET_PATH "img/spritesheet_600.png"

enum Sprites{
    EYE_OPEN,
    EYE_CLOSED,
    EYE_CLOSED_STRAIGHT,
    EYE_CLOSED_KU,
    MOUTH_CLOSED,
    MOUTH_OPEN,
};

SDL_Rect gSpriteClips[7];

int screen_width = 0;
int screen_height = 0;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool isMouthOpen = false;

LTexture gSpriteSheetTexture;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

void setSprite(int index, int x, int y, int w, int h);

int selected_display = 0;

int main( int argc, char* args[] ){
    //Start up SDL and create window
    if(argc > 0){
        selected_display = atoi(args[0]);
    }
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
            //Hack to get window to stay up
            SDL_Event e;
            bool quit = false; 
            while( quit == false ){ 
                while( SDL_PollEvent( &e ) ){ 
                    if( e.type == SDL_QUIT ) quit = true; 
                    else if(e.type = SDL_KEYDOWN) {
                        switch (e.key.keysym.sym)
                        {
                            case SDLK_q:
                                quit = true;
                                break;
                            
                            default:
                                break;
                        }
                    }

                    isMouthOpen = !isMouthOpen;

                    //Clear screen
                    SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
                    SDL_RenderClear( gRenderer );

                    SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;

                    //Render top left sprite
                    gSpriteSheetTexture.render( 0, 0, &gSpriteClips[ EYE_CLOSED_KU ] );
                    //Render top left sprite
                    gSpriteSheetTexture.renderEx( 750, 0, &gSpriteClips[ EYE_CLOSED_KU ], 0, NULL,  flip);
                    //Render mouth
                    gSpriteSheetTexture.render( screen_width/2 - 300, screen_height - 600, &gSpriteClips[ isMouthOpen ? MOUTH_OPEN : MOUTH_CLOSED ] );

                    //Update screen
                    SDL_RenderPresent( gRenderer );

                    SDL_Delay(rand() % 300 + 150);
                } 
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}

bool init(){
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        // choose display
        // enumerate displays
        int displays = SDL_GetNumVideoDisplays();

        // get display bounds for all displays
        SDL_Rect displayBounds[4];
        for( int i = 0; i < displays && i < 4; i++ ) {
            displayBounds[i] = SDL_Rect();
            SDL_GetDisplayBounds( i, &displayBounds[i] );
        }
        int winx = 0;
        int winy = 0;
        int winw = 0;
        int winh = 0;
        // set window position based on display count
        if(selected_display < (displays - 1)) {
            winx = displayBounds[selected_display].x;
            winy = displayBounds[selected_display].y;
            winw = displayBounds[selected_display].w;
            winh = displayBounds[selected_display].h;
        }


        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", winx, winy, winw, winh, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Set window fullscreen
            SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);

            //Set params
            SDL_GetWindowSize(gWindow, &screen_width, &screen_height);

            //Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                gSpriteSheetTexture.setRenderer(gRenderer);

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

    //Load sprite sheet texture
    if( !gSpriteSheetTexture.loadFromFile( SPRITESHEET_PATH ) )
    {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        setSprite(EYE_OPEN, 0, 0, SPRITE_W, SPRITE_H);
        setSprite(EYE_CLOSED, 600, 0, SPRITE_W, SPRITE_H);
        setSprite(EYE_CLOSED_STRAIGHT, 0, 600, SPRITE_W, SPRITE_H);
        setSprite(EYE_CLOSED_KU, 600, 600, SPRITE_W, SPRITE_H);
        setSprite(MOUTH_CLOSED, 0, 1200, SPRITE_W, SPRITE_H);
        setSprite(MOUTH_OPEN, 600, 1200, SPRITE_W, SPRITE_H);
    }

    return success;
}

void setSprite(int index, int x, int y, int w, int h){
    gSpriteClips[index].x = x;
    gSpriteClips[index].y = y;
    gSpriteClips[index].w = w;
    gSpriteClips[index].h = h;
}

void close()
{
    // free the sprites
    gSpriteSheetTexture.free();

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}
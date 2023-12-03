#ifndef __LTEXTURE_H__
#define __LTEXTURE_H__

#include <string>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

class LTexture {
    private:
        SDL_Texture* mTexture;
        SDL_Renderer* mRenderer;
        int mWidth;
        int mHeight;

    public:
        LTexture();
        ~LTexture();

        void setRenderer( SDL_Renderer* renderer ) { mRenderer = renderer; }

        bool loadFromFile( string path );

        void free();

        void render( int x, int y, SDL_Rect* clip = NULL );
        void renderEx( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip );

        int getWidth();
        int getHeight();
};

#endif

#ifndef HEADER_SDLMOVIE_H
#define HEADER_SDLMOVIE_H
#ifdef HAVE_SMPEG

#include "SDL.h"

#include "smpeg/smpeg.h"

#include "Drawable.h"
#include "Log.h"

/**
 * Thanks to Drew Benton for the initial code:
 * http://www.gamedev.net/community/forums/topic.asp?topic_id=348340
 */
class SDL_Movie : public Drawable
{
private:
    // Surface for the movie
    SDL_Surface *movieSurface;

    // Holds the movie information
    SMPEG_Info movieInfo;

    // Load the movie
    SMPEG *movie;

    // The max we can scale by
    int MaxScaleX;
    int MaxScaleY;
    int MaxScale;

public:
    SDL_Movie()
    {
        MaxScaleX = MaxScaleY = MaxScale = 1;

        movieSurface = 0;
        movie = 0;
    }

    // Free our movie
    virtual ~SDL_Movie()
    {
        if (movie) {
            Stop();
            SMPEG_delete( movie );
            movie = 0;
        }
        if (movieSurface) {
            SDL_FreeSurface( movieSurface );
            movieSurface = 0;
        }
    }

    void ClearScreen()
    {
        SDL_FillRect( movieSurface, 0, 0 );
    }

    // Set's the volume on a scale of 0 - 100
    void SetVolume( int vol )
    {
        SMPEG_setvolume( movie, vol );
    }

    // Scale the movie by the desired factors
    void Scale( int w, int h )
    {
        // Prevent a divide by 0
        if( w == 0 )
            w = 1;
        if( h == 0 )
            h = 1;

        SMPEG_scaleXY( movie, w, h );
        CheckErrors();
    }

    // Scale the movie by the desired factor
    void ScaleBy( int factor )
    {
        // Prevent a divide by 0
        if( factor == 0 )
            factor = 1;
        // Make sure we don't scale larger than the surface size
        if( factor > MaxScale )
            factor = MaxScale;

        SMPEG_scale( movie, factor );
        CheckErrors();
    }

    // Sets the region of the video to be shown
    void SetDisplayRegion( int x, int y, int w, int h )
    {
        SMPEG_setdisplayregion( movie, x, y, w, h );
        CheckErrors();
    }

    // Check for any errors
    void CheckErrors()
    {
        char* error = SMPEG_error( movie );
        if( error ) {
            LOG_WARNING(ExInfo("SMPEG_error")
                    .addInfo("error", error));
        }
    }

    // Load the movie
    void Load( const char* fileName, int maxscalex = 1, int maxscaley = 1 )
    {
        MaxScaleX = maxscalex;
        MaxScaleY = maxscaley;
        
        // Limit how much we can scale by
        MaxScale = (maxscalex > maxscaley ? maxscaley : maxscalex);

        // Load the movie and store the information about it
        movie = SMPEG_new( fileName, &movieInfo, true );
        CheckErrors();

        SDL_Surface *screen = SDL_GetVideoSurface();
        // Create a temporary surface to render the movie to
        SDL_Surface *tempSurface2 = SDL_CreateRGBSurface( SDL_SWSURFACE, movieInfo.width * MaxScaleX, movieInfo.height * MaxScaleY, 32, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask );

        // Now make a surface optimized for the main screen
        movieSurface = SDL_DisplayFormat( tempSurface2 );

        // Free the temporary surface
        SDL_FreeSurface( tempSurface2 );

        // Set the surface to draw to
        SMPEG_setdisplay( movie, movieSurface, 0, 0 );
        CheckErrors();

        // Set the display region
        SMPEG_setdisplayregion( movie, 0, 0, movieInfo.width, movieInfo.height );
        CheckErrors();
    }

    // Set the looping of hte movie
    void SetLoop( int val )
    {
        SMPEG_loop( movie, val );
        CheckErrors();
    }

    // Play the movie
    void Play()
    {
        SMPEG_play( movie );
        CheckErrors();
    }

    // Pause the movie
    void Pause()
    {
        SMPEG_pause( movie );
        CheckErrors();
    }

    // Stops the movie, but keeps current position
    void Stop()
    {
        SMPEG_stop( movie );
        CheckErrors();
    }

    // Rewind the movie back to 0:00:00
    void Rewind()
    {
        SMPEG_rewind( movie );
        CheckErrors();
    }

    // Seek a number of bytes into the movie
    void Seek( int bytes )
    {
        SMPEG_seek( movie, bytes );
        CheckErrors();
    }

    // Skip a number of seconds
    void Skip( float seconds )
    {
        SMPEG_skip( movie, seconds );
        CheckErrors();
    }

    // Render some frame of the movie
    void RenderFrame( int frame )
    {
        SMPEG_renderFrame( movie, frame );
        CheckErrors();
    }

    // Render the final frame of the movie
    void RenderFinal()
    {
        SMPEG_renderFinal( movie, movieSurface, 0, 0 );
        CheckErrors();
    }

    // Draw the movie surface to the screen
    virtual void drawOn(SDL_Surface *screen)
    {
        if (movieSurface) {
            SDL_BlitSurface(movieSurface, NULL, screen, NULL);
        }
    }

    // Return the current info for the movie
    SMPEG_Info GetInfo()
    {
        SMPEG_getinfo( movie, &movieInfo );
        return movieInfo;
    }

    // Get the current status of the movie, can be SMPEG_ERROR = -1, SMPEG_STOPPED, SMPEG_PLAYING
    SMPEGstatus GetStatus()
    {
        return SMPEG_status(movie);
    }
};

#endif
#endif


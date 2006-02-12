/* 

 SDL_gfxPrimitives: graphics primitives for SDL

 LGPL (c) A. Schiffler
 
*/

#ifndef _SDL_gfxPrimitives_h
#define _SDL_gfxPrimitives_h

#include <math.h>
#ifndef M_PI
#define M_PI	3.141592654
#endif

#include "SDL.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* ----- Versioning */

#define SDL_GFXPRIMITIVES_MAJOR	2
#define SDL_GFXPRIMITIVES_MINOR	0
#define SDL_GFXPRIMITIVES_MICRO	10

/* ----- Prototypes */

/* Note: all ___Color routines expect the color to be in format 0xRRGGBBAA */

/* Pixel */

    extern int pixelColor(SDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color);
    extern int pixelRGBA(SDL_Surface * dst, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Horizontal line */

    extern int hlineColor(SDL_Surface * dst, Sint16 x1, Sint16 x2, Sint16 y, Uint32 color);
    extern int hlineRGBA(SDL_Surface * dst, Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Vertical line */

    extern int vlineColor(SDL_Surface * dst, Sint16 x, Sint16 y1, Sint16 y2, Uint32 color);
    extern int vlineRGBA(SDL_Surface * dst, Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Rectangle */

    extern int rectangleColor(SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
    extern int rectangleRGBA(SDL_Surface * dst, Sint16 x1, Sint16 y1,
				   Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled rectangle (Box) */

    extern int boxColor(SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
    extern int boxRGBA(SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2,
			     Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Line */

    extern int lineColor(SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
    extern int lineRGBA(SDL_Surface * dst, Sint16 x1, Sint16 y1,
			      Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA Line */
    extern int aalineColor(SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
    extern int aalineRGBA(SDL_Surface * dst, Sint16 x1, Sint16 y1,
				Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Circle */

    extern int circleColor(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 r, Uint32 color);
    extern int circleRGBA(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA Circle */

    extern int aacircleColor(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 r, Uint32 color);
    extern int aacircleRGBA(SDL_Surface * dst, Sint16 x, Sint16 y,
				  Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Circle */

    extern int filledCircleColor(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 r, Uint32 color);
    extern int filledCircleRGBA(SDL_Surface * dst, Sint16 x, Sint16 y,
				      Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Ellipse */

    extern int ellipseColor(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
    extern int ellipseRGBA(SDL_Surface * dst, Sint16 x, Sint16 y,
				 Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA Ellipse */

    extern int aaellipseColor(SDL_Surface * dst, Sint16 xc, Sint16 yc, Sint16 rx, Sint16 ry, Uint32 color);
    extern int aaellipseRGBA(SDL_Surface * dst, Sint16 x, Sint16 y,
				   Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Ellipse */

    extern int filledEllipseColor(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color);
    extern int filledEllipseRGBA(SDL_Surface * dst, Sint16 x, Sint16 y,
				       Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
/* Filled Pie */

    extern int filledpieColor(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
				    Sint16 start, Sint16 end, Uint32 color);
    extern int filledpieRGBA(SDL_Surface * dst, Sint16 x, Sint16 y, Sint16 rad,
				   Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Trigon */

    extern int trigonColor(SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color);
    extern int trigonRGBA(SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
				 Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA-Trigon */

    extern int aatrigonColor(SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color);
    extern int aatrigonRGBA(SDL_Surface * dst,  Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
				   Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Trigon */

    extern int filledTrigonColor(SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, int color);
    extern int filledTrigonRGBA(SDL_Surface * dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
				       Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Polygon */

    extern int polygonColor(SDL_Surface * dst, Sint16 * vx, Sint16 * vy, int n, Uint32 color);
    extern int polygonRGBA(SDL_Surface * dst, Sint16 * vx, Sint16 * vy,
				 int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA-Polygon */

    extern int aapolygonColor(SDL_Surface * dst, Sint16 * vx, Sint16 * vy, int n, Uint32 color);
    extern int aapolygonRGBA(SDL_Surface * dst, Sint16 * vx, Sint16 * vy,
				   int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Polygon */

    extern int filledPolygonColor(SDL_Surface * dst, Sint16 * vx, Sint16 * vy, int n, int color);
    extern int filledPolygonRGBA(SDL_Surface * dst, Sint16 * vx,
				       Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Bezier */
/* s = number of steps */

    extern int bezierColor(SDL_Surface * dst, Sint16 * vx, Sint16 * vy, int n, int s, Uint32 color);
    extern int bezierRGBA(SDL_Surface * dst, Sint16 * vx, Sint16 * vy,
				 int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif				/* _SDL_gfxPrimitives_h */

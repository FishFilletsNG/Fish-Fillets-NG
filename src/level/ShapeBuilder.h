#ifndef HEADER_SHAPE_BUILDER_H
#define HEADER_SHAPE_BUILDER_H

class Shape;

#include "Cube.h"

#include "SDL.h"

/**
 * Creator of images from shapes.
 * Useable for testing of new levels.
 */
class ShapeBuilder {
    private:
        /**
        * ShapeBuilder is not meant to be instantiated.
         */
        ShapeBuilder() {}
    public:
        static SDL_Surface *createImage(const Shape *shape, Cube::eWeight weight);
};

#endif

#ifndef FILTERS_H
#define FILTERS_H

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
Uint32 negativefilter(Uint32 pixel_color, SDL_PixelFormat* format);

#endif

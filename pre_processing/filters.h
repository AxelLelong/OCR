#ifndef FILTERS_H
#define FILTERS_H

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
Uint32 negativefilter(Uint32 pixel_color, SDL_PixelFormat* format);
Uint32 contrastefilter(Uint32 pixel_color, SDL_PixelFormat* format);
Uint32 NormLight(Uint32 pixel_color, SDL_PixelFormat* format, Uint8 m);
Uint32 medianfilter(Uint32* pixels,int i,SDL_PixelFormat* format,int w, int h);
#endif

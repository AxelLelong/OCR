#ifndef FILTERS_H
#define FILTERS_H

Uint8 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
Uint8 negativefilter(Uint8 pixel_color);
Uint8 contrastefilter(Uint8 pixel_color);
void NormLight(Uint8* pixels, int len, Uint8 m);
void medianfilter(Uint8* pixels,Uint8* pixels1,int w, int h);
Uint8 get_max(Uint8* pixels, int len);
void GaussianFlou(Uint8* pixels, Uint8* pixels1,int w, int h);
void adaptativeThreshold(Uint8* pixels,double seuil, int w ,int h);
float noiseLevel(Uint8* pixels,int w, int h);
void lissage(Uint8* pixels,Uint8* pixels1,int w, int h);
#endif

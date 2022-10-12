//
// Created by mathieu on 12/10/2022.
//

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

Uint32 interpolation(unsigned int top,unsigned int bottom,unsigned int left,unsigned int right,
                     double horizontal_position,double vertical_position,
                     Uint32* pixels,SDL_PixelFormat* format,int w);

#endif //INTERPOLATION_H

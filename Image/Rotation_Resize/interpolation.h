//
// Created by mathieu on 12/10/2022.
//

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

Uint32 interpolation(Uint32 top, Uint32 bottom,Uint32 left, Uint32 right,
                     double horizontal_position,double vertical_position,
                     Uint32* pixels,SDL_PixelFormat* format);

#endif //INTERPOLATION_H

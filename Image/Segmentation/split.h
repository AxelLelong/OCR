//
// Created by mathieu on 16/11/2022.
//

#ifndef LIGN_DETECTION_C_SPLIT_H
#define LIGN_DETECTION_C_SPLIT_H

SDL_Surface* cropImage(SDL_Surface* surface, SDL_Rect* rect);
void split(SDL_Surface* surface, SDL_Surface** segmentation);

#endif //LIGN_DETECTION_C_SPLIT_H

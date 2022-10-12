//
// Created by mathi on 12/10/2022.
//

#ifndef DISPLAY_H
#define DISPLAY_H

void draw(SDL_Renderer* renderer, SDL_Texture* texture);
void save_image(SDL_Surface *image_surface, char *path);
void event_loop(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Surface * image_surface,char* path);
SDL_Surface* load_image(const char* path);

#endif //DISPLAY_H

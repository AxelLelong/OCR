#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);
}

void save_image(SDL_Surface *image_surface, char *path)
{
    printf("<--💾 Saving output to %s\n", path);

    if (IMG_SavePNG(image_surface, path) != 0)
        errx(EXIT_FAILURE, "could not save the image to '%s': %s.\n", path,
            SDL_GetError());
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Surface * image_surface,char* path)
{
    SDL_Event event;

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
	        save_image(image_surface,path);
	        return;

            // If the window is resized, updates and redraws the image.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
		    draw(renderer,texture);
                }
                break;
        }
    }
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface * source = IMG_Load(path);
    if (source == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* surface = SDL_ConvertSurfaceFormat
    (source, SDL_PIXELFORMAT_RGB888, 0);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(source);
    return surface;
}

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = 0.3*r + 0.59*g + 0.11*b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

Uint32 pixel_to_negative(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = 0.3*r + 0.59*g + 0.11*b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
    for(i = 0; i < h; i++)
      for(j = 0; j < w; j++)
	{
	  color = SDL_GetRGB(p[i * w + j], s->format, &color.r, &color.g, &color.b);
	  pixels[i * w + j] = SDL_MapRGB(s->format, 255 - color.r, 255 - color.g, 255 - color.b);
	}
}


void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_LockSurface(surface);
    for(int i = 0;i<len;i++)
    {
        pixels[i] = pixel_to_grayscale(pixels[i],format);
    }
    SDL_UnlockSurface(surface);
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file save-path");
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Converting into Grayscale", 0, 0, 640, 400,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window,surface->w,surface->h);

    // - Convert the surface into grayscale.
    surface_to_grayscale(surface);

    // - Create a texture from the image.
    SDL_Texture* texturegrayscale = SDL_CreateTextureFromSurface(renderer,surface);
    if (texturegrayscale == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Dispatch the events.
    event_loop(renderer,texturegrayscale,surface,argv[2]);

    // - Free the surface.
    SDL_FreeSurface(surface);
    
    // - Destroy the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texturegrayscale);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}

#include "sobel.h"
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

double Convolution(SDL_Surface *image, double kernel[3][3], int row, int col)
{
  Uint32 *pixels = image->pixels;
  Uint32 pixel;
  if(pixels == NULL)
    errx(0,"Convolution Sobel Pixels Not Found");
  double sum = 0;
  double px_value;
  //unsigned int r;
  Uint8 r,g,b;
  int h = (int)image->h;
  int w = (int)image->w;
  for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < 3; j++)
        {
	  int x = i + row;
	  int y = j + col;

	  //check if we're inside of the picture
	  if (x >= 0 && y >= 0 && x < w
	      && y < h)
	    {
	      SDL_GetRGB(pixels[y*w+x], image->format,&r,&g,&b);
	      sum += r * kernel[i][j];
	    }
	  
        }
    }
  return sum;
}

void SobelEdgeDetection(SDL_Surface *image)
{
  Uint32 *pixels = image->pixels;
  Uint32 pixel;
  if(pixels == NULL)
    errx(0,"SobelEdgeDetection Pixels Not Found");
  double gx, gy;
  double g_px;
  //Uint8 r,g,b;
  double kernel_x[3][3] = { { -1.0, 0.0, 1.0 },
			    { -2.0, 0.0, 2.0 },
			    { -1.0, 0.0, 1.0 } };
  
  double kernel_y[3][3] = { { -1.0, -2.0, -1.0 },
			    { 0.0, 0.0, 0.0 },
			    { 1.0, 2.0, 1.0 } };

  const unsigned int h = image->h;
  const unsigned int w = image->w;
  for (unsigned int i = 0; i < h; i++)
    {
      for (unsigned int j = 0; j < w; j++)
        {
	  gx = Convolution(image, kernel_x, i, j);
	  gy = Convolution(image, kernel_y, i, j);
	  g_px = sqrt(gx * gx + gy * gy);

	  //update pixel
	  if(g_px > 128)
	    {
	      pixel = SDL_MapRGB(image->format, 255,255,255);
	      pixels[i*w+j] = pixel;
	    }
	  else
	    {
	      pixel = SDL_MapRGB(image->format, 0,0,0);
	      pixels[i*w+j] = pixel;
	    }
	  /*
	  SDL_GetRGB(pixels[j*width+i], image->format,&r,&g,&b);
	  r = g_px > 128 ? 255 : 0;
	  g = g_px > 128 ? 255 : 0;
	  b = g_px > 128 ? 255 : 0;*/
        }
    }
}

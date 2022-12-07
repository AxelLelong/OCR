//
// Created by mathieu on 12/10/2022.
//

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "filters.h"
#include "transformation.h"
#include "../Detection/lign_detection.h"
#include "../Detection/square_detection.h"
#include "../Perspective/correct_perspective.h"
#include "../Detection/sobel.h"
#include "../Display/display.h"
#include "../Segmentation/split.h"
#include "../Rotation_Resize/rotation.h"

void transformation(SDL_Surface* surface, SDL_Surface** segmentation)
{

    /// ------ INITIALIZE VARIABLES ------
    int w = surface->w;
    int h = surface->h;
    int len = w * h;
    /// ----------------------------------


    /// ------ SET IMAGE ------
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_LockSurface(surface);

    Uint32* pixels1 = malloc(len*sizeof(Uint32));
    if (pixels1 == NULL)
        errx(EXIT_FAILURE, "C'est de la faute de pixels1 pendant le malloc");

    Uint32* pixels2 = malloc(len*sizeof(Uint32));
    if (pixels2 == NULL)
        errx(EXIT_FAILURE, "C'est de la faute de pixels2 pendant le malloc");
    /// -----------------------


    /// ------ GRAYSCALE & CONTRASTE -------
    for(int i = 0;i<len;i++)
    {
        Uint32 tmp = pixel_to_grayscale(pixels[i],format);
        pixels[i] = contrastefilter(tmp,format);
    }
    //save_image(surface,"test_grayscale_contrast.png");

    /// ------------------------------------


    /// ------ LIGHT NORMALIZATION ------
    Uint8 max = get_max(pixels,len,format);
    NormLight(pixels, format, len, max);
    //save_image(surface,"test_light_normalisation.png");
    /// ---------------------------------


    /// ------ MEDIAN FILTER ------
    medianfilter(pixels,pixels1,format,w,h);
    for (int i = 0; i < len ; ++i)
    {
        pixels[i] = pixels1[i];
    }
    //save_image(surface,"test_median_filter.png");
    /// ---------------------------


    /// ------ GAUSSIAN BLUR ------
    GaussianFlou(pixels,pixels1,format,w,h);
    for (int i = 0; i < len ; ++i)
    {
        pixels[i] = pixels1[i];
    }
    //save_image(surface,"test_gaussian_blur.png");
    /// ---------------------------


    /// ------ BINARISATION ------
    //global noise of the picture
    float noise = noiseLevel(pixels,w,h, format);
    double seuil;
    //seuil adaptatif
    if(noise>20)
        seuil = 0.5;
    else if(noise == 0 && w>1100 && w<1400)
        seuil = 0.26;
    else if(noise == 0 && w>=1400)
        seuil = 0.5;
    else if(noise == 0 && w<=1100)
        seuil = 0.15;
    else
        seuil = 0.15;
    adaptativeThreshold(pixels,seuil,w,h, format);
    //save_image(surface,"test_binarisation.png");
    /// -------------------------


    /// ------ SMOOTHING ------
    lissage(pixels,pixels1,w,h,format);
    for (int i = 0; i < len ; ++i)
    {
        pixels[i] = pixels1[i];
    }
    //save_image(surface,"test_smoothing.png");
    /// -----------------------


    /// ------ SOBEL ------
    for (int i = 0; i < len ; ++i)
    {
        pixels2[i] = pixels[i];
    }
    SobelEdgeDetection(surface);
    save_image(surface,"test_sobel.png");
    /// -------------------


    /// ------ HOUGH TRANSFORM ------
    double *max_Theta = malloc(sizeof(double));
    int *lenliste = malloc(sizeof(int));
    int** allLines = houghtransform(pixels,w, h, format,0,max_Theta,lenliste);
    for (int i = 0; i < len ; ++i)
    {
        pixels1[i] = pixels[i];
    }
    int lenRes;
    for(int i = 0;i<*lenliste;i++)
    {
        draw_line(pixels, w, h, allLines[i][0],allLines[i][1],allLines[i][2],allLines[i][3], SDL_MapRGB(format, 40, 40, 200), 1, 1,format);
    }
    //save_image(surface,"test_hough_transform.png");
    for (int i = 0; i < len ; ++i)
    {
        pixels[i] = pixels1[i];
    }
    /// -----------------------------


    /// ------ LINES SIMPLIFICATION ------
    int** lines = LineSimpl(allLines, lenliste, 50,&lenRes);
    // Draw lines
    for (int i = 0; i < len ; ++i)
    {
        pixels1[i] = pixels[i];
    }
    for(int i = 0;i<lenRes;i++)
    {
        draw_line(pixels, w, h, lines[i][0],lines[i][1],lines[i][2],lines[i][3], SDL_MapRGB(format, 40, 40, 200), 1, 1,format);
    }
    save_image(surface,"test_simplify.png");
    for (int i = 0; i < len ; ++i)
    {
        pixels[i] = pixels1[i];
    }
    /// -----------------------------

    /// ------------- AUTO ROTATE --------------
    for (int i = 0; i < len; i++)
    {
        pixels[i] = pixels2[i];
    }
    save_image(surface,"test_beforerot.png");
    double angle = *max_Theta * 180.0 / M_PI;
    int angleRounded = (int)angle % 90; // ROTATE
    if ((angleRounded >= 85 && angleRounded <= 95)
        || (angleRounded >= 0 && angleRounded <= 5))

    {
        printf("Do not need to rotate image");
    }
    else
    {
        rotateAll(surface,lines, angleRounded,lenRes);
    }
    save_image(surface,"test_afterrot.png");
    /// ------------------------------------------

    /// ------ DETECTION CARRE ------
    int** square = findSquare(lines,w,h,&lenRes);
    for (int i = 0; i < len; i++)
    {
        pixels1[i] = pixels[i];
    }
    compute_Square(square);
    drawSquare(square, pixels, w,h, 2,format,1);

    save_image(surface,"test_carre.png");
    ///------------------------------


    /// ------ CORRECT PERSPECTIVE ------
    for (int i = 0; i < len ; ++i)
    {
        pixels[i] = negativefilter(pixels1[i], format);
    }
    correctPerspective(square, surface, w,h);
    save_image(surface,"test_pers.png");
    ///----------------------------------


    /// ------ SEGMENTATION & CLEAR ------
    split(surface, segmentation);
    ///-----------------------------------

    free(pixels1);
    free(pixels2);
    /*for(int i = 0;i<lenRes;i++)
      free(lines[i]);*/
    free(lines);
    for(int i = 0;i<*lenliste;i++)
        free(allLines[i]);
    free(allLines);
    free(lenliste);
    for(int i = 0;i<4;i++)
        free(square[i]);
    free(square);
    free(max_Theta);
    //free(segmentation);
    SDL_UnlockSurface(surface);
}

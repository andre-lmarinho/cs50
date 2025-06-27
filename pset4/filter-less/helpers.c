#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            //Finding Grayscale values
            int avg = round((image[row][col].rgbtRed + image[row][col].rgbtGreen + image[row][col].rgbtBlue) / 3.000);

            //Switching image values
            image[row][col].rgbtRed = image[row][col].rgbtGreen = image[row][col].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // Finding Sepia values
            int sepiaRed = round(.393 * image[row][col].rgbtRed + .769 * image[row][col].rgbtGreen + .189 * image[row][col].rgbtBlue);
            int sepiaGreen = round(.349 * image[row][col].rgbtRed + .686 * image[row][col].rgbtGreen + .168 * image[row][col].rgbtBlue);
            int sepiaBlue = round(.272 * image[row][col].rgbtRed + .534 * image[row][col].rgbtGreen + .131 * image[row][col].rgbtBlue);

            // Switching Immage with values, cant be more than 255
            image[row][col].rgbtRed = sepiaRed > 255 ? 255 : sepiaRed;
            image[row][col].rgbtGreen = sepiaGreen > 255 ? 255 : sepiaGreen;
            image[row][col].rgbtBlue = sepiaBlue > 255 ? 255 : sepiaBlue;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempReflect[height][width];

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {

            tempReflect[row][col] = image[row][col];

        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int col = 0, lastcol = width - 1; col < width; col++)
        {

            image[row][col] = tempReflect[row][lastcol];
            lastcol --;

        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Creating a temp Imagem
    RGBTRIPLE tempBlur[height][width];

    //Duplicating the original values to the temp Image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {

            tempBlur[row][col] = image[row][col];

        }
    }

    float tempBlurRed = 0;
    float tempBlurGreen = 0;
    float tempBlurBlue = 0;

    int countBlur = 0;


    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {

            // Finding new array for creating the values
            for (int arrrow = row - 1; arrrow <= row + 1; arrrow++)
            {
                for (int arrcol = col - 1; arrcol <= col + 1; arrcol++)
                {
                    if (arrrow < height && arrcol < width && arrrow >= 0 && arrcol >= 0)
                    {

                        tempBlurRed += tempBlur[arrrow][arrcol].rgbtRed;
                        tempBlurGreen += tempBlur[arrrow][arrcol].rgbtGreen;
                        tempBlurBlue += tempBlur[arrrow][arrcol].rgbtBlue;
                        countBlur++;
                    }
                }
            }

            //Swap avrgs of array on the image
            image[row][col].rgbtRed = round(tempBlurRed / countBlur);
            image[row][col].rgbtGreen = round(tempBlurGreen / countBlur);
            image[row][col].rgbtBlue = round(tempBlurBlue / countBlur);

            //Reset the values
            tempBlurRed = tempBlurGreen = tempBlurBlue = 0;
            countBlur = 0;

        }
    }

    return;
}

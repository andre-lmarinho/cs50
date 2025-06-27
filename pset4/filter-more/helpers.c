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




    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {

            float tempBlurRed = 0;
            float tempBlurGreen = 0;
            float tempBlurBlue = 0;

            int countBlur = 0;

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


        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Creating a temp Imagem
    RGBTRIPLE tempEdges[height][width];

    //Duplicating the original values to the temp Image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {

            tempEdges[row][col] = image[row][col];

        }
    }

    //Creating the matrix for X usage
    int gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    //Creating the matrix for Y usage
    int gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };


    //Creating the matrix from the image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {

            //Creating the matrix for the Image
            int rowm[3] = {row - 1, row, row + 1};
            int colm[3] = {col - 1, col, col + 1};

            int tempEdgeRedX = 0;
            int tempEdgeGreenX = 0;
            int tempEdgeBlueX = 0;

            int tempEdgeRedY = 0;
            int tempEdgeGreenY = 0;
            int tempEdgeBlueY = 0;

            // Finding array for pulling the values
            for (int arrrow = 0; arrrow < 3; arrrow++)
            {
                for (int arrcol = 0; arrcol < 3; arrcol++)
                {
                    int rown = rowm[arrrow];
                    int coln = colm[arrcol];

                    RGBTRIPLE pixel = tempEdges[rown][coln];

                    if (rown < height && coln < width && rown >= 0 && coln >= 0)
                    {
                        tempEdgeRedX += pixel.rgbtRed * gx[arrrow][arrcol];
                        tempEdgeGreenX += pixel.rgbtGreen * gx[arrrow][arrcol];
                        tempEdgeBlueX += pixel.rgbtBlue * gx[arrrow][arrcol];

                        tempEdgeRedY += pixel.rgbtRed * gy[arrrow][arrcol];
                        tempEdgeGreenY += pixel.rgbtGreen * gy[arrrow][arrcol];
                        tempEdgeBlueY += pixel.rgbtBlue * gy[arrrow][arrcol];

                    }

                }
            }

            //Saving Edge Values
            int EdgeRed = round(sqrt(tempEdgeRedX * tempEdgeRedX + tempEdgeRedY * tempEdgeRedY));
            int EdgeGreen = round(sqrt(tempEdgeGreenX * tempEdgeGreenX + tempEdgeGreenY * tempEdgeGreenY));
            int EdgeBlue = round(sqrt(tempEdgeBlueX * tempEdgeBlueX + tempEdgeBlueY * tempEdgeBlueY));

            // Switching Immage with values, cant be more than 255
            image[row][col].rgbtRed = EdgeRed > 255 ? 255 : EdgeRed;
            image[row][col].rgbtGreen = EdgeGreen > 255 ? 255 : EdgeGreen;
            image[row][col].rgbtBlue = EdgeBlue > 255 ? 255 : EdgeBlue;


        }
    }

    return;
}

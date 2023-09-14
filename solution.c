#include <stdio.h>
#include <stdlib.h>

// Structure to hold grayscale pixel data
typedef struct {
    unsigned char value;
} PixelGray;

// Function to read a PGM image into a 2D array
PixelGray** readPGM(const char* filename, int* width, int* height);

// Function to write a 2D matrix as a PGM image
void writePGM(const char* filename, PixelGray** matrix, int* width, int* height);

// Function to threshold the image matrix
PixelGray** threshold(PixelGray** matrix, int* width, int* height);

// Function to rotate the image matrix
PixelGray** rotate(PixelGray** matrix, int* width, int* height);


//main function - DO NOT MODIFY
int main() {
    int width, height;  // variable to hold width and height. Use reference in other functions

    PixelGray** image_original = readPGM("lenna.pgm", &width, &height);


    // Now you have the grayscale image data in the 'image_original' 2D array

    // Access pixel data using image[row][col].value
    // For example, to access the pixel at row=2, col=3:


    // Create a new 2D array 'image_thresh' to store the threshold image
    PixelGray** image_thresh = threshold(image_original, &width, &height);
    //write the image data as "threshold.pgm"
    writePGM("threshold.pgm", image_thresh, &width, &height);

    // Create a new 2D array 'image_rotate' to store the rotated image
   PixelGray** image_rotate = rotate(image_original, &width, &height);
    //write the image data as "rotate.pgm"
    writePGM("rotate.pgm", image_rotate, &width, &height);

    image_rotate = rotate(image_rotate, &width, &height);
    //write the image data as "rotate_again.pgm"
    writePGM("rotate_again.pgm", image_rotate, &width, &height);

    // Free the allocated memory when you're done
    for (int i = 0; i < height; ++i) {
        free(image_original[i]);
        free(image_thresh[i]);
        free(image_rotate[i]);
    }
    free(image_original);
    free(image_thresh);
    free(image_rotate);
    return 0;
}

PixelGray** readPGM(const char* filename, int* width, int* height) {

    //Declare variables
    FILE *image_in = NULL;
    char version[3];
    int maxGray;
    int temp;
    //open file
    image_in = fopen(filename, "rb");


    //declare 2d array
    PixelGray **matrix;

    //handle if file is not exist
    if (image_in == NULL) {
        printf("File does not exist!");
        exit(1);
    }

    // read image dimension

    fscanf(image_in, "%s", version);
    fscanf(image_in, "%d %d", &(*width), &(*height ));
    fscanf(image_in, "%d", &maxGray);
    fgetc(image_in); // eat the newline char
    //allocated 2d array
     matrix = (PixelGray **)malloc(*height *sizeof (PixelGray*));

    for (int i =0; i < *height; i++)
    {
        matrix[i] = (PixelGray*)malloc (*width * sizeof(PixelGray));
        if(matrix[i]==NULL) {
            printf("Error: Unable to allocate memory for columns in row %d\n", i);
            exit(1);
        }
    }


    //copy data into the array and handling error
    for (int i = 0; i < *height; i++) {
        for (int j =0 ; j < *width;j++)
        {
            temp = fgetc(image_in);
            if(feof(image_in))
            {
                printf("End of file");
            }
            else if (ferror(image_in))
            {
                printf("Reading Error");
            }
            else
            {
                matrix[i][j].value = temp;
            }

        }
    }



    fclose(image_in);

    return matrix;
}

PixelGray** threshold(PixelGray** matrix, int* width, int* height)
{
    //Declare pointer to 2d array
    PixelGray **threshold;

    //Dynamically Allocated 2d array
    threshold = (PixelGray **)malloc(*height *sizeof (PixelGray*));
    for (int i =0; i < *height; i++)
    {
        threshold[i] = (PixelGray*)malloc (*width * sizeof(PixelGray));
        if(threshold[i]==NULL) {
            printf("Error: Unable to allocate memory for columns in row %d\n", i);
            exit(1);
        }
    }

    //Logic to change the value
    for (int i = 0; i < *height; i++) {
        for (int j =0; j < *width; j++)
        {
            threshold[i][j].value = matrix[i][j].value;
            if(threshold[i][j].value > 80)
            {
                threshold[i][j].value = 255;
            }
            else
            {
                threshold[i][j].value = 0;
            }
        }
    }
    return threshold;
}


PixelGray** rotate(PixelGray** matrix, int* width, int* height)
{
    //Declare 2d Array
    PixelGray **rotateMatrix;

    //Dynamically allocated 2d array
    rotateMatrix = (PixelGray **)malloc(*height *sizeof (PixelGray*));

    for (int i =0; i <= *height; i++)
    {
        rotateMatrix[i] = (PixelGray*)malloc (*width * sizeof(PixelGray));
        if(rotateMatrix[i]==NULL) {
            printf("Error: Unable to allocate memory for columns in row %d\n", i);
            exit(1);
        }
    }

    // matrix transpose
    for(int i = 0; i < *height ; i++)
    {
        for(int j =0; j < *width; j++)
        {
            rotateMatrix[j][i].value = matrix[i][j].value;
        }
    }

    return rotateMatrix;
}

void writePGM(const char* filename, PixelGray** matrix, int* width, int* height)
{
    FILE* image_out;
    image_out = fopen(filename,"wb");

    //Printing the first 3 headers
    fprintf(image_out, "P5\n");
    fprintf(image_out, "%d %d\n", *width, *height);
    fprintf(image_out, "255\n");


    //Write the file and handling error
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            if (feof(image_out)) {
                printf("End of file");
            } else if (ferror(image_out)) {
                printf("Reading Error");
            } else {
                fputc(matrix[i][j].value, image_out);
            }
        }

    }
    //Close file
    fclose(image_out);
}
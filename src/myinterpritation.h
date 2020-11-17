#ifndef MY_INTERPRITATION_H
#define MY_INTERPRITATION_H

#include <stdlib.h>
#include <stdio.h>

#define OPEN_FILE_ERROR 1
#define VERSION_ERROR 2
#define NULLS_ERROR 3
#define WIDTH_ERROR 4
#define HEIGHT_ERROR 5
#define ALLOCATION_MEMORY_ERROR 6
#define GENERAL_ERROR 7

typedef struct {
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
} ONEPIXEL;

#pragma pack(push, 1)
typedef struct {
    unsigned int bfOffBits;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned short bfType;
} HEADER;
#pragma pack(pop)

typedef struct {
    unsigned int biSize;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
} DATAHEADER;

typedef struct {
    HEADER file_header1;
    DATAHEADER info_header1;
    ONEPIXEL** data1;
} BMP_24;

typedef struct {
    HEADER file_header2;
    DATAHEADER info_header2;
    ONEPIXEL* palette;
    unsigned char** data2;
} BMP_8;

int read_header(HEADER* header, FILE* fin);
int read_dataheader(DATAHEADER* dataheader, FILE* fin);
int count_padding(int width);
int load_bmp_24(BMP_24* image, char const* filename);
int write_negative_bmp_24(BMP_24* image, char const* filename);
int load_bmp_8(BMP_8* image, char const* filename);
int write_negative_bmp_8(BMP_8* image, char const* filename);
int return_bits_per_pixel(char const* filename);
int compare_pixels(ONEPIXEL a, ONEPIXEL b);
int compare_bmp8(BMP_8* image1, BMP_8* image2);
int compare_bmp24(BMP_24* image1, BMP_24* image2);
void free_bmp8(BMP_8* image);
void free_bmp24(BMP_24 * image);
#endif //GGS4MY_INTERPRITATION_H

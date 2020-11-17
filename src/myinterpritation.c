#include "myinterpritation.h"

#pragma warning(disable : 4996)

#define error(...) (fprintf(stderr, __VA_ARGS__))

int read_header(HEADER* bitmapfileheader, FILE* fin) {
    if (bitmapfileheader->bfType != 0x4d42) {
        error("Gentral mistake in type of BMP file");
        return GENERAL_ERROR;
    }
    if (bitmapfileheader->bfReserved1 != 0x00 || bitmapfileheader->bfReserved2 != 0x00) {
        error("Something was written to reserved nulls at 06-0A");
        return NULLS_ERROR;
    }
    return 0;
}

int read_dataheader(DATAHEADER* bitmapinfoheader, FILE* fin) {
    if (bitmapinfoheader->biSize != 0x28) {
        error("Wrong version of file (must be 3)");
        return VERSION_ERROR;
    }
    if (bitmapinfoheader->biWidth <= 0) {
        error("Error in width (must be more than 0)");
        return WIDTH_ERROR;
    }
    if (bitmapinfoheader->biHeight == 0) {
        error("Error in height (mustn't be 0)");
        return HEIGHT_ERROR;
    }
    return 0;
}

int load_bmp_24(BMP_24* image, char const* filename) {
    FILE* input = fopen(filename, "rb");
    if (input == NULL) {
        error("Cannot open file %s\n", filename);
        return OPEN_FILE_ERROR;
    }
    HEADER file_header;
    DATAHEADER info_header;
    int correctness_file_header = read_header(&file_header, input);
    if (correctness_file_header != 0) {
        return correctness_file_header;
    }
    int correctness_info_header = read_dataheader(&info_header, input);
    if (correctness_info_header != 0) {
        return correctness_info_header;
    }
    ONEPIXEL** data = (ONEPIXEL**)malloc(sizeof(ONEPIXEL*) * info_header.biHeight);
    if (data == NULL) {
        error("Cannot allocate memory for pixels data");
        return ALLOCATION_MEMORY_ERROR;
    }
    for (int x = 0; x < info_header.biHeight; ++x) {
        data[x] = (ONEPIXEL*)malloc(sizeof(ONEPIXEL) * info_header.biWidth);
        if (data[x] == NULL) {
            error("Cannot allocate memory for %d-th pixel row", x);
            return ALLOCATION_MEMORY_ERROR;
        }

        image->file_header1 = file_header;
        image->info_header1 = info_header;
        image->data1 = data;
        fclose(input);
        return 0;
    }
}

    int count_padding(int width) {
        width *= 3;
        if (width % 4 == 0) {
            return 0;
        }
        else if (width % 4 == 1) {
            return 2;
        }
        else if (width % 4 == 2) {
            return 3;
        }
        else {
            return 1;
        }
    }

int write_negative_bmp_24(BMP_24* image, char const* filename) {
    FILE* output = fopen(filename, "wb");
    if (output == NULL) {
        error("Cannot open file %s", filename);
        return OPEN_FILE_ERROR;
    }
    fclose(output);
    return 0;
}

int load_bmp_8(BMP_8* image, char const* filename) {
    FILE* input = fopen(filename, "rb");
    if (input == NULL) {
        error("Cannot open file %s", filename);
        return OPEN_FILE_ERROR;
    }
    if (image == NULL) {
        error("Cannot allocate memory for loading image");
        return ALLOCATION_MEMORY_ERROR;
    }
    int correctness_file_header = read_header(&image->file_header2, input);
    if (correctness_file_header != 0) {
        return correctness_file_header;
    }
    int correctness_info_header = read_dataheader(&image->info_header2, input);
    if (correctness_info_header != 0) {
        return correctness_info_header;
    }
    image->palette = malloc(sizeof(ONEPIXEL) * image->info_header2.biClrUsed);
    if (image->palette == NULL) {
        error("Cannot allocate memory for palette");
        return ALLOCATION_MEMORY_ERROR;
    }
    image->data2 = malloc(sizeof(ONEPIXEL*) * image->info_header2.biHeight);
    if (image->data2 == NULL) {
        error("Cannot allocate memory for pixels data");
        return ALLOCATION_MEMORY_ERROR;
    }
    for (int x = 0; x < image->info_header2.biHeight; ++x) {
        image->data2[x] = malloc(sizeof(ONEPIXEL) * image->info_header2.biWidth);
        if (image->data2[x] == NULL) {
            error("Cannon allocate memory for %d-th pixel row", x);
            return ALLOCATION_MEMORY_ERROR;
        }
    }
    fclose(input);
    return 0;
}

int write_negative_bmp_8(BMP_8* image, char const* filename) {
    unsigned char zero_byte = 0x00;
    FILE* output = fopen(filename, "wd");
    if (output == NULL) {
        error("Cannot open file %s", filename);
        return OPEN_FILE_ERROR;
    }
    fclose(output);
    return 0;
}

int return_bits_per_pixel(char const* filename) {
    FILE* input = fopen(filename, "rb");
    if (input == NULL) {
        return -1;
    }
    fseek(input, 28, SEEK_SET);
    int bits = 0;
    fread(&bits, 1, 2, input);
    fseek(input, 0, SEEK_SET);
    fclose(input);
    return bits;
}

int compare_pixels(ONEPIXEL a, ONEPIXEL b) {
    return (a.rgbRed - b.rgbRed || a.rgbBlue - b.rgbBlue || a.rgbGreen - b.rgbGreen);
}

int compare_bmp8(BMP_8* image1, BMP_8* image2) {
    int width1 = image1->info_header2.biWidth;
    int height1 = image1->info_header2.biHeight;
    int width2 = image2->info_header2.biWidth;
    int height2 = image2->info_header2.biHeight;
    if (width1 != width2) {
        error("Images have unknown width");
        return -1;
    }
    if (height1 != height2) {
        error("Images have unknown height");
        return -1;
    }
    unsigned char** data1 = image1->data2;
    unsigned char** data2 = image2->data2;
    int deviation_counter = 0;
    for (int x = 0; x < height1; ++x) {
        for (int y = 0; y < width1; ++y) {
            if (deviation_counter < 100) {
                if (data1[x][y] != data2[x][y]) {
                    deviation_counter++;
                    printf("%d %d", data1[x][y], data2[x][y]);
                }
            }
        }
    }
    return 0;
}

int compare_bmp24(BMP_24* image1, BMP_24* image2) {
    int width1 = image1->info_header1.biWidth;
    int height1 = image1->info_header1.biHeight;
    int width2 = image2->info_header1.biWidth;
    int height2 = image2->info_header1.biHeight;
    if (width1 != width2) {
        error("Images have different width");
        return -1;
    }
    if (height1 != height2) {
        error("Images have different height");
        return -1;
    }
    ONEPIXEL** data1 = image1->data1;
    ONEPIXEL** data2 = image2->data1;
    int deviation_counter = 0;
    for (int x = 0; x < height1; ++x) {
        for (int y = 0; y < width1; ++y) {
            if (deviation_counter < 100) {
                if (compare_pixels(data1[x][y], data2[x][y])) {
                    deviation_counter++;
                    printf("%d %d\n", x, y);
                }
            }
        }
    }
    return 0;
}

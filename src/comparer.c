#include "myinterpritation.h"
#include <stdio.h>

#define error(...) (fprintf(stderr, __VA_ARGS__))

int main(int argc, char* argv[]) {
    if (argc > 2) return -1;

    FILE* file1, * file2;

    file1 = fopen(argv[1], "rb");
    file2 = fopen(argv[2], "rb");

    struct BMP_FILE info_header1, info_header2;
    int return_error = get_bmp_info(file1, &info_header1);
    if (return_error) {
        error("can not read the first file\n");
        return -1;
    }
    return_error = get_bmp_info(file2, &info_header2);
    if (return_error) {
        error("can not read the second file\n");
        return -1;
    }

    return_error = compare_info(&info_header1, &info_header2);
    if (return_error) return -1;

    if (!bmp_info1.number_of_colors) bmp_info1.number_of_colors = 256;
    unsigned int palette_size = bmp_info1.number_of_colors;
    return_error = compare_pixels(file1, file2, &bmp_info1, &palette_size);
    if (return_error) return -1;
    return 0;
}
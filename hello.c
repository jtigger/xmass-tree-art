#include <stdio.h>

int main() {
    printf("Merry Christmas!\n");

    printf("      *\n");
    printf("     ***\n");
    printf("    *****\n");
    printf("   *******\n");
    printf("  *********\n");
    printf(" ***********\n");
    printf("*************\n");
    printf("     | |\n");
    printf("    [___]\n");

    printf("\n-----\n\n");



    const int CANVAS_HEIGHT = 12;
    const int CANVAS_WIDTH = 14;

    char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];

    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            canvas[i][j] = '.';
        }
    }

    int height = 7;
    int width = 2 * height - 1;

    int scan_row=0;

    for (int row = 0; row < height; row++, scan_row++) {
        int tree_pixels = 2 * row + 1;
        int bg_pixels = 0;
        int scan_col = height - row - 1;
        for (int j = 0; j < tree_pixels; j++, scan_col++) {
            canvas[scan_row][scan_col] = '*';
        }
    }
    int start_of_trunk = (width - 1) / 2;
    int thickness_of_trunk = 1;
    int end_of_trunk = start_of_trunk + thickness_of_trunk;

    for (int i = 1; i < start_of_trunk; i++) {
        printf(" ");
    }
    printf("|");
    for (int i = start_of_trunk; i < end_of_trunk; i++) {
        printf(" ");
    }
    printf("|\n");
    for (int i = 1; i < start_of_trunk-1; i++) {
        printf(" ");
    }
    printf("[");
    for (int i = start_of_trunk-1; i < end_of_trunk+1; i++) {
        printf("_");
    }
    printf("]\n");

    printf("\n");

    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}

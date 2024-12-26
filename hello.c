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



    const int CANVAS_HEIGHT = 9;
    const int CANVAS_WIDTH = 13;

    char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];

    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            canvas[i][j] = ' ';
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

    int scan_col = 0;
    scan_col = start_of_trunk-1;

    canvas[scan_row][scan_col++] = '|';

    for (int i = start_of_trunk; i < end_of_trunk; i++, scan_col++) {
        canvas[scan_row][scan_col] = ' ';
    }
    canvas[scan_row][scan_col++] = '|';
    scan_row++;

    scan_col = start_of_trunk-1-1;
    canvas[scan_row][scan_col++] = '[';
    for (int i = start_of_trunk-1; i < end_of_trunk+1; i++, scan_col++) {
        canvas[scan_row][scan_col] = '_';
    }
    canvas[scan_row][scan_col++] = ']';

    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}

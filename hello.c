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


    int height = 14;
    int width = 2 * height - 1;

    for (int row = 0; row < height; row++) {
        int leaves = 2 * row + 1;
        for (int air = 0; air < height - row - 1; air++) {
            printf(" ");
        }
        for (int j = 0; j < leaves; j++) {
            printf("*");
        }
        printf("\n");
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

    
    return 0;
}

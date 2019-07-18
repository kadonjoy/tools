/*
 **************************************************************************************
 *       Filename:  unpack.c
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2016-08-25 15:17:05
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, const char *argv[]) {
    if (argc != 4) {
        printf("usage: <filename> <width> <height>\n");
        return -1;
    }

    char inputname[256];
    strncpy(inputname, argv[1], sizeof(inputname));
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

//    printf("file name: %s\n", inputname);
    FILE* fp = fopen(inputname, "rb");
    if (!fp) {
        printf("Fail to open file <%s>\n", inputname);
        return -1;
    }

    char fn[128];
    const char ch = '/';
    char * find_name = strrchr(inputname, ch);
    printf("find name: %s\n", (find_name+1));
    sprintf(fn, "output/align_%s", (char*)(find_name + 1));
    printf("find name: %s\n", fn);
    FILE* fo = fopen(fn, "wb");
    if (!fo) {
        printf("Fail to open out file <%s>\n", fn);
        fclose(fp);
        return -1;
    }
    int stride = ceil(width*1.25/8)*8;
    int widthAlign = width*1.25;
    unsigned char * mipiraw = malloc(widthAlign * height);
    unsigned char * alignraw = malloc(stride * height);
    int ret = fread(mipiraw, 1, widthAlign * height, fp);
    int i = 0;
    for(i = 0; i < height; i++) {
        memcpy(alignraw + i *stride, mipiraw + i * widthAlign, widthAlign);
    }
    fwrite(alignraw, stride * height, 1, fo);

    fclose(fo);
    fclose(fp);
    free(mipiraw);
    free(alignraw);

    return 0;
}

/********************************** END **********************************************/


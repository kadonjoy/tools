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

int unpack_mipiraw(const unsigned char * src, unsigned short * dst, int width, int height, int stride);
int convertTo8bit(const unsigned short * src, unsigned char * dst, int width, int height);
int convertToYV12(const unsigned char * src, unsigned char * dst, int width, int height);

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
    sprintf(fn, "output/unpack_%s", (char*)(find_name + 1));
    printf("find name: %s\n", fn);
    const char * yuvName = "output/2560x1920.yv12";
    FILE* fo = fopen(fn, "wb");
    if (!fo) {
        printf("Fail to open out file <%s>\n", fn);
        fclose(fp);
        return -1;
    }
    FILE* foy = fopen(yuvName, "wb");
    if (!foy) {
        printf("Fail to open out file <%s>\n", yuvName);
        fclose(fp);
        return -1;
    }
    int stride = ceil(width*1.25/8)*8;
    unsigned char * mipiraw = malloc(stride * height);
    unsigned short * unpackraw = malloc(width * 2 * height);
    unsigned char * byteRaw = malloc(ceil(width/8)*8 * height);
    unsigned char * YV12Buf = malloc((width * height * 3) / 2);
    int ret = fread(mipiraw, 1, stride * height, fp);
    unpack_mipiraw(mipiraw, unpackraw, width, height, stride);
    convertTo8bit(unpackraw, byteRaw, width, height);
    convertToYV12(byteRaw, YV12Buf, width, height);
//    fwrite(unpackraw, width * 2 * height, 1, fo);
    fwrite(byteRaw, ceil(width/8)*8 * height, 1, fo);
    fwrite(YV12Buf, (width * height * 3) / 2, 1, foy);

    fclose(fo);
    fclose(fp);
    fclose(foy);
    free(mipiraw);
    free(unpackraw);
    free(byteRaw);
    free(YV12Buf);

    return 0;
}

int unpack_mipiraw(const unsigned char * src, unsigned short *dst, int width, int height, int stride) {
    unsigned char buf[1024*8];
    int i = 0;
    int j = 0;
    int n = 0;
    int k = 0;
    for (n = 0; n < height; n++) {
        for (j = 0; j < width/4; j++) {
            const unsigned char * p = src + stride * n + j*5;
//            printf("n = %d, j = %d, k = %d\n", n, j, k);
            for (i = 0, k = width * n + j*4; i < 4; i++) {
                unsigned short d = p[i];
                d = d << 2;
                d = d | ((p[4]>>(i*2))&0x3);
//                printf("k = %d, d: 0x%x\n", k, d);
                dst[k++] = d;
                //printf("k = %d, d: 0x%x\n", k, d);
            }
        }
    }
}

int convertTo8bit(const unsigned short * src, unsigned char * dst, int width, int height) {
    int i = 0;
    int j = 0;
    int k = 0;
    for(i = 0; i < height; i++) {
        for(j = 0; j < width; j++ ) {
            k = src[width*i + j]*0xff;
            dst[width*i + j] = k / 0x3ff; 
        }
    }
}

int convertToYV12(const unsigned char * src, unsigned char * dst, int width, int height) {
    int Y_Size =  width * height;
    int i = 0;
    for (; i < Y_Size; i++) {
        dst[i] = src[i];
    }
    memset(dst+Y_Size, 128, Y_Size/2);
}
/********************************** END **********************************************/


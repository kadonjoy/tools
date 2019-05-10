/*
* use libjpeg lib to convert YUV to jpeg file
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <fcntl.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <string.h>

#define LOG(format, ...)     fprintf(stdout, format, ##__VA_ARGS__)

/* * Convert NV21 (YYYYYYYYY:VUVU) to YV12 (YYYYYYYY:UU:VV) * */
void convertNV21ToYV12(unsigned char* nv21Buf, unsigned char* yv12Buf, int width, int height)
{
    int imgSize = width * height;
    int quartSize = imgSize / 4;
    int vPos = imgSize + quartSize;
    int i = 0, j = 0;
    memcpy(yv12Buf, nv21Buf, imgSize);
    for (; i < imgSize/4; i++)
    {
        yv12Buf[imgSize + i] = nv21Buf[imgSize + j + 1];
        yv12Buf[vPos + i] = nv21Buf[imgSize + j];
        j += 2;
    }
    LOG("exit: %s\n", __func__);

    return ;
}

/* * Convert NV12 (YYYYYYYYY:UVUV) to YV12 (YYYYYYYY:UU:VV) * */
void convertNV12ToYV12(unsigned char* nv12Buf, unsigned char* yv12Buf, int width, int height)
{
    int imgSize = width * height;
    int quartSize = imgSize / 4;
    int vPos = imgSize + quartSize;
    int i = 0, j = 0;
    memcpy(yv12Buf, nv12Buf, imgSize);
    for (; i < imgSize/4; i++)
    {
        yv12Buf[imgSize + i] = nv12Buf[imgSize + j];
        yv12Buf[vPos + i] = nv12Buf[imgSize + j +1];
        j += 2;
    }
    LOG("exit: %s\n", __func__);

    return ;
}

// for mtk yv12 format
int jpeg_enc_I420(unsigned char* buffer, int width, int height, int quality, char* filename)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile = NULL;
    int ret = TRUE;
    if(buffer == NULL || width <=0 || height <=0|| filename == NULL)
        return FALSE;
    if ((outfile = fopen(filename, "wb")) == NULL) 
    {  
        return FALSE;
    }    
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width; 
    cinfo.image_height = height;
    cinfo.input_components = 3;        
    cinfo.in_color_space = JCS_YCbCr;
    jpeg_set_defaults(&cinfo);

#if JPEG_LIB_VERSION >= 70

    cinfo.do_fancy_downsampling = FALSE;
    cinfo.dct_method = JDCT_FASTEST;
    cinfo.smoothing_factor = 0;

#endif
    jpeg_set_quality(&cinfo, quality, TRUE);
    cinfo.raw_data_in = TRUE;  
    
    {
        JSAMPARRAY pp[3];
        JSAMPROW rpY[16], rpU[16], rpV[16];
        pp[0] = rpY;
        /*pp[1] = rpU;*/
        /*pp[2] = rpV;*/
        pp[1] = rpV;
        pp[2] = rpU;
        int k;
        if(rpY == NULL && rpU == NULL && rpV == NULL)
        {
            ret = FALSE;
            goto exit;
        }
        cinfo.raw_data_in = TRUE;                  // supply downsampled data
        cinfo.comp_info[0].h_samp_factor = 2;
        cinfo.comp_info[0].v_samp_factor = 2;
        cinfo.comp_info[1].h_samp_factor = 1;
        cinfo.comp_info[1].v_samp_factor = 1;
        cinfo.comp_info[2].h_samp_factor = 1;
        cinfo.comp_info[2].v_samp_factor = 1;
        cinfo.dct_method = JDCT_FASTEST;
        jpeg_start_compress(&cinfo, TRUE);

        int i,j;
        for (j = 0; j < cinfo.image_height; j += 16) {
            for (i = 0; i < 16; i++) {
                 rpY[i] = buffer + width * (i + j);
                 if (i%2 == 0) {
                     rpU[i/2] = buffer + width * height + width / 2 * ((i + j) / 2);
                     rpV[i/2] = buffer + width * height + width * height / 4 + width / 2 * ((i + j) / 2);
                 }
            }
            jpeg_write_raw_data(&cinfo, pp, 16);
        }

        jpeg_finish_compress(&cinfo);
    }
//    LOG("%s : exit\n", __func__);
exit:
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
    return ret;
}


int jpeg_enc_yv12(unsigned char* buffer, int width, int height, int quality, char* filename)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile = NULL;
    int ret = TRUE;
    if(buffer == NULL || width <=0 || height <=0|| filename == NULL)
        return FALSE;
    if ((outfile = fopen(filename, "wb")) == NULL) 
    {  
        return FALSE;
    }    
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width; 
    cinfo.image_height = height;
    cinfo.input_components = 3;        
    cinfo.in_color_space = JCS_YCbCr;
    jpeg_set_defaults(&cinfo);

#if JPEG_LIB_VERSION >= 70

    cinfo.do_fancy_downsampling = FALSE;
    cinfo.dct_method = JDCT_FASTEST;
    cinfo.smoothing_factor = 0;

#endif
    jpeg_set_quality(&cinfo, quality, TRUE);
    cinfo.raw_data_in = TRUE;  
    
    {
        JSAMPARRAY pp[3];
        JSAMPROW rpY[16], rpU[16], rpV[16];
        pp[0] = rpY;
        pp[1] = rpU;
        pp[2] = rpV;
        int k;
        if(rpY == NULL && rpU == NULL && rpV == NULL)
        {
            ret = FALSE;
            goto exit;
        }
        cinfo.raw_data_in = TRUE;                  // supply downsampled data
        cinfo.comp_info[0].h_samp_factor = 2;
        cinfo.comp_info[0].v_samp_factor = 2;
        cinfo.comp_info[1].h_samp_factor = 1;
        cinfo.comp_info[1].v_samp_factor = 1;
        cinfo.comp_info[2].h_samp_factor = 1;
        cinfo.comp_info[2].v_samp_factor = 1;
        cinfo.dct_method = JDCT_FASTEST;
        jpeg_start_compress(&cinfo, TRUE);

        int i,j;
        for (j = 0; j < cinfo.image_height; j += 16) {
            for (i = 0; i < 16; i++) {
                 rpY[i] = buffer + width * (i + j);
                 if (i%2 == 0) {
                     rpU[i/2] = buffer + width * height + width / 2 * ((i + j) / 2);
                     rpV[i/2] = buffer + width * height + width * height / 4 + width / 2 * ((i + j) / 2);
                 }
            }
            jpeg_write_raw_data(&cinfo, pp, 16);
        }

        jpeg_finish_compress(&cinfo);
    }
//    LOG("%s : exit\n", __func__);
exit:
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
    return ret;
}


void
usage (char *pname) {
	fprintf (stderr,
		"Usage: %s [-v]  [-i #] [-w #] [-h #] \n",
		pname);
	exit (1);
}


void get_extension(const char *file_name,char *extension)
{
    int i=0,length;
    length=strlen(file_name);
        while(file_name[i])
    {
        if(file_name[i]=='.')
        break;
        i++;
    }
    if(i<length)
    strcpy(extension,file_name+i+1);
    else
    strcpy(extension,"\0");
}

int main(int argc, char ** argv)
{
	int c, fd,  input = -1;
	char input_file[256] = {0};
	char outfile_name[256] = {0};
	char fn_outfile_name[256] = {0};
    char extension_name[32] = {0};
	int width=320, height =240;
	unsigned int size;
	int qualite = 100;
	unsigned char *buffer;
	unsigned char *yuv_buffer;
	int verbose=0;
	
	/* parse arguments
	 */
/* 	while ((c = getopt (argc, argv, "vi:w:h:")) != EOF) {
 * 		switch (c) {
 * 			case 'i':
 * 				input_file = optarg;
 * 				break;
 * 			case 'w':
 * 				width = strtoul(optarg, NULL, 10);
 * 				break;
 * 			case 'h':
 * 				height = strtoul(optarg, NULL, 10);
 * 				break;
 * 			case 'v':
 * 				verbose++;
 * 				break;
 * 			default:
 * 				usage (argv[0]);
 * 				break;
 * 		}
 * 	}
 */
    strncpy(input_file, argv[1], sizeof(input_file));
//    LOG("argv[1]: %s\n", argv[1]);
//    LOG("argv[2]: %s\n", argv[2]);
//    LOG("argv[3]: %s\n", argv[3]);
    width = atoi(argv[2]);
    height = atoi(argv[3]);
	size = width* height *3/2;

    get_extension(input_file, extension_name);
//    LOG("file extension name: %s\n", extension_name);
	
   	buffer = malloc(width*height*3/2);
	if (!buffer ){
		LOG("can't alloc buffer \n");
		exit(-1);
	}
   	yuv_buffer = malloc(width*height*3/2);
	 if (!yuv_buffer ){
		LOG("can't alloc yuv_buffer \n");
		exit(-1);
	}
    fd = open(input_file, O_RDWR);
	if (fd < 0 ){
		LOG("can't open file to read/write\n");
		exit(-1);
	}
	
	if (read(fd, buffer, size) != size){
		LOG("read file error\n");
		exit(-1);
	}
    const char ch = '/';
    char * find_name = strrchr(input_file, ch);
//    printf("find name: %s\n", (find_name+1));	
    strcat(outfile_name, strtok((find_name+1), "."));
	strcat(outfile_name, ".jpg");
    sprintf(fn_outfile_name, "output/%s", outfile_name);
	LOG("outfile name : %s\n", fn_outfile_name);
    
    if (!strcmp(extension_name, "yuv")) {
        jpeg_enc_I420(buffer, width, height, qualite, fn_outfile_name);
    } else if(!strcmp(extension_name, "nv21")) {
        convertNV21ToYV12(buffer, yuv_buffer, width, height);
        jpeg_enc_yv12(yuv_buffer, width, height, qualite, fn_outfile_name);
    } else if(!strcmp(extension_name, "nv12")) {
        convertNV12ToYV12(buffer, yuv_buffer, width, height);
        jpeg_enc_yv12(yuv_buffer, width, height, qualite, fn_outfile_name);
    }

    free(buffer);
	free(yuv_buffer);
    close(fd);
	return 0;
}

all: yuv2jpeg raw_unpack

yuv2jpeg: src/yuv2jpeg/yuv2jpeg.c
	gcc $^ -ljpeg -Ijpeg -o tools/$@ 

raw_unpack: src/raw_unpack/raw_unpack.c
	gcc $^ -lm -o tools/$@

clean:
	$(shell rm output/*)
	$(shell rm tools/*)


说明:
1. unpack 10bit mipi raw to 2 bytes
2. 转换yuv(YV12,I420,NV21,N12)数据为jpeg文件
3. 需要转换的图片请放到images目录下，转换后产生的结果输出到output目录

环境准备:
sudo apt-get install libjpeg-dev
sudo apt-get install libjpeg62:i386
sudo apt-get install libjpeg62

工具使用:
1. 编译bin文件
在本目录下执行make命令

2. 如何转换图片?
./imgConvertTool.sh -unpack images/1.raw 1920 1080
./imgConvertTool.sh -Y2J images/2.yuv 1920 1080
./imgConvertTool.sh -Y2J images/3.nv21 1920 1080

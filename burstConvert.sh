#!/bin/bash

function usage()
{
cat << EOF
USAGE:
        $0 <option> <directory> <width> <height>
OPTION:
        -raw
        -yuv
EXAMPLE:
       $0 -raw images/raw 1920 1080
       $0 -yuv images/yuv 1920 1080
EOF
exit 0
}

function burst_unpack_mipiraw()
{
    for file in $(find $1 -regex ".*\.raw")
    do
        ./imgConvertTool.sh -unpack $file $2 $3
    done
}

function burst_yuv2jpeg()
{
    echo "burst_yuv2jpeg"
    for file in $(find $1 -regex ".*\.yuv\|.*\.nv[12][12]\|.*\.NV[12][12]")
    do
        ./imgConvertTool.sh -Y2J $file $2 $3
    done
}
function main()
{
    if [ -z $1 ]; then
        usage
        exit -1
    fi  

    while [ ! -z $1 ]
    do
        case $1 in
            -raw)
                shift
                burst_unpack_mipiraw $@
            ;;

            -yuv)
                shift
                burst_yuv2jpeg $@
            ;;
            *)
                usage
                echo
            ;;
        esac
        shift
    done
    exit 0
}

main $@

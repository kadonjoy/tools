#!/bin/bash

TOOL_PATH="$(pwd)/tools"

function usage()
{
cat << EOF
USAGE:
        $0 <option> <filename> <width> <height>
OPTION:
        -unpack
        -Y2J
EXAMPLE:
       $0 -unpack images/1.raw 1920 1080
       $0 -Y2J images/2.yuv 1920 1080
EOF
exit 0
}

function unpack_mipiraw()
{
    echo $@
    #var=$@
    #w_h=${var%.*}
    #width=${w_h%x*}
    #height=${w_h#*x}
    #echo $w_h
    #echo $width
    #echo $height
    $TOOL_PATH/raw_unpack $@
    #$TOOL_PATH/raw_unpack images/$var $width $height
    exit 0
}

function yuv2jpeg()
{
    echo $@
    $TOOL_PATH/yuv2jpeg $@
    exit 0
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
            -unpack)
                shift
                unpack_mipiraw $@
            ;;

            -Y2J)
                shift
                yuv2jpeg $@
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

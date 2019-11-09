#!/bin/bash

INPUT=$1

while read -r url; do
    output=`curl -s -I -X GET $url | grep -i content-length | tr '\r\n' ' ' | tr '\n' ' '`
    if [[ -n $output ]]; then
        SIZE=`echo $output | awk '{print $2}'`
        [ "$SIZE" != "0" ] && echo $url
    fi
done < $INPUT


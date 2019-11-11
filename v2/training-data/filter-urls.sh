#!/bin/bash

INPUT=$1

while read -r url; do
    # echo "$url"
    output=$(timeout 3 curl -s -I -X GET "$url" | grep -i -E 'content-length|transfer-encoding' | tr '\r\n' ' ' | tr '\n' ' ')
    # echo "$output"
    header=$(echo "$output" | awk '{print $1}')
    unset content_length
    unset transfer_encoding
    if [ "${header,,}" = "content-length:" ]; then
        content_length=$(echo "$output" | awk '{print $2}')
    elif [ "${header,,}" = "transfer-encoding:" ]; then
        transfer_encoding=$(echo "$output" | awk '{print $2}')
    else
        continue
    fi
    [ "$content_length" != "0" ] && echo "$url"
    [ "${transfer_encoding,,}" = "chunked" ] && echo "$url"
done < "$INPUT"


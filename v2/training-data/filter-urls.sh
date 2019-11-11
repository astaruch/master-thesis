#!/bin/bash

INPUT=$1

function filter_url() {
    URL=$1
    HEADER=$(timeout 3 curl -s -I -X GET "$URL" | grep -i -E 'content-length|transfer-encoding' | tr '\r\n' ' ' | tr '\n' ' ')
    HEADER_NAME=$(echo "$HEADER" | awk '{print $1}')
    CONTENT_LENGTH=0
    TRANSFER_ENCODING=""
    if [ "${HEADER_NAME,,}" = "content-length:" ]; then
        CONTENT_LENGTH=$(echo "$HEADER" | awk '{print $2}')
    elif [ "${HEADER_NAME,,}" = "transfer-encoding:" ]; then
        TRANSFER_ENCODING=$(echo "$HEADER" | awk '{print $2}')
    else
        return
    fi
    [ "$CONTENT_LENGTH" != "0" ] && echo "$URL"
    [ "${TRANSFER_ENCODING,,}" = "chunked" ] && echo "$URL"
}

JOBS=0

while read -r URL; do
    filter_url "$URL" &
    JOBS=$((JOBS + 1))
    if [ $JOBS = 64 ]; then
        wait;
        JOBS=0
    fi
done < "$INPUT"


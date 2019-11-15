#!/bin/bash

function print_header() {
    cat <<EOF
#pragma once

#ifndef PHISHSVC_ASN_ROGUE_INDEX_H
#define PHISHSVC_ASN_ROGUE_INDEX_H

#include <string_view>
#include <unordered_map>

static const std::unordered_map<std::string_view, double> asn_rogue_index{
EOF
}

function print_mappings() {
    while read -r LINE; do
        [[ $LINE != AS* ]] && continue
        echo "$LINE" | sed 's/^/    { "/g' | sed 's/,/", /g' | sed 's/$/ },/g'
    done < "${1:-/dev/stdin}"
}

function print_end() {
    cat <<EOF
};
#endif // PHISHSVC_ASN_ROGUE_INDEX_H
EOF
}

print_header
print_mappings $1
print_end

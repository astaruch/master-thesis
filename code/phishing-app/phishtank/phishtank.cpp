#include "phishtank.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include "../third-party/csv.h"

bool phishtank::parse_database(const std::string& filepath) {

    io::CSVReader<8, io::trim_chars<' '>, io::double_quote_escape<',','\"'>> in(filepath);
    in.read_header(io::ignore_extra_column, "phish_id", "url", "phish_detail_url",
        "submission_time", "verified", "verification_time", "online", "target");

    record rec;
    while(in.read_row(rec.phish_id, rec.url, rec.phish_detail_url, rec.submission_time,
        rec.str_verified, rec.verification_time, rec.str_online, rec.target))
    {
        rec.verified = rec.str_verified == "yes";
        rec.online = rec.str_online == "yes";
        m_records.push_back(rec);
    }
    return true;
}

bool phishtank::check_url(const std::string& url) {
    const auto& it = std::find_if(m_records.begin(), m_records.end(), [&url](const record& rec) {
        return rec.url == url;
    });
    return it != m_records.end();
}


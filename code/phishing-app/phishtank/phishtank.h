#ifndef PHISHTANK_CPP_LIBRARY_H
#define PHISHTANK_CPP_LIBRARY_H

#include <string>
#include <vector>

class phishtank {
public:
    /**
phish_id,url,phish_detail_url,submission_time,verified,verification_time,online,target
5870400,https://www.fxdomains.com/,http://www.phishtank.com/phish_detail.php?phish_id=5870400,2018-12-09T04:37:49+00:00,yes,2018-12-09T04:45:59+00:00,yes,Other
5870399,http://www.fxdomains.com/,http://www.phishtank.com/phish_detail.php?phish_id=5870399,2018-12-09T04:37:48+00:00,yes,2018-12-09T04:45:59+00:00,yes,Other
     */
    struct record {
        unsigned phish_id;
        std::string url;
        std::string phish_detail_url;
        std::string submission_time;
        std::string str_verified;
        bool verified;
        std::string verification_time;
        std::string str_online;
        bool online;
        std::string target;
    };

    bool parse_database(const std::string& filepath);

    bool check_url(const std::string& url);

private:

    std::vector<record> m_records; // TODO: optimalization   - use set

};


#endif
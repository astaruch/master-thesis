#include <iostream>
#include <fstream>

#include "phishtank/phishtank.h"
#include "safebrowsing.h"
#include "url_anomalies.h"


int main(int argc, char* argv[]) {
    std::cout << "PHISHING DETECTION APPLICATION\n";
    const auto& google_api_key = argv[1];
    const char* input_file_urls;
    if (argc >= 3) {
        input_file_urls = argv[2];
    } else {
        std::cerr << "Enter input file\n";
        return 1;
    }
    const char* phishtank_database;
    if (argc >= 4) {
        phishtank_database = argv[3];
    } else {
        std::cerr << "Enter phishtank db\n";
        return 1;
    }
    phishtank phishtank_api;
    phishtank_api.parse_database(phishtank_database);

    url_anomalies anomalies;

    safebrowsing google_api(google_api_key);
    google_api.add_threat_type(safebrowsing::threat_type::unwanted_software);
    google_api.add_threat_type(safebrowsing::threat_type::malware);
    google_api.add_threat_type(safebrowsing::threat_type::social_engineering);
    google_api.add_platform_type(safebrowsing::platform_type::windows);
    google_api.add_threat_entry_type(safebrowsing::threat_entry_type::url);


    std::string url;
    json response;
    std::ifstream file(input_file_urls);
    while (std::getline(file, url)) {
        std::cout << url << '\n';
        std::cout << "PHISHTANK: " << (phishtank_api.check_url(url) ? "THREAT.\n" : "OK.\n");
        anomalies.parse_url(url);
        std::cout << "ANOMALIES: " << (anomalies.perform_tests(85, 5) == 0 ? "NONE" : "") << "\n";
        google_api.add_url(url);
        if (google_api.number_of_urls() == 500) {
            response = google_api.threat_matches();
            std::cout << "GOOGLE:\n";
            google_api.evaluate_response(response);
            google_api.clear_urls();
        }

    }
    // TODO: handle max number of url in google API - it is 500
    response = google_api.threat_matches();
    google_api.evaluate_response(response);


    return 0;
}
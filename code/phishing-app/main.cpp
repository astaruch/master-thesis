#include <iostream>

#include "phishtank/phishtank.h"
#include "safebrowsing.h"
#include "url_anomalies.h"

int main(int argc, char* argv[]) {
    std::cout << "PHISHING DETECTION APPLICATION\n";
    const auto& google_api_key = argv[1];
    const char* input_file;
    if (argc == 3) {
        input_file = argv[2];
    }
    phishtank phishtank_api;
    phishtank_api.parse_database("../example-database.csv");

    url_anomalies anomalies;

    safebrowsing google_api(google_api_key);
    google_api.add_threat_type(safebrowsing::threat_type::unwanted_software);
    google_api.add_threat_type(safebrowsing::threat_type::malware);
    google_api.add_threat_type(safebrowsing::threat_type::social_engineering);
    google_api.add_platform_type(safebrowsing::platform_type::windows);
    google_api.add_threat_entry_type(safebrowsing::threat_entry_type::url);
    for (auto url: {"google.com", "https://www.fxdomains.com/"}) {
        std::cout << "URL " << url << " is a " << (phishtank_api.check_url(url) ? "THREAT.\n" : "OK.\n");
        anomalies.parse_url(url);
        std::cout << "Anomalies: " << anomalies.perform_tests(85, 5) << "\n";
        google_api.add_url(url);
    }
    json response = google_api.threat_matches();
    google_api.evaluate_response(response);


    return 0;
}
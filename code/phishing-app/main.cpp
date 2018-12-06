#include <iostream>
#include <iomanip>
#include <fstream>
#include <curl/curl.h>

#include "json.hpp"

using json = nlohmann::json;

static std::string readBuffer;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

int main(int argc, char *argv[]) {

    std::string url = argv[1];

    json j = {
        {"client",
            {
                {"clientId",    "phishingapp"},
                {"clientVersion", "0.1"}
            }
        },
        {"threatInfo",
            {
                {"threatTypes", {"MALWARE", "SOCIAL_ENGINEERING"}},
                {"platformTypes", {"WINDOWS"}},
                {"threatEntryTypes", {"URL"}},
                {"threatEntries", {{"url", url}}}
            }
        }
    };
    std::string google_url = "https://safebrowsing.googleapis.com/v4/threatMatches:find?key=AIzaSyCU5orm0vjOCEsA4qYzmgugY1ruSa44V2k";
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
//    std::string json_str = j.get<std::string>();
    std::string readBuffer;

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

        curl_easy_setopt(curl, CURLOPT_URL, google_url.c_str());
        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        std::string dump = j.dump();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dump.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);



        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
//    std::cout << readBuffer << std::endl;
    json response = json::parse(readBuffer);
//    std::cout << "response: " << response << '\n';
    if (response.empty()) {
        std::cout << "This URL is safe to open." << '\n';
    } else {
        std::cout << "This URL is unsafe to open because it was found in Google's " <<
                  response["matches"][0]["threatType"] << " category of harmful URLs." << std::endl;
    }


    return 0;


}
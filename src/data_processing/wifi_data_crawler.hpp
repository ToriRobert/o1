#ifndef WIFI_DATA_CRAWLER_HPP
#define WIFI_DATA_CRAWLER_HPP

#include <string>
#include <curl/curl.h>

class InfluxDBQueryClient {
public:
    InfluxDBQueryClient(const std::string& url, const std::string& token);
    
    bool sendQuery(const std::string& query, std::string& response);

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    bool setCurlOptions(CURL* curl);
    bool prepareHttpHeader(struct curl_slist*& header, CURL* curl);

    std::string mUrl;
    std::string mToken;
};

#endif
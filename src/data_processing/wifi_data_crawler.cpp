/* This file contains a class to query InfluxDB using HTTP requests */

#include <iostream>
#include <cassert>
#include <iomanip>
#include "wifi_data_crawler.hpp"
#include "response_formatter.hpp"
#include "wifi_ves_event_send.hpp"


InfluxDBQueryClient::InfluxDBQueryClient(const std::string& url, const std::string& token)
    : mUrl(url), mToken(token) {
}


/*******************************************************************
 *
 * @brief Set options for the curl object
 *
 * @details
 *
 *    Function : setCurlOptions
 *
 *    Functionality:
 *      - Sets curl options in its object
 *
 * @params[in] pointer to curl
 * @return true  : success
 *         false : failure
 ******************************************************************/
bool InfluxDBQueryClient::setCurlOptions(CURL* curl) {
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_FRESH_CONNECT, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

    // Disable SSL verifications (Note: This is not recommended for production use)
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    return true;
}

/*******************************************************************
 *
 * @brief Prepare HTTP Header components
 *
 * @details
 *
 *    Function : prepareHttpHeader
 *
 *    Functionality:
 *      - Prepares HTTP Header components and sets them in the
 *        header object
 *
 * @params[in] pointer to curl and header
 * @return true  : success
 *         false : failure
 ******************************************************************/
bool InfluxDBQueryClient::prepareHttpHeader(struct curl_slist*& header, CURL* curl) {
    setCurlOptions(curl);

    header = curl_slist_append(header, ("Authorization: Token " + mToken).c_str());
    header = curl_slist_append(header, "Accept: application/csv");
    header = curl_slist_append(header, "Content-type: application/vnd.flux");

    if (!header) {
        std::cerr << "Failed to append to header" << std::endl;
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
    curl_easy_setopt(curl, CURLOPT_URL, mUrl.c_str());

    return true;
}

/*******************************************************************
 *
 * @brief Callback function for curl to write received data
 *
 * @details
 *
 *    Function : WriteCallback
 *
 *    Functionality:
 *      - Appends received data to the response string
 *
 * @params[in] received data, size, nmemb, userp
 * @return total size written
 ******************************************************************/
size_t InfluxDBQueryClient::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/*******************************************************************
 *
 * @brief Sends a query to InfluxDB
 *
 * @details
 *
 *    Function : sendQuery
 *
 *    Functionality:
 *      - Prepares and sends an HTTP POST request to InfluxDB
 *      - Receives and stores the response
 *
 * @params[in] query string, reference to response string
 * @return true  : success
 *         false : failure
 ******************************************************************/
bool InfluxDBQueryClient::sendQuery(const std::string& query, std::string& response) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return false;
    }

    struct curl_slist* header = nullptr;
    if (!prepareHttpHeader(header, curl)) {
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(header);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code != 200) {
        std::cerr << "HTTP request failed with code " << http_code << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_cleanup(curl);
    return true;
}

// Example usage
int main() {
    InfluxDBQueryClient client("http://140.118.122.119:8086/api/v2/query?org=tori", 
                               "Jl_Evo6QakYbmObqca5epKSddaU7UxjLen_rGvMqSeqZJWXoG6rzqKD1SXL7ryadhFWF7iIp_ynaML72MY4tyg==");

    std::string query = "from(bucket: \"wifi\")"
                        "|> range(start: -10m)"
                        "|> filter(fn: (r) => r[\"_measurement\"] == \"AP\")"
                        "|> filter(fn: (r) => r[\"ap_group_building\"] == \"EE\")"
                        "|> filter(fn: (r) => r[\"_field\"] == \"sta_count\")";

    std::string response;
    if (client.sendQuery(query, response)) {
        if (!response.empty()) {
            try {
                ResponseFormatter formatter;
                std::string formattedResponse = formatter.formatResponse(response);
                if (!formattedResponse.empty()) {
                    
                    std::cout << std::setw(4) << formattedResponse << std::endl;
                    
                    WifiVesEventSend sender("https://192.168.8.6:30780/dcae-ves-collector/eventListener/v7",
                                            "sample1", "sample1");

                    if (sender.sendWifiData(formattedResponse)) {
                        std::cout << std::endl << "WiFi 數據發送成功" << std::endl;
                    } else {
                        std::cout << "WiFi 數據發送失敗" << std::endl;
                    }                        
                        
                } else {
                    std::cerr << "格式化回應失敗。" << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "處理回應時發生錯誤：" << e.what() << std::endl;
            }
        } else {
            std::cerr << "回應為空。" << std::endl;
        }
    } else {
        std::cerr << "查詢失敗。" << std::endl;
    }

    return 0;
}
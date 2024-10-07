#include "wifi_ves_event_send.hpp"
#include <curl/curl.h>
#include <iostream>

WifiVesEventSend::WifiVesEventSend(const std::string& url, const std::string& username, const std::string& password)
    : m_url(url), m_username(username), m_password(password) {}

bool WifiVesEventSend::sendWifiData(const std::string& formattedResponse) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "無法初始化 curl" << std::endl;
        return false;
    }

    // 設置 curl 選項
    curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, formattedResponse.c_str());
    curl_easy_setopt(curl, CURLOPT_USERPWD, (m_username + ":" + m_password).c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // 執行請求
    CURLcode res = curl_easy_perform(curl);

    // 清理
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() 失敗: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    return true;
}
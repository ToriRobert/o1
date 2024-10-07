#pragma once

#include <string>

class WifiVesEventSend {
public:
    WifiVesEventSend(const std::string& url, const std::string& username, const std::string& password);
    bool sendWifiData(const std::string& formattedResponse);

private:
    std::string m_url;
    std::string m_username;
    std::string m_password;
};
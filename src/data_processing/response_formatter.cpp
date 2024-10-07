#include "response_formatter.hpp"
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>

std::string ResponseFormatter::formatResponse(const std::string& influxResponse) {
    try {
        std::vector<std::map<std::string, std::string>> parsedData = parseCSV(influxResponse);
        return createJSON(parsedData);
    } catch (const std::exception& e) {
        std::cerr << "格式化回應時發生錯誤：" << e.what() << std::endl;
        return ""; // 或者返回一個錯誤 JSON
    }
}

std::vector<std::map<std::string, std::string>> ResponseFormatter::parseCSV(const std::string& csv) {
    std::vector<std::map<std::string, std::string>> result;
    std::istringstream iss(csv);
    std::string line;
    std::vector<std::string> headers;

    // 讀取標頭
    if (std::getline(iss, line)) {
        headers = split(line, ',');
    } else {
        throw std::runtime_error("CSV 數據為空或格式不正確");
    }

    // 讀取資料
    while (std::getline(iss, line)) {
        auto values = split(line, ',');
        std::map<std::string, std::string> row;
        for (size_t i = 0; i < headers.size() && i < values.size(); ++i) {
            row[headers[i]] = values[i];
        }
        if (!row.empty()) {
            result.push_back(row);
        }
    }

    if (result.empty()) {
        throw std::runtime_error("沒有有效的數據行");
    }

    return result;
}

std::string ResponseFormatter::createJSON(const std::vector<std::map<std::string, std::string>>& data) {
    if (data.empty()) {
        throw std::runtime_error("沒有數據可以轉換為 JSON");
    }

    std::string result = "{";

    // 設定通用事件標頭
    result += "\"event\": {\"commonEventHeader\": {";
    result += "\"version\": \"4.1\",";
    result += "\"vesEventListenerVersion\": \"7.1.1\",";
    result += "\"domain\": \"measurement\",";
    result += "\"eventName\": \"Measurement_Vscf:WiFi_AccessPointConnections\",";
    result += "\"eventId\": \"measurement" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + "\",";
    result += "\"sequence\": 1,";
    result += "\"priority\": \"Normal\",";
    result += "\"reportingEntityId\": \"wifi001\",";
    result += "\"reportingEntityName\": \"NTUST-EE\",";
    result += "\"sourceId\": \"wifi002\",";
    result += "\"sourceName\": \"WIFI-AP\",";
    result += "\"nfVendorName\": \"BMWLAB\",";
    result += "\"nfNamingCode\": \"wifi\",";
    result += "\"nfcNamingCode\": \"NFC\",";
    result += "\"startEpochMicrosec\": " + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ",";
    result += "\"lastEpochMicrosec\": " + std::to_string(std::chrono::system_clock::now().time_since_epoch().count() + 6000000) + ",";
    result += "\"timeZoneOffset\": \"+00:00\"";
    result += "},";

    // 設定測量欄位
    result += "\"measurementFields\": {\"measurementInterval\": 60,"; // 更新測量間隔
    result += "\"measurementFieldsVersion\": \"4.0\",";
    result += "\"additionalObjects\": [";
    result += "{ \"objectName\": \"WiFi Access Points\", \"objectInstances\": [";

    for (size_t i = 0; i < data.size() -1; ++i) {
        const auto& row = data[i];
        result += "{ \"objectInstance\": {";
        try {
            result += "\"ap_building\": \"" + row.at("ap_group_building") + "\",";
            result += "\"ap_floor\": \"" + row.at("ap_group_floor") + "\",";
            result += "\"ap_name\": \"" + row.at("ap_name") + "\",";
            result += "\"sta_count\": " + std::to_string(std::stoi(row.at("_value"))) + ","; // 更新為 sta_count
            result += "\"unit\": \"count\"";
        } catch (const std::exception& e) {
            std::cerr << "創建 JSON 時發生錯誤：" << e.what() << std::endl;
            continue; // 跳過這一行數據
        }
        result += "}}";
        if (i < data.size() - 2) {
            result += ",";
        }
    }

    result += "]}]}}";
    result += "}";

    return result;
}

std::vector<std::string> ResponseFormatter::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
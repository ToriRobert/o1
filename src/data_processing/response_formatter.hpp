#ifndef RESPONSE_FORMATTER_HPP
#define RESPONSE_FORMATTER_HPP

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ResponseFormatter {
public:
    std::string formatResponse(const std::string& influxResponse);

private:
    std::vector<std::map<std::string, std::string>> parseCSV(const std::string& csv);
    std::string createJSON(const std::vector<std::map<std::string, std::string>>& data);
    std::vector<std::string> split(const std::string& s, char delimiter);
};

#endif 
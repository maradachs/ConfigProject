#pragma once

#include <string>
#include <vector> 

#include <rapidjson/document.h>

class CConfig
{
private:
    rapidjson::Document doc;
    rapidjson::Value* FindValueByKey(const std::string& key);
    std::vector<std::string> ExtractElements(const std::string& key);

public:
    bool Load(const std::string& filename);
    std::string GetOption(const std::string& key, const std::string& defaultValue = "", bool isBase64Encoded = false);
    rapidjson::Value* GetSection(const std::string& key);
    bool SetOption(const std::string& key, int value);
    bool SetOption(const std::string& key, double value);
    bool SetOption(const std::string& key, bool value);
    bool SetOption(const std::string& key, const std::string& value, bool encodeBase64 = false);
};


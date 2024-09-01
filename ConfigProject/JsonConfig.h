#pragma once

#include <string>
#include <vector> 

#include "IConfigStorage.h"

#include <rapidjson/document.h>

using namespace rapidjson;

class JsonConfig : public IConfigStorage
{
private:
    Document doc;
    Value* FindValueByKey(const std::string& key);
    std::vector<std::string> ExtractElements(const std::string& key);

public:
    bool Load(const std::string& filename) override;
    std::string GetOption(const std::string& key, const std::string& defaultValue = "",
                            bool isBase64Encoded = false) override;
    SectionType* GetSection(const std::string& key) override;
    bool SetOption(const std::string& key, int value) override;
    bool SetOption(const std::string& key, double value) override;
    bool SetOption(const std::string& key, bool value) override;
    bool SetOption(const std::string& key, const std::string& value, bool encodeBase64 = false) override;
};


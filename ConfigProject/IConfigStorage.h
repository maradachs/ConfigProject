#pragma once
#include <string>
#include "SectionType.h"

class IConfigStorage
{
public:
	virtual ~IConfigStorage() = default;
    virtual bool Load(const std::string& filename) = 0;
    virtual std::string GetOption(const std::string& key, const std::string& defaultValue = "",
                    bool isBase64Encoded = false) = 0;
    virtual SectionType* GetSection(const std::string& key) = 0;
    virtual bool SetOption(const std::string& key, int value) = 0;
    virtual bool SetOption(const std::string& key, double value) = 0;
    virtual bool SetOption(const std::string& key, bool value) = 0;
    virtual bool SetOption(const std::string& key, const std::string& value, bool encodeBase64 = false) = 0;

};


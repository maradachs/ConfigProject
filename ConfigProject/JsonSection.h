#pragma once
#include <rapidjson/document.h>
#include "SectionType.h"


class JsonSection : public SectionType
{
private:
    rapidjson::Value* section_;
public:
    explicit JsonSection(rapidjson::Value* section);
    bool IsObject() const override;
    bool HasMember(const std::string& name) const override;
    const SectionType& operator[](const std::string& name) const override;
    int GetInt() const override;
    bool GetBool() const override;
};


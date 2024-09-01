#pragma once
#include <string>

class SectionType
{
public:
    virtual ~SectionType() = default;

    virtual bool IsObject() const = 0;
    virtual bool HasMember(const std::string& name) const = 0;
    virtual const SectionType& operator[](const std::string& name) const = 0;
    virtual int GetInt() const = 0;
    virtual bool GetBool() const = 0;

protected:
    SectionType() = default;
};


#include "JsonSection.h"
#include <rapidjson/document.h>

JsonSection::JsonSection(rapidjson::Value* section) : section_(section) {}

bool JsonSection::IsObject() const {
    return section_->IsObject();
}

bool JsonSection::HasMember(const std::string& name) const {
    return section_->HasMember(name.c_str());
}

const SectionType& JsonSection::operator[](const std::string& name) const {
    return *new JsonSection(&(*section_)[name.c_str()]);
}

int JsonSection::GetInt() const {
    return section_->GetInt();
}

bool JsonSection::GetBool() const {
    return section_->GetBool();
}

#include "JsonConfig.h"
#include "JsonSection.h"
#include "Base64Helper.h"

#include <iostream>
#include <cstdio>
#include "ErrorMessage.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>
#include <rapidjson/error/en.h>


using namespace std;
using namespace rapidjson;


Value* JsonConfig::FindValueByKey(const string& key) {
    Value* current = &doc;
    vector<string> elements = ExtractElements(key);

    for (const string& element : elements) {
        if (!current->IsObject() || !current->HasMember(element.c_str())) {
            return nullptr;
        }
        current = &(*current)[element.c_str()];
    }

    return current;
}
vector<string> JsonConfig::ExtractElements(const string& key) {
    vector<string> elements;
    size_t start = 0, end = 0;
    while ((end = key.find('.', start)) != string::npos) {
        elements.push_back(key.substr(start, end - start));
        start = end + 1;
    }
    elements.push_back(key.substr(start));
    return elements;
}
bool JsonConfig::Load(const string& filename) {
    FILE* fp = nullptr;
    fopen_s(&fp, filename.c_str(), "r");
    if (!fp) {
        cerr << "Error opening file: " << filename << endl;
        return false;
    }

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    doc.ParseStream(is);
    fclose(fp);

    if (doc.HasParseError()) {
        cerr << "JSON parse error: " << GetParseError_En(doc.GetParseError())
            << " at offset " << doc.GetErrorOffset() << endl;
        return false;
    }
    return true;
}
string JsonConfig::GetOption(const string& key, const string& defaultValue, bool isBase64Encoded) {

    Value* value = FindValueByKey(key);
    if (value) {
        if (value->IsString()) {
            string result = value->GetString();
            if (isBase64Encoded) {
                result = Base64Helper::Decode(result);
            }
            return result;
        }
        else if (value->IsInt()) {
            return to_string(value->GetInt());
        }
        else if (value->IsDouble()) {
            return to_string(value->GetDouble());
        }
        else if (value->IsBool()) {
            return value->GetBool() ? "true" : "false";
        }
    }
    return defaultValue;
}
SectionType* JsonConfig::GetSection(const string& key) {
    auto value = FindValueByKey(key);
    return value ? new JsonSection(value) : nullptr;
}
bool JsonConfig::SetOption(const string& key, int value) {
    Value* v = FindValueByKey(key);
    if (v && v->IsInt()) {
        v->SetInt(value);
        return true;
    }
    return false;
}
bool JsonConfig::SetOption(const string& key, double value) {
    Value* v = FindValueByKey(key);
    if (v && v->IsDouble()) {
        v->SetDouble(value);
        return true;
    }
    return false;
}

bool JsonConfig::SetOption(const string& key, bool value) {
    Value* v = FindValueByKey(key);
    if (v && v->IsBool()) {
        v->SetBool(value);
        return true;
    }
    return false;
}

bool JsonConfig::SetOption(const string& key, const string& value, bool encodeBase64) {
    Value* v = FindValueByKey(key);
    if (v && v->IsString()) {
        string finalValue = value;
        if (encodeBase64) {
            finalValue = Base64Helper::Encode(value);
        }
        v->SetString(finalValue.c_str(), doc.GetAllocator());
        return true;
    }
    return false;
};

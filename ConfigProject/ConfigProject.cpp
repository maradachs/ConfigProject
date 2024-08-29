#include <iostream>

#include <vector> 
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/error/en.h>

#include <cstdio>


using namespace std;
using namespace rapidjson;


class ErrorMessage {
public:
    void ShowMsg(const string& message) {
        cout << message << endl;
    }
};
class CConfig {
private:
    Document doc;

    Value* FindValueByKey(const string& key) {
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
    vector<string> ExtractElements(const string& key) {
        vector<string> elements;
        size_t start = 0, end = 0;
        while ((end = key.find('.', start)) != string::npos) {
            elements.push_back(key.substr(start, end - start));
            start = end + 1;
        }
        elements.push_back(key.substr(start));
        return elements;
    }
public:
    bool Load(const string& filename) {
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
	string GetOption(const string& key, const string& defaultValue = "0"){

        Value* value = FindValueByKey(key);
        if (value) {
            if (value->IsString()) {
                return value->GetString();
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
};
class DB {
public:
	string Host;
	string Instance;
	int Port;
};

bool fnApplySettings(CConfig* cfg)
{
	if (!cfg)
	{
		return false;
	}

	DB db;
	db.Host = cfg->GetOption("App.Globals.DB.HostName");
	db.Instance = cfg->GetOption("App.Globals.DB.InstanceName");
	db.Port = stoi(cfg->GetOption("App.Globals.DB.PortNo"));

	return true;
}


int main() {
    CConfig cfg;
    auto* file = "Settings.json";
    if (!cfg.Load(file))  return 0;

    ErrorMessage error_messanger;

    if (!fnApplySettings(&cfg)) {
        error_messanger.ShowMsg("Can’t apply settings :(");
    }
    else {
        error_messanger.ShowMsg("OK :)");
    }

    return 0;
}

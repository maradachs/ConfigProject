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
    Value* GetSection(const string& key) {
        return FindValueByKey(key);
    }
    bool SetOption(const string& key, int value) {
        Value* v = FindValueByKey(key);
        if (v && v->IsInt()) {
            v->SetInt(value);
            return true;
        }
        return false;
    }
    bool SetOption(const string& key, double value) {
        Value* v = FindValueByKey(key);
        if (v && v->IsDouble()) {
            v->SetDouble(value);
            return true;
        }
        return false;
    }

    bool SetOption(const string& key, bool value) {
        Value* v = FindValueByKey(key);
        if (v && v->IsBool()) {
            v->SetBool(value);
            return true;
        }
        return false;
    }

    bool SetOption(const string& key, const string& value) {
        Value* v = FindValueByKey(key);
        if (v && v->IsString()) {
            v->SetString(value.c_str(), doc.GetAllocator());
            return true;
        }
        return false;
    }
};
class DB {
public:
	string Host;
	string Instance;
	int Port;
};

class MainFrame {
public:
    int x;
    int y;
    int Width;
    int Height;
    double FontScale;

    void MaximizeWind() {
        cout << "Window maximized." << endl;
    }
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

    MainFrame mainFrame;

    mainFrame.FontScale = stod(cfg->GetOption("App.Globals.Frames.RootFrame.FontScale"));

    Value* sect = cfg->GetSection("App.Globals.Frames.RootFrame.Placement");

    if (sect && sect->IsObject()) {
        if (sect->HasMember("FullScreen") && (*sect)["FullScreen"].GetBool()) {
            mainFrame.MaximizeWind();
        }
        else {
            mainFrame.x = (*sect)["ScreenPosX"].GetInt();
            mainFrame.y = (*sect)["ScreenPosY"].GetInt();
            mainFrame.Width = (*sect)["Width"].GetInt();
            mainFrame.Height = (*sect)["Height"].GetInt();
        }
    }

    ErrorMessage error_messanger;

    if (!sect || !cfg->SetOption("App.Globals.Frames.RootFrame.Placement.ScreenPosX", mainFrame.x + 10)) {
        error_messanger.ShowMsg("Can’t save option ScreenPosX :(");
    }

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

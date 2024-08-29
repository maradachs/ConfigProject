#include <iostream>

#include <vector> 
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/error/en.h>

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#include <cstdio>


using namespace std;
using namespace rapidjson;


class Base64Helper {
public:
    static string Encode(const string& data) {
        BIO* bio, * b64;
        BUF_MEM* bufferPtr;

        b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // Ѕез переноса строки
        bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);

        BIO_write(bio, data.c_str(), data.length());
        BIO_flush(bio);
        BIO_get_mem_ptr(bio, &bufferPtr);

        string encoded(bufferPtr->data, bufferPtr->length);
        BIO_free_all(bio);

        return encoded;
    }

    static string Decode(const string& encoded) {
        BIO* bio, * b64;
        char* buffer = new char[encoded.length()];
        memset(buffer, 0, encoded.length());

        b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // Ѕез переноса строки
        bio = BIO_new_mem_buf(encoded.c_str(), -1);
        bio = BIO_push(b64, bio);

        int decodedLength = BIO_read(bio, buffer, encoded.length());
        BIO_free_all(bio);

        string decoded(buffer, decodedLength);
        delete[] buffer;

        return decoded;
    }
};
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
	string GetOption(const string& key, const string& defaultValue = "", bool isBase64Encoded = false){

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

    bool SetOption(const string& key, const string& value, bool encodeBase64 = false) {
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
    }
};
class DB {
public:
	string Host;
	string Instance;
	int Port;
    string Password;
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
    try {
        db.Port = stoi(cfg->GetOption("App.Globals.DB.PortNo"));
    }
    catch (exception& e) {
        cerr << "Error: Invalid PortNo value. " << e.what() << endl;
        return false;
    }
    db.Password = cfg->GetOption("App.Globals.DB.Password", "defaultPassword", true);

    MainFrame mainFrame;
    try {
        mainFrame.FontScale = stod(cfg->GetOption("App.Globals.Frames.RootFrame.FontScale"));
    }
    catch (exception& e) {
        cerr << "Error: Invalid FontScale value. " << e.what() << endl;
        return false;
    }
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
        error_messanger.ShowMsg("CanТt save option ScreenPosX :(");
    }

    if (!cfg->SetOption("App.Globals.DB.Password", "1234", true)) {
        error_messanger.ShowMsg("CanТt save option Password :(");
    }

    return true;
}


int main() {
    CConfig cfg;
    auto* file = "Settings.json";
    if (!cfg.Load(file))  return 0;

    ErrorMessage error_messanger;

    if (!fnApplySettings(&cfg)) {
        error_messanger.ShowMsg("CanТt apply settings :(");
    }
    else {
        error_messanger.ShowMsg("OK :)");
    }

    return 0;
}

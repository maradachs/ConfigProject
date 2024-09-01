#include <iostream>

#include <vector> 
#include <string>

#include "Base64Helper.h"
#include "ErrorMessage.h"
#include "ConfigFactory.h"
#include "JsonConfig.h"


#include <rapidjson/document.h>


#include <cstdio>


using namespace std;
using namespace rapidjson;

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
bool fnApplySettings(IConfigStorage* cfg)
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
    
    SectionType* sect = cfg->GetSection("App.Globals.Frames.RootFrame.Placement");

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

    if (!cfg->SetOption("App.Globals.DB.Password", "1234", true)) {
        error_messanger.ShowMsg("Can’t save option Password :(");
    }
    
    return true;
}


int main() {
    //CConfig cfg;
    IConfigStorage* cfg = ConfigFactory::CreateConfig("json");
    auto* file = "Settings.json";
    if (!cfg->Load(file))  return 0;

    ErrorMessage error_messanger;

    if (!fnApplySettings(cfg)) {
        error_messanger.ShowMsg("Can’t apply settings :(");
    }
    else {
        error_messanger.ShowMsg("OK :)");
    }

    return 0;
}

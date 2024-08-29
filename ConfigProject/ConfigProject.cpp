#include <iostream> 

//#include <vector> 
#include <string> 


#include <rapidjson/document.h> 
#include <rapidjson/filereadstream.h> 

//#include <cstdio> 


using namespace std;

class CConfig {
public:
    void Load(const string& filename) {
    
    }
	string GetOption(const string& key){

		return "";
	}
};
class ErrorMessage {
public:
	void ShowMsg(const string& message) {
		cout << message << endl;
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
	db.Port = stoi(cfg->GetOption("App.Global.DB.PortNo"));


	return true;
}


int main() {
    CConfig cfg;
    auto* file = "Settings.json";
    cfg.Load(file);

    ErrorMessage error_messanger;

    if (!fnApplySettings(&cfg)) {
        error_messanger.ShowMsg("Can’t apply settings :(");
    }
    else {
        error_messanger.ShowMsg("OK :)");
    }

    return 0;
}

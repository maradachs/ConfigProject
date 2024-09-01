#include "ConfigFactory.h"
#include "JsonConfig.h"

IConfigStorage* ConfigFactory::CreateConfig(const std::string& type) {
    if (type == "json") { 
        return new JsonConfig();
    }
    
    return nullptr;
}
#pragma once

#include "IConfigStorage.h"
#include <string>

class ConfigFactory
{
public:
	static IConfigStorage* CreateConfig(const std::string& type);
};


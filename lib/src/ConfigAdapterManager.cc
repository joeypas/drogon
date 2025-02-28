#include "ConfigAdapterManager.h"
#include "JsonConfigAdapter.h"
#include "YamlConfigAdapter.h"
#include <algorithm>

using namespace drogon;
#define REGISTER_CONFIG_ADAPTER(adapter)                                    \
    {                                                                       \
        auto adapterPtr = std::make_shared<adapter>();                      \
        auto exts = adapterPtr->getExtensions();                            \
        for (auto ext : exts)                                               \
        {                                                                   \
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower); \
            adapters_[ext] = adapterPtr;                                    \
        }                                                                   \
    }
ConfigAdapterManager &ConfigAdapterManager::instance()
{
    static ConfigAdapterManager instance;
    return instance;
}

Json::Value ConfigAdapterManager::getJson(const std::string &configFile) const
    noexcept(false)
{
    auto pos = configFile.find_last_of('.');
    if (pos == std::string::npos)
    {
        throw std::runtime_error("Invalid config file name!");
    }
    auto ext = configFile.substr(pos + 1);
    // convert ext to lower case
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    auto it = adapters_.find(ext);
    if (it == adapters_.end())
    {
        throw std::runtime_error("No valid parser for this config file!");
    }
    return it->second->getJson(configFile);
}

ConfigAdapterManager::ConfigAdapterManager()
{
    REGISTER_CONFIG_ADAPTER(JsonConfigAdapter);
    REGISTER_CONFIG_ADAPTER(YamlConfigAdapter);
}
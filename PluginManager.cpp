/* GPL 2015 Jake Sebastian-Jones */

#include <list>
#include <string>
#include <iostream>
#include "PluginManager.hpp"
#include "FooComponent.hpp"

const std::string PluginManager::pluginCallbackName = "registerPlugin";

PluginManager::PluginManager()
{
}

void PluginManager::registerComponent(FooComponentFactory *factory)
{
    std::cout << "[PluginManager]: Registered a Foo Component" << std::endl;
    mFooFactories.push_back(factory);
}

void PluginManager::registerComponent(BarComponentFactory *factory)
{
    std::cout << "[PluginManager]: Registered a Bar Component" << std::endl;
    mBarFactories.push_back(factory);
}

typedef std::list<FooComponentFactory *> FooFactories;
typedef std::list<BarComponentFactory *> BarFactories;

FooFactories &PluginManager::getFooFactories()
{
    return mFooFactories;
}

BarFactories &PluginManager::getBarFactories()
{
    return mBarFactories;
}

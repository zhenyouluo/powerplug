/* GPL 2015 Jake Sebastian-Jones */

#include <iostream>

#include "Version.hpp"
#include "PluginManager.hpp"
#include "FooComponent.hpp"


namespace FooPlugin1
{

class VerySpecificTypeOfFoo : public FooComponent
{
public:
    VerySpecificTypeOfFoo() : FooComponent()
    {
        std::cout
            << "[FooPlugin]: Plugin instantiated a Foo Component" << std::endl;
    }
};

class Factory : public FooComponentFactory
{
public:
    FooComponent *create() const
    {
        return new VerySpecificTypeOfFoo();
    }
};

}  // namespace FooPlugin1

extern "C" void registerPlugin(PluginManager *pluginManager)
{
    FooComponentFactory *factory = new FooPlugin1::Factory();
    pluginManager->registerComponent(factory);
}

extern "C" int getCompatibleVersion(void)
{
    return VERSION;
}


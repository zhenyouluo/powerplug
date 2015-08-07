/* GPL 2015 Jake Sebastian-Jones */

#include <iostream>

#include "PluginManager.hpp"
#include "BarComponent.hpp"


namespace BarPlugin1
{

class VerySpecificTypeOfBar : public BarComponent
{
public:
    VerySpecificTypeOfBar() : BarComponent()
    {
        std::cout
            << "[BarPlugin]: Plugin instantiated a Bar Component" << std::endl;
    }
};

class Factory : public BarComponentFactory
{
public:
    BarComponent *create() const
    {
        return new VerySpecificTypeOfBar();
    }
};

}  // namespace BarPlugin1

extern "C" void registerPlugin(PluginManager *pluginManager)
{
    BarComponentFactory *factory = new BarPlugin1::Factory();
    pluginManager->registerComponent(factory);
}

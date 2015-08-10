/* GPL 2015 Jake Sebastian-Jones */

#ifndef __INCLUDE_PLUGINMANAGER_HPP__
#define __INCLUDE_PLUGINMANAGER_HPP__

#include <string>
#include <list>
#include <map>
#include <typeinfo>

#include "Factory.hpp"
#include "FooComponent.hpp"
#include "BarComponent.hpp"

class PluginManager
{
public:
    PluginManager();

    void registerComponent(FooComponentFactory *factory);
    void registerComponent(BarComponentFactory *factory);

    typedef std::list<FooComponentFactory *> FooFactories;
    typedef std::list<BarComponentFactory *> BarFactories;

    FooFactories &getFooFactories();

    BarFactories &getBarFactories();

    typedef void (*registration_func_t)(PluginManager *);
    typedef int  (*query_version_func_t)(void);

    static const std::string pluginCallbackName;
    static const std::string pluginVersionApi;

private:
    FooFactories mFooFactories;
    BarFactories mBarFactories;
};
#endif  /* __INCLUDE_PLUGINMANAGER_HPP__ */

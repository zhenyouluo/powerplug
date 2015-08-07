/* GPL 2015 Jake Sebastian-Jones */

#include <dirent.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

#include <list>
#include <iostream>
#include <string>

#include "PluginManager.hpp"
#include "BarComponent.hpp"
#include "FooComponent.hpp"

#define MAX_PATHLEN (4096)
#define MAX_FILELEN (256)

#define DIRECTORY_SEPARATOR "/"
#ifdef WIN32
    #undef DIRECTORY_SEPARATOR
    #define DIRECTORY_SEPARATOR "\\"
#endif

static const std::string getCwd()
{
    char   buffer[MAX_PATHLEN + MAX_FILELEN];
    getcwd(buffer, MAX_PATHLEN + MAX_FILELEN);
    return std::string(buffer);
}


static void loadPlugins(PluginManager *pm)
{
    DIR * const pluginDir = opendir(".");

    if (pluginDir)
    {
        const struct dirent *file;

        while (file = readdir(pluginDir))
        {
            const std::string absoluteFileName =
                getCwd() + DIRECTORY_SEPARATOR + file->d_name;

            void * const handle = dlopen(absoluteFileName.c_str(), RTLD_LAZY);

            if (!handle)
            {
                // std::cerr << "Cannot open library: " << dlerror() << '\n';
                continue;
            }

            const PluginManager::registration_func_t pluginCallback =
                reinterpret_cast<PluginManager::registration_func_t>(
                    dlsym(handle, PluginManager::pluginCallbackName.c_str()));

            const char * const dlsym_error = dlerror();

            if (dlsym_error)
            {
                std::cerr << "Cannot load symbol \""
                    << PluginManager::pluginCallbackName << "\": "
                    << dlsym_error << std::endl;
                dlclose(handle);
                continue;
            }

            else
            {
                std::cout << "[Main]: Loading plugin \""
                                    << file->d_name << "\"" << std::endl;
                pluginCallback(pm);
            }
        }
    }
    closedir(pluginDir);
}

int main(int argc, char *argv[])
{
    PluginManager manager;

    loadPlugins(&manager);

    PluginManager::FooFactories fooFactories = manager.getFooFactories();
    PluginManager::BarFactories barFactories = manager.getBarFactories();


    BarComponentFactory *barFact = barFactories.front();
    if (barFact)
    {
        std::cout
            << "[Main]: Asking plugin's factory to create a Bar Component "
            << std::endl;
        barFact->create();
    }

    FooComponentFactory *fooFact = fooFactories.front();
    if (fooFact)
    {
        std::cout
            << "[Main]: Asking plugin's factory to create a Foo Component "
            << std::endl;
        fooFact->create();
    }

    return 0;
}

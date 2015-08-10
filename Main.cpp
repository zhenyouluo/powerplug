/* GPL 2015 Jake Sebastian-Jones */

/**
 Largely inspired by Cygon's Blog
@see http://blog.nuclex-games.com/tutorials/cxx/plugin-architecture/

Here are some of the justifications for the design decisions:

1. Choice of plugin registration API. I.e. what function should be exposed in
   the shared library?

   Choices considered:
        a. void *createInstance(const char*)

           In this case we call createInstance in all the shared libs, giving
           the name of the component as the argument (for example "Renderer"),
           any of them which don't return NULL are renderers and you can
           cast the pointer to a Renderer object.

           Criticism:
           This method has several flaws.
            * It requires casting the return value of the registration
            * The registration always instatiates the component
              (which you might not want to do - you might want to make a choice
               first)
            * Passing a string as the parameter doesn't take advantage of type
              safety.
            * When asked for a "Renderer" a plugin could just instantiate a
              "Decoder", or give a garbage non-null value (easy to write a
              plugin that crashes the whole system)
 */

#include <dirent.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

#include <list>
#include <iostream>
#include <string>

#include "Version.hpp"
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


            /* Load the registration function... */
            const PluginManager::registration_func_t pluginCallback =
                reinterpret_cast<PluginManager::registration_func_t>(
                    dlsym(handle, PluginManager::pluginCallbackName.c_str()));

            const char * const reg_sym_err = dlerror();

            if (reg_sym_err)
            {
                std::cerr << "Cannot load symbol \""
                    << PluginManager::pluginCallbackName << "\": "
                    << reg_sym_err << std::endl;
                dlclose(handle);
                continue;
            }

            /* Load the versioning function... */
            const PluginManager::query_version_func_t queryVersion =
                reinterpret_cast<PluginManager::query_version_func_t>(
                    dlsym(handle, PluginManager::pluginVersionApi.c_str()));

            const char * const ver_sym_err = dlerror();

            if (ver_sym_err)
            {
                std::cerr << "Cannot load symbol \""
                    << PluginManager::pluginCallbackName << "\": "
                    << ver_sym_err << std::endl;
                dlclose(handle);
                continue;
            }

            std::cout << "[Main]: Loading plugin \""
                                << file->d_name << "\"" << std::endl;

            const int expectedVersion = queryVersion();
            if (expectedVersion != VERSION)
            {
                std::cerr << "[Main]: Plugin expects system version "
                          << expectedVersion << ", but system is version "
                          << VERSION << std::endl;
                continue;
            }

            pluginCallback(pm);
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


    if (!barFactories.empty())
    {
        BarComponentFactory *barFact = barFactories.front();
        std::cout
            << "[Main]: Asking plugin's factory to create a Bar Component "
            << std::endl;
        barFact->create();
    }

    if (!fooFactories.empty())
    {
        FooComponentFactory *fooFact = fooFactories.front();
        std::cout
            << "[Main]: Asking plugin's factory to create a Foo Component "
            << std::endl;
        fooFact->create();
    }

    return 0;
}

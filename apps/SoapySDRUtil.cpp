// Copyright (c) 2014-2014 Josh Blum
// SPDX-License-Identifier: BSL-1.0

#include <SoapySDR/Version.hpp>
#include <SoapySDR/Modules.hpp>
#include <SoapySDR/Registry.hpp>
#include <SoapySDR/Device.hpp>
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <getopt.h>

/***********************************************************************
 * Print help message
 **********************************************************************/
static void printHelp(void)
{
    std::cout << "Usage SoapySDRUtil [--find[=\"driver=foo,type=bar\"]] [--help]" << std::endl;
    std::cout << std::endl;
}

/***********************************************************************
 * Print version and module info
 **********************************************************************/
static void printInfo(void)
{
    std::cout << "API Version: v" << SoapySDR::getAPIVersion() << std::endl;
    std::cout << "ABI Version: v" << SoapySDR::getABIVersion() << std::endl;
    std::cout << "Install root: " << SoapySDR::getRootPath() << std::endl;

    std::vector<std::string> modules = SoapySDR::listModules();
    for (size_t i = 0; i < modules.size(); i++)
    {
        std::cout << "Module found: " << modules[i] << std::endl;
    }
    if (modules.empty()) std::cout << "No modules found!" << std::endl;

    std::cout << "Loading modules... " << std::flush;
    SoapySDR::loadModules();
    std::cout << "done" << std::endl;

    std::cout << "Available factories...";
    const SoapySDR::FindFunctions factories = SoapySDR::Registry::listFindFunctions();
    for (SoapySDR::FindFunctions::const_iterator it = factories.begin(); it != factories.end(); ++it)
    {
        std::cout << it->first << ", ";
    }
    if (factories.empty()) std::cout << "No factories found!" << std::endl;
    std::cout << std::endl;
}

/***********************************************************************
 * Find devices and print args
 **********************************************************************/
static void findDevices(void)
{
    std::string argStr;
    if (optarg != NULL) argStr = optarg;

    std::vector<SoapySDR::Kwargs> results = SoapySDR::Device::enumerate(argStr);
    for (size_t i = 0; i < results.size(); i++)
    {
        std::cout << "Found device " << i << std::endl;
        for (SoapySDR::Kwargs::const_iterator it = results[i].begin(); it != results[i].end(); ++it)
        {
            std::cout << "  " << it->first << " = " << it->second << std::endl;
        }
        std::cout << std::endl;
    }
    if (results.empty()) std::cout << "No devices found!" << std::endl;
    std::cout << std::endl;
}

/***********************************************************************
 * Print help message
 **********************************************************************/
int main(int argc, char *argv[])
{
    std::cout << "######################################################" << std::endl;
    std::cout << "## Soapy SDR -- the SDR abstraction library" << std::endl;
    std::cout << "######################################################" << std::endl;
    std::cout << std::endl;

    /*******************************************************************
     * parse command line options
     ******************************************************************/
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"find", optional_argument, 0, 'f'},
        {"info", optional_argument, 0, 'i'},
        {0, 0, 0,  0}
    };
    int long_index = 0;
    int option = 0;
    while ((option = getopt_long_only(argc, argv, "", long_options, &long_index)) != -1)
    {
        switch (option)
        {
        case 'h': printHelp(); return EXIT_SUCCESS;
        case 'i': printInfo(); return EXIT_SUCCESS;
        case 'f': findDevices(); return EXIT_SUCCESS;
        }
    }

    //unknown or unspecified options, do help...
    printHelp();
    return EXIT_SUCCESS;
}

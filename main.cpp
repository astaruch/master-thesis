#include <iostream>

#include <spdlog/spdlog.h>

#include "libs/config.h"

int
main()
{
    spdlog::info("Starting the app");

    phishapp::Config::set_path("config.yaml");
    phishapp::Config::save();

    return 0;
}
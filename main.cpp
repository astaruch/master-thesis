#include <iostream>

#include <spdlog/spdlog.h>

#include "libs/config.h"
#include "libs/database.h"

int
main()
{
    spdlog::info("Starting the app");

    phishapp::Config::set_path("config.yaml");
    phishapp::Config::save();

    auto db = phishapp::Database();


    return 0;
}
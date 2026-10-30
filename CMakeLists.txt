cmake_minimum_required(VERSION 3.16)

# Do not download unused ESP-IDF submodules such as the Wi-Fi library.
set(ENV{IDF_SKIP_CHECK_SUBMODULES} "1")

# Only the application and the components it depends on are built.
set(COMPONENTS main)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(e_paper)

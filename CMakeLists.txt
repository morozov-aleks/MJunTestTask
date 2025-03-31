cmake_minimum_required(VERSION 3.1)
project(MJunTestTask)

find_package(ProteiCMakeModules)
protei_init(CXX_STANDARD c++17)

find_package(jsoncpp REQUIRED)

file(GLOB_RECURSE SOURCES src/*)

protei_add_target(MJunTestTask EXECUTABLE
        SOURCES ${SOURCES}
        HEADERS src
        DEPENDS PSC::psc ATE::BaseLogic ATE::Common jsoncpp_lib)
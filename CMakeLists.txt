cmake_minimum_required(VERSION 3.1)
project(project-loader)

include(ExternalProject)

ExternalProject_Add(oatpp
        GIT_REPOSITORY "https://github.com/oatpp/oatpp.git"
        GIT_TAG origin/master
)

ExternalProject_Add(oatpp_swagger
        GIT_REPOSITORY "https://github.com/oatpp/oatpp-swagger.git"
        GIT_TAG origin/master
        DEPENDS oatpp
)

ExternalProject_Add(main
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/main
        INSTALL_COMMAND cmake -E echo "SKIP INSTALL"
        DEPENDS oatpp oatpp_swagger
)

ExternalProject_Get_Property(main BINARY_DIR)
message("BINARY_DIR=${BINARY_DIR}")

enable_testing()
add_test(
        NAME
            all-tests
        COMMAND
            ${BINARY_DIR}/crud-test
)
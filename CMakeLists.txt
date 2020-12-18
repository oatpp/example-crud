cmake_minimum_required(VERSION 3.1)
project(crud)

set(CMAKE_CXX_STANDARD 11)

add_library(crud-lib
        src/controller/StaticController.hpp
        src/controller/UserController.hpp
        src/db/UserDb.hpp
        src/dto/PageDto.hpp
        src/dto/StatusDto.hpp
        src/dto/UserDto.hpp
        src/service/UserService.cpp
        src/service/UserService.hpp
        src/AppComponent.hpp
        src/DatabaseComponent.hpp
        src/SwaggerComponent.hpp
        src/ErrorHandler.cpp
        src/ErrorHandler.hpp)

## include directories

target_include_directories(crud-lib PUBLIC src)


## link libs

find_package(SQLite3 REQUIRED)

find_package(oatpp          1.2.0 REQUIRED)
find_package(oatpp-swagger  1.2.0 REQUIRED)
find_package(oatpp-sqlite   1.2.0 REQUIRED)

target_link_libraries(crud-lib
        # Oat++
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
        PUBLIC oatpp::oatpp-sqlite

        # SQLite
        SQLite::SQLite3
)

add_definitions(
        ## define path to swagger-ui static resources folder
        -DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res"

        ## SQLite databse file
        -DDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/db.sqlite"

        ## Path to database migration scripts
        -DDATABASE_MIGRATIONS="${CMAKE_CURRENT_SOURCE_DIR}/sql"
)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    find_package(Threads REQUIRED)
    target_link_libraries(crud-lib INTERFACE Threads::Threads ${CMAKE_DL_LIBS})
endif()

## add executables

add_executable(crud-exe src/App.cpp)
target_link_libraries(crud-exe crud-lib)

add_executable(crud-test
        test/tests.cpp
)
target_link_libraries(crud-test crud-lib)

enable_testing()
add_test(project-tests crud-test)

cmake_minimum_required(VERSION 3.1)
project(crud)

set(CMAKE_CXX_STANDARD 11)

add_library(crud-lib
        src/AppComponent.hpp
        src/Logger.hpp
        src/SwaggerComponent.hpp
        src/controller/UserController.hpp
        src/db/Database.cpp
        src/db/Database.hpp
        src/db/model/User.hpp
        src/dto/UserDto.hpp
)

target_include_directories(crud-lib
        PUBLIC src
)

find_package(oatpp          0.19.1 REQUIRED)
find_package(oatpp-swagger  0.19.1 REQUIRED)

target_link_libraries(crud-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
)

add_definitions(-DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res")

add_executable(crud-exe src/App.cpp)
target_link_libraries(crud-exe crud-lib)

add_executable(crud-test
        test/tests.cpp
)
target_link_libraries(crud-test crud-lib)

# Example-CRUD [![Build Status](https://dev.azure.com/lganzzzo/lganzzzo/_apis/build/status/oatpp.example-crud?branchName=master)](https://dev.azure.com/lganzzzo/lganzzzo/_build/latest?definitionId=9?branchName=master)

Example project how-to create basic CRUD endpoints and document them with Swagger-UI and OpenApi 3.0.0

More about oat++:
- Website: [https://oatpp.io](https://oatpp.io)
- Docs: [https://oatpp.io/docs/start](https://oatpp.io/docs/start)
- Oat++ Repo: [https://github.com/oatpp/oatpp](https://github.com/oatpp/oatpp)

## Overview
This project is using `oatpp` and `oatpp-swagger` modules.

### Project layout

```

- CMakeLists.txt          // project loader script. load and build dependencies 
- main/                   // main project directory
    |
    |- CMakeLists.txt     // projects CMakeLists.txt
    |- src/               // source folder
    |- test/              // test folder

```
```
- src/
    |
    |- controller/              // Folder containing UserController where all endpoints are declared
    |- db/                      // Folder with database mock
    |- dto/                     // DTOs are declared here
    |- SwaggerComponent.hpp     // Swagger-UI config
    |- AppComponent.hpp         // Service config
    |- Logger.hpp               // Application Logger
    |- App.cpp                  // main() is here
    
```

---

### Build and Run

#### Using CMake

```
$ mkdir build && cd build
$ cmake ..
$ make run        ## Download, build, and install all dependencies. Run project

```

#### In Docker

```
$ docker build --no-cache -t example-crud .
$ docker run -t example-crud
```

---

### Endpoints declaration

#### Create User

```c++
ENDPOINT_INFO(createUser) {
  info->summary = "Create new User";
  info->addConsumes<UserDto::ObjectWrapper>("application/json");
  info->addResponse<UserDto::ObjectWrapper>(Status::CODE_200, "application/json");
}
ENDPOINT("POST", "demo/api/users", createUser,
         BODY_DTO(UserDto::ObjectWrapper, userDto)) {
  return createDtoResponse(Status::CODE_200, m_database->createUser(userDto));
}
```

#### Update User

```c++
ENDPOINT_INFO(putUser) {
  info->summary = "Update User by userId";
  info->addConsumes<UserDto::ObjectWrapper>("application/json");
  info->addResponse<UserDto::ObjectWrapper>(Status::CODE_200, "application/json");
  info->addResponse<String>(Status::CODE_404, "text/plain");
}
ENDPOINT("PUT", "demo/api/users/{userId}", putUser,
         PATH(Int32, userId),
         BODY_DTO(UserDto::ObjectWrapper, userDto)) {
  userDto->id = userId;
  return createDtoResponse(Status::CODE_200, m_database->updateUser(userDto));
}
```

#### Get one User

```c++
ENDPOINT_INFO(getUserById) {
  info->summary = "Get one User by userId";
  info->addResponse<UserDto::ObjectWrapper>(Status::CODE_200, "application/json");
  info->addResponse<String>(Status::CODE_404, "text/plain");
}
ENDPOINT("GET", "demo/api/users/{userId}", getUserById,
         PATH(Int32, userId)) {
  auto user = m_database->getUserById(userId);
  OATPP_ASSERT_HTTP(user, Status::CODE_404, "User not found");
  return createDtoResponse(Status::CODE_200, user);
}
```

#### Get list of users

```c++
ENDPOINT_INFO(getUsers) {
  info->summary = "get all stored users";
  info->addResponse<List<UserDto::ObjectWrapper>::ObjectWrapper>(Status::CODE_200, "application/json");
}
ENDPOINT("GET", "demo/api/users", getUsers) {
  return createDtoResponse(Status::CODE_200, m_database->getUsers());
}
```

#### Delete User
```c++
ENDPOINT_INFO(deleteUser) {
  info->summary = "Delete User by userId";
  info->addResponse<String>(Status::CODE_200, "text/plain");
  info->addResponse<String>(Status::CODE_404, "text/plain");
}
ENDPOINT("DELETE", "demo/api/users/{userId}", deleteUser,
         PATH(Int32, userId)) {
  bool success = m_database->deleteUser(userId);
  OATPP_ASSERT_HTTP(success, Status::CODE_417, "User not deleted. Perhaps no such User in the Database");
  return createResponse(Status::CODE_200, "User successfully deleted");
}  
```

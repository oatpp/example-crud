#include "UserService.hpp"
#include "bcrypt.h"

oatpp::Object<ReturnUserDto> UserService::createUser(const oatpp::Object<UserDto> &dto)
{
    dto.get()->password = bcrypt::generateHash(dto.get()->password, 10);
    auto dbResult = m_database->createUser(dto);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");
    return getUserById(result[0].get()->id);
}

oatpp::Object<ReturnUserDto> UserService::verifyUser(const oatpp::Object<LoginUserDto> &dto)
{
    auto user = this->getUserByUsername(dto.get()->username);
    OATPP_ASSERT_HTTP(user.get(), Status::CODE_500, "Unkown error");

    OATPP_ASSERT_HTTP(bcrypt::validatePassword(dto.get()->password, user.get()->password), Status::CODE_404, "User not found");

    auto returnUser = ReturnUserDto::createShared();
    returnUser->id = user->id;
    returnUser->userName = user->userName;
    returnUser->createdAt = user->createdAt;
    returnUser->updatedAt = user->updatedAt;
    returnUser->role = user->role;
    returnUser->email = user->email;

    return returnUser;
}

oatpp::Object<UserDto> UserService::getUserByUsername(const oatpp::String &username)
{
    auto result = m_database->getUserByUsername(username);
    OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage());
    OATPP_ASSERT_HTTP(result->hasMoreToFetch(), Status::CODE_404, "User not found");

    auto users = result->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
    OATPP_ASSERT_HTTP(users->size() == 1, Status::CODE_500, "Unknown error");
    return users[0];
}

oatpp::Object<UserDto> UserService::updateUser(const oatpp::Object<UserDto> &dto)
{
    return oatpp::Object<UserDto>();
}

oatpp::Object<ReturnUserDto> UserService::getUserById(const oatpp::Int32 &id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection> &connection)
{
    auto dbResult = m_database->getUserById(id, connection);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "User not found");

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<ReturnUserDto>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

    return result[0];
}

oatpp::Object<StatusDto> UserService::deleteUserById(const oatpp::Int32 &id)
{
    return oatpp::Object<StatusDto>();
}

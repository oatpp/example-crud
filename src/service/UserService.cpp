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

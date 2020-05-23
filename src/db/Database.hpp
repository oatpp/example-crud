
#ifndef Database_hpp
#define Database_hpp

#include "../dto/UserDto.hpp"
#include "model/User.hpp"

#include "oatpp/core/concurrency/SpinLock.hpp"
#include <unordered_map>

/**
 *  Trivial in-memory Database based on unordered_map container.
 *  For demo purposes only :)
 */
class Database {
private:
  oatpp::concurrency::SpinLock m_lock;
  v_int32 m_idCounter; ///< counter to generate userIds
  std::unordered_map<v_int32, User> m_usersById; ///< Map userId to User
private:
  User serializeFromDto(const oatpp::Object<UserDto>& userDto);
  oatpp::Object<UserDto> deserializeToDto(const User& user);
public:
  
  Database()
    : m_idCounter(0)
  {}

  oatpp::Object<UserDto> createUser(const oatpp::Object<UserDto>& userDto);
  oatpp::Object<UserDto> updateUser(const oatpp::Object<UserDto>& userDto);
  oatpp::Object<UserDto> getUserById(v_int32 id);
  oatpp::List<oatpp::Object<UserDto>> getUsers();
  bool deleteUser(v_int32 id);
  
};

#endif /* Database_hpp */

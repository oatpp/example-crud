//
//  Database.cpp
//  crud
//
//  Created by Leonid on 3/13/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#include "Database.hpp"

User Database::serializeFromDto(const UserDto::ObjectWrapper& userDto){
  User user;
  if(userDto->id){
    user.id = *userDto->id;
  }
  user.firstName = userDto->firstName;
  user.lastName = userDto->lastName;

  for(auto& id : *userDto->friends) {
    user.friends.push_back(id);
  }
  return user;
}

UserDto::ObjectWrapper Database::deserializeToDto(const User& user){
  auto dto = UserDto::createShared();
  dto->id = user.id;
  dto->firstName = user.firstName;
  dto->lastName = user.lastName;
  auto it = user.friends.begin();
  while (it != user.friends.end()) {
    dto->friends->push_back(*it++);
  }
  return dto;
}

UserDto::ObjectWrapper Database::createUser(const UserDto::ObjectWrapper& userDto){
  std::lock_guard<oatpp::concurrency::SpinLock> lock(m_lock);
  auto user = serializeFromDto(userDto);
  user.id = m_idCounter++;
  m_usersById[user.id] = user;
  return deserializeToDto(user);
}

UserDto::ObjectWrapper Database::updateUser(const UserDto::ObjectWrapper& userDto){
  std::lock_guard<oatpp::concurrency::SpinLock> lock(m_lock);
  auto user = serializeFromDto(userDto);
  if(user.id < 0){
    throw std::runtime_error("User Id cannot be less than 0");
  }
  auto it = m_usersById.find(user.id);
  if(it != m_usersById.end()) {
    m_usersById[user.id] = user;
  } else {
    throw new std::runtime_error("Such user not found");
  }
  return deserializeToDto(user);
}

UserDto::ObjectWrapper Database::getUserById(v_int32 id){
  std::lock_guard<oatpp::concurrency::SpinLock> lock(m_lock);
  auto it = m_usersById.find(id);
  if(it == m_usersById.end()){
    return nullptr;
  }
  return deserializeToDto(it->second);
}

oatpp::data::mapping::type::List<UserDto::ObjectWrapper>::ObjectWrapper Database::getUsers(){
  std::lock_guard<oatpp::concurrency::SpinLock> lock(m_lock);
  auto result = oatpp::data::mapping::type::List<UserDto::ObjectWrapper>::createShared();
  auto it = m_usersById.begin();
  while (it != m_usersById.end()) {
    result->push_back(deserializeToDto(it->second));
    it++;
  }
  return result;
}

bool Database::deleteUser(v_int32 id){
  std::lock_guard<oatpp::concurrency::SpinLock> lock(m_lock);
  auto it = m_usersById.find(id);
  if(it == m_usersById.end()){
    return false;
  }
  m_usersById.erase(it);
  return true;
}

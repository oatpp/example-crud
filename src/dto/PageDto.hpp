
#ifndef CRUD_PAGEDTO_HPP
#define CRUD_PAGEDTO_HPP

#include "UserDto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

template<class T>
class PageDto : public oatpp::DTO {

  DTO_INIT(PageDto, DTO)

  DTO_FIELD(UInt32, offset);
  DTO_FIELD(UInt32, limit);
  DTO_FIELD(UInt32, count);
  DTO_FIELD(Vector<T>, items);

};

class UsersPageDto : public PageDto<oatpp::Object<UserDto>> {

  DTO_INIT(UsersPageDto, PageDto<oatpp::Object<UserDto>>)

};

#include OATPP_CODEGEN_END(DTO)

#endif //CRUD_PAGEDTO_HPP

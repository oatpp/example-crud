#ifndef UserDto_hpp
#define UserDto_hpp

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

ENUM(Role, v_int32,
     VALUE(GUEST, 0, "ROLE_GUEST"),
     VALUE(ADMIN, 1, "ROLE_ADMIN")
)

class UserDto : public oatpp::DTO {
  
  DTO_INIT(UserDto, DTO)

  DTO_FIELD(Int32, id, "id");
  DTO_FIELD(String, userName, "username");
  DTO_FIELD(String, email, "email");
  DTO_FIELD(String, password, "password");
};

class ReturnUserDto : public oatpp::DTO {
  DTO_INIT(ReturnUserDto, DTO)

  DTO_FIELD(Int32, id, "id");
  DTO_FIELD(String, userName, "username");
  DTO_FIELD(String, email, "email");
  DTO_FIELD(String, role, "role");
  DTO_FIELD(Int64, createdAt, "created_at");
  DTO_FIELD(Int64, updatedAt, "updated_at");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* UserDto_hpp */

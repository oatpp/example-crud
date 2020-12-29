#ifndef DatabaseTestClient_hpp
#define DatabaseTestClient_hpp

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "dto/UserDto.hpp"

/* Begin Api Client code generation */
#include OATPP_CODEGEN_BEGIN(ApiClient)

/**
 * Test API client.
 * Use this client to call application APIs.
 */
class TestClient : public oatpp::web::client::ApiClient {

  API_CLIENT_INIT(TestClient)

  /*****************************************************************
   * UserController
   *****************************************************************/

  API_CALL("POST", "/users", addUser, BODY_DTO(Object<UserDto>, userDto))
  API_CALL("GET", "/users/{userId}", getUser, PATH(Int32, userId))
  API_CALL("DELETE", "/users/{userId}", deleteUser, PATH(Int32, userId))

  /*****************************************************************/

  // TODO - add more client API calls here

};

/* End Api Client code generation */
#include OATPP_CODEGEN_END(ApiClient)

#endif // DatabaseTestClient_hpp
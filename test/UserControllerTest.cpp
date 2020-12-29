#include "UserControllerTest.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"

#include "controller/UserController.hpp"

#include "app/TestClient.hpp"
#include "app/TestComponent.hpp"

#include <cstdio>

void UserControllerTest::onRun() {

  /* Remove test database file before running the test */
  OATPP_LOGI(TAG, "DB-File='%s'", TESTDATABASE_FILE);
  std::remove(TESTDATABASE_FILE);

  /* Register test components */
  TestComponent component;

  /* Create client-server test runner */
  oatpp::test::web::ClientServerTestRunner runner;

  /* Add UserController endpoints to the router of the test server */
  runner.addController(std::make_shared<UserController>());

  /* Run test */
  runner.run([this, &runner] {

    /* Get client connection provider for Api Client */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider);

    /* Get object mapper component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

    /* Create http request executor for Api Client */
    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);

    /* Create Test API client */
    auto client = TestClient::createShared(requestExecutor, objectMapper);

    auto dto = UserDto::createShared();

    dto->userName = "jondoe";
    dto->email = "jon.doe@abc.com";
    dto->password = "1234";

    /* Call server API */
    auto addedUserResponse = client->addUser(dto);

    /* Assert that server responds with 200 */
    OATPP_ASSERT(addedUserResponse->getStatusCode() == 200);

    /* Read response body as MessageDto */
    auto addedUserDto = addedUserResponse->readBodyToDto<oatpp::Object<UserDto>>(objectMapper.get());

    int addedUserId = addedUserDto->id;

    /* Assert that user has been added */
    auto newUserResponse = client->getUser(addedUserId);

    OATPP_ASSERT(newUserResponse->getStatusCode() == 200);

    auto newUserDto = newUserResponse->readBodyToDto<oatpp::Object<UserDto>>(objectMapper.get());

    OATPP_ASSERT(newUserDto->id == addedUserId);

    /* Delete newly added users */
    auto deletedUserResponse = client->deleteUser(addedUserId);

    OATPP_ASSERT(deletedUserResponse->getStatusCode() == 200);

  }, std::chrono::minutes(10) /* test timeout */);

  /* wait all server threads finished */
  std::this_thread::sleep_for(std::chrono::seconds(1));

  /* stop db connection pool */
  OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);
  dbConnectionProvider->stop();

}
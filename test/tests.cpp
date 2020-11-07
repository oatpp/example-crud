#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/Deserializer.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/parser/json/mapping/Serializer.hpp>
#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/web/protocol/http/encoding/Chunked.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include <oatpp-swagger/Controller.hpp>
#include <oatpp-swagger/Model.hpp>
#include <oatpp-swagger/Resources.hpp>

namespace web_service_base_test {

// dummy_controller class to demo the swagger crash
#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin ApiController codegen section

class dummy_controller : public oatpp::web::server::api::ApiController {
public:

  dummy_controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper) /* Inject object mapper */)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}

  ENDPOINT("GET", "/", root) {
    return createResponse(Status::CODE_200, "Hello World!");
  }

};

#include OATPP_CODEGEN_END(ApiController) ///< End ApiController codegen section

void test() {

  auto swaggerDocInfo = oatpp::swagger::DocumentInfo::Builder()
    .setTitle("title")
    .setDescription("description")
    .setVersion("version")
    .setContactName("contact name")
    .setContactUrl("contact url")
    .setLicenseName("All Rights Reserved.")
    .addServer("http://{localhost}:{45678}", "description")
    .build();

  auto const resources = oatpp::base::Environment::Component<std::shared_ptr<oatpp::swagger::Resources>>(
    oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH));

  // server_connection_provider
  auto server_connection_provider = std::make_shared<oatpp::network::tcp::server::ConnectionProvider>(
    oatpp::network::Address{ "localhost", 56789, oatpp::network::Address::IP_4 });

  // http_router
  auto http_router = oatpp::web::server::HttpRouter::createShared();

  // server_connection_handler
  auto server_connection_handler = std::make_shared<oatpp::web::server::HttpConnectionHandler>(http_router);

  // api_object_mapper
  auto object_mapper =
    oatpp::base::Environment::Component<
      std::shared_ptr<oatpp::data::mapping::ObjectMapper>>(
      oatpp::parser::json::mapping::ObjectMapper::createShared());

  // user_controller
  auto user_controller = std::make_shared<dummy_controller>();

  // doc_endpoints
  auto doc_endpoints = oatpp::swagger::Controller::Endpoints::createShared();

  doc_endpoints->pushBackAll(user_controller->getEndpoints());

  // swagger_controller
//  auto generator_config =
//    oatpp::base::Environment::Component<
//      std::shared_ptr<
//        oatpp::swagger::Generator::Config>>(
//      std::make_shared<oatpp::swagger::Generator::Config>());

  auto swagger_controller = oatpp::swagger::Controller::createShared(doc_endpoints, swaggerDocInfo);
  swagger_controller->addEndpointsToRouter(http_router);

  // server
  auto server = oatpp::network::Server::createShared(server_connection_provider, server_connection_handler);
}

}

int main() {

  // init env
  oatpp::base::Environment::init();

  // run test
  web_service_base_test::test();

  // Assert there is no leaking objects.
  OATPP_ASSERT(oatpp::base::Environment::getObjectsCount() == 0);

  // Destroy env. Should be called after all components are destroyed.
  oatpp::base::Environment::destroy();

  return 0;
}

#include "AppComponent.hpp"

#include "controller/UserController.hpp"
#include "controller/StaticController.hpp"

#include "oatpp-swagger/Controller.hpp"

#include "oatpp/network/Server.hpp"

#include <iostream>

void run() {
  
  AppComponent components; // Create scope Environment components
  
  /* create ApiControllers and add endpoints to router */
  
  auto router = components.httpRouter.getObject();
  auto docEndpoints = oatpp::swagger::Controller::Endpoints::createShared();
  
  auto userController = UserController::createShared();
  userController->addEndpointsToRouter(router);
  
  docEndpoints->pushBackAll(userController->getEndpoints());
  
  auto swaggerController = oatpp::swagger::Controller::createShared(docEndpoints);
  swaggerController->addEndpointsToRouter(router);

  auto staticController = StaticController::createShared();
  staticController->addEndpointsToRouter(router);
  
  /* create server */
  
  oatpp::network::Server server(components.serverConnectionProvider.getObject(),
                                components.serverConnectionHandler.getObject());
  
  OATPP_LOGD("Server", "Running on port %s...", components.serverConnectionProvider.getObject()->getProperty("port").toString()->c_str());
  
  server.run();
  
}

/**
 *  main
 */
int main(int argc, const char * argv[]) {

  oatpp::base::Environment::init();

  run();
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::base::Environment::destroy();
  
  return 0;
}

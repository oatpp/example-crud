
#include "AppComponent.hpp"

#include "controller/UserController.hpp"
#include "controller/StaticController.hpp"

#include "oatpp-swagger/Controller.hpp"

#include "oatpp/network/Server.hpp"

#include <iostream>
#include <fstream>
#include <mutex>

#include "oatpp-mcp/Server.hpp"

class SilentLogger : public oatpp::Logger {
private:
  std::mutex m_mutex;
public:

  void log(v_uint32 priority, const std::string& tag, const std::string& message) override {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ofstream fout(LOG_FILE);
    fout << tag << ": " << message << std::endl;
  }

};

void run() {

  oatpp::mcp::Server mcpServer;
  
  AppComponent components; // Create scope Environment components
  
  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  oatpp::web::server::api::Endpoints docEndpoints;

  auto userController = UserController::createShared();

  docEndpoints.append(router->addController(userController)->getEndpoints());
  mcpServer.addEndpoints(userController->getEndpoints());

  router->addController(oatpp::swagger::Controller::createShared(docEndpoints));
  router->addController(StaticController::createShared());

  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* create server */
  oatpp::network::Server server(connectionProvider,
                                connectionHandler);

  std::thread http([&server, connectionProvider]{
    OATPP_LOGd("Server", "Running on port {}...", connectionProvider->getProperty("port").toString())
    server.run();
  });

  std::thread mcp([&mcpServer]{
    OATPP_LOGd("MCP Server", "Serving via STDIO {}...")
    mcpServer.stdioListen();
  });

  http.join();
  mcp.join();

}

/**
 *  main
 */
int main(int argc, const char * argv[]) {

  oatpp::Environment::init(
    std::make_shared<SilentLogger>()
  );

  run();
  
  /* Print how many objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::Environment::destroy();
  
  return 0;
}


#ifndef CRUD_DATABASECOMPONENT_HPP
#define CRUD_DATABASECOMPONENT_HPP

#include "db/UserDb.hpp"

class DatabaseComponent {
public:
    
  /**
   * Create database connection provider component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::postgresql::Connection>>, dbConnectionProvider)([] {

    /* Create database-specific ConnectionProvider */
    auto connectionProvider = std::make_shared<oatpp::postgresql::ConnectionProvider>("jdbc:postgresql://database:5432/TaskManager");

    /* Create database-specific ConnectionPool */
    return oatpp::postgresql::ConnectionPool::createShared(connectionProvider,
                                                       10 /* max-connections */,
                                                       std::chrono::seconds(5) /* connection TTL */);

  }());

  /**
   * Create database client
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<UserDb>, userDb)([] {

    /* Get database ConnectionProvider component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::postgresql::Connection>>, connectionProvider);

    /* Create database-specific Executor */
    auto executor = std::make_shared<oatpp::postgresql::Executor>(connectionProvider);

    /* Create MyClient database client */
    return std::make_shared<UserDb>(executor);

  }());

};

#endif //CRUD_DATABASECOMPONENT_HPP

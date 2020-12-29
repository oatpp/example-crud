
#ifndef TEST_DATABASECOMPONENT_HPP
#define TEST_DATABASECOMPONENT_HPP

#include "db/UserDb.hpp"

class TestDatabaseComponent {
public:

  /**
   * Create database client
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<UserDb>, userDb)([] {

    /* Create database-specific ConnectionProvider */
    auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>(TESTDATABASE_FILE);

    /* Create database-specific ConnectionPool */
    auto connectionPool = oatpp::sqlite::ConnectionPool::createShared(connectionProvider,
        10 /* max-connections */,
        std::chrono::seconds(5) /* connection TTL */);

    /* Create database-specific Executor */
    auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionPool);

    /* Create MyClient database client */
    return std::make_shared<UserDb>(executor);

  }());


};

#endif //TEST_DATABASECOMPONENT_HPP

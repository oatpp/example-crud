
#ifndef CRUD_USERDB_HPP
#define CRUD_USERDB_HPP

#ifndef DATABASE_MIGRATIONS
#define DATABASE_MIGRATIONS "sql"
#endif

#include "dto/UserDto.hpp"
#include "oatpp-postgresql/orm.hpp"

#include "oatpp/base/Log.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient) //<- Begin Codegen

/**
 * UserDb client definitions.
 */
class UserDb : public oatpp::orm::DbClient {
public:

  UserDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor)
  {

    oatpp::orm::SchemaMigration migration(executor);
    migration.addFile(1 /* start from version 1 */, DATABASE_MIGRATIONS "/001_init.sql");
    migration.addFile(2, DATABASE_MIGRATIONS "/002.sql");
    // TODO - Add more migrations here.
    migration.migrate(); // <-- run migrations. This guy will throw on error.

    auto version = executor->getSchemaVersion();
    OATPP_LOGd("UserDb", "Migration - OK. Version={}.", version);

  }

  QUERY(createUser,
        "INSERT INTO AppUser"
        "(username, email, password) VALUES "
        "(:user.username, :user.email, :user.password)"
        "RETURNING (id);",
        PREPARE(true), // user prepared statement!
        PARAM(oatpp::Object<UserDto>, user))

  QUERY(updateUser,
        "UPDATE AppUser "
        "SET "
        " username=:user.username, "
        " email=:user.email, "
        " password=:user.password, "
        " role=:user.role "
        "WHERE "
        " id=:user.id;",
        PARAM(oatpp::Object<UserDto>, user))

  QUERY(getUserById,
      //   "SELECT id, username, email, role, created_at::text as created_at, updated_at::text as updated_at "
      //   "SELECT id, username, email, role, created_at::timestamp as created_at, updated_at "
        "select * from get_user_by_id(:id) ",
      //   "FROM AppUser WHERE id=1;",
        PREPARE(false), //<-- user prepared statement!
        PARAM(oatpp::Int32, id))

  QUERY(getAllUsers,
        "SELECT * FROM AppUser LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit))

  QUERY(deleteUserById,
        "DELETE FROM AppUser WHERE id=:id;",
        PARAM(oatpp::Int32, id))

};

#include OATPP_CODEGEN_END(DbClient) //<- End Codegen

#endif //CRUD_USERDB_HPP

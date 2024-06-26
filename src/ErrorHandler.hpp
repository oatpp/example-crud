
#ifndef CRUD_ERRORHANDLER_HPP
#define CRUD_ERRORHANDLER_HPP

#include "dto/StatusDto.hpp"

#include "oatpp/web/server/handler/ErrorHandler.hpp"
#include "oatpp/web/protocol/http/outgoing/ResponseFactory.hpp"
#include "oatpp/web/mime/ContentMappers.hpp"

class ErrorHandler : public oatpp::web::server::handler::DefaultErrorHandler {
private:
  typedef oatpp::web::protocol::http::outgoing::Response OutgoingResponse;
  typedef oatpp::web::protocol::http::Status Status;
  typedef oatpp::web::protocol::http::outgoing::ResponseFactory ResponseFactory;
private:
  std::shared_ptr<oatpp::web::mime::ContentMappers> m_mappers;
public:

  ErrorHandler(const std::shared_ptr<oatpp::web::mime::ContentMappers>& mappers);

  std::shared_ptr<OutgoingResponse> renderError(const HttpServerErrorStacktrace& stacktrace) override;

};


#endif //CRUD_ERRORHANDLER_HPP

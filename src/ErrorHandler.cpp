
#include "ErrorHandler.hpp"
#include "oatpp/data/stream/BufferStream.hpp"

ErrorHandler::ErrorHandler(const std::shared_ptr<oatpp::web::mime::ContentMappers>& mappers)
  : m_mappers(mappers)
{}

std::shared_ptr<ErrorHandler::OutgoingResponse> ErrorHandler::renderError(const HttpServerErrorStacktrace& stacktrace) {

  Status status = stacktrace.status;
  if(status.description == nullptr) {
    status.description = "Unknown";
  }

  oatpp::data::stream::BufferOutputStream ss;

  for(auto& s : stacktrace.stack) {
    ss << s << "\n";
  }

  auto error = StatusDto::createShared();
  error->status = "ERROR";
  error->code = stacktrace.status.code;
  error->message = ss.toString();

  std::vector<oatpp::String> accept;
  if(stacktrace.request) {
    accept = stacktrace.request->getHeaderValues("Accept");
  }

  auto mapper = m_mappers->selectMapper(accept);
  if(!mapper) {
    mapper = m_mappers->getDefaultMapper();
  }

  auto response = ResponseFactory::createResponse(stacktrace.status,error,mapper);

  for(const auto& pair : stacktrace.headers.getAll()) {
    response->putHeader(pair.first.toString(), pair.second.toString());
  }

  return response;

}
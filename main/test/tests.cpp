
#include "oatpp-test/UnitTest.hpp"

#include "oatpp/core/concurrency/SpinLock.hpp"
#include "oatpp/core/base/Environment.hpp"

#include "oatpp-swagger/oas3/Model.hpp"

#include <iostream>

namespace {

class Logger : public oatpp::base::Logger {
private:
  oatpp::concurrency::SpinLock::Atom m_atom;
public:

  Logger()
  : m_atom(false)
  {}

  void log(v_int32 priority, const std::string& tag, const std::string& message) override {
    oatpp::concurrency::SpinLock lock(m_atom);
    std::cout << tag << ":" << message << "\n";
  }

};

class Test : public oatpp::test::UnitTest {
public:
  Test() : oatpp::test::UnitTest("MyTag")
  {}

  bool onRun() override {
    return true;
  }
};

void runTests() {

  OATPP_LOGD("test", "insert oatpp-swagger tests here");

  OATPP_RUN_TEST(Test);

}

}

int main() {

  oatpp::base::Environment::init();
  oatpp::base::Environment::setLogger(new Logger());

  runTests();

  oatpp::base::Environment::setLogger(nullptr);
  oatpp::base::Environment::destroy();

  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";

  OATPP_ASSERT(oatpp::base::Environment::getObjectsCount() == 0);

  oatpp::base::Environment::destroy();

  return 0;
}

#ifndef DatabaseTest_hpp
#define DatabaseTest_hpp

#include "oatpp-test/UnitTest.hpp"

class DatabaseTest : public oatpp::test::UnitTest {
public:
    DatabaseTest() : oatpp::test::UnitTest("TEST[DatabaseTest]")
    {}

    void onRun() override;
};

#endif // DatabaseTest_hpp
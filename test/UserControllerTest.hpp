#ifndef UserControllerTest_hpp
#define UserControllerTest_hpp

#include "oatpp-test/UnitTest.hpp"

class UserControllerTest : public oatpp::test::UnitTest {
public:
    UserControllerTest() : oatpp::test::UnitTest("TEST[UserControllerTest]")
    {}

    void onRun() override;
};

#endif // UserControllerTest_hpp

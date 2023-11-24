#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class Input {
public:
    virtual void someMethod(char &c)
    {
        c = '8';
        std::cout << "Say something" << std::endl;
    }

    virtual void someAnotherMethod(char* c)
    {
        c[0] = '8';
        c[1] = '0';
        c[2] = 0;
        std::cout << "Say something" << std::endl;
    }
};

class MockInput : public Input
{
public:
    MOCK_METHOD(void, someMethod, (char& c), (override));
    MOCK_METHOD(void, someAnotherMethod, (char* c), (override));
};

TEST(TestSample, TestInput)
{
    MockInput input;
    EXPECT_CALL(input, someMethod(testing::_)).WillOnce(testing::SetArgReferee<0>('f'));
    char a[10] = "mmm";
    EXPECT_CALL(input, someAnotherMethod(testing::_)).WillOnce(testing::SetArrayArgument<0>(a, a + 3));

    char b = '1';
    input.someMethod(b);
    ASSERT_EQ('f', b);
    char c[5] = "abc";
    input.someAnotherMethod(c);
    ASSERT_EQ('m', c[1]);
}


class BaseApplication
{
public:
    BaseApplication(Input* providedInput = new Input)
    {
        userInputSystem = providedInput;
    };

    ~BaseApplication()
    {
        //delete userInputSystem;
    }
    Input* userInputSystem;

    virtual char anotherMethodCString () {
        char a[10] = "ke";
        userInputSystem->someAnotherMethod(a);
        return a[1];
    }

    virtual char anotherMethod () {
        char a = 'k';
        userInputSystem->someMethod(a);
        return a;
    }
};

class MockBaseApplication : public BaseApplication {
public:
    MockInput* userInputSystem;
    MockBaseApplication(MockInput* providedInput = new MockInput) : BaseApplication(providedInput)
    {
        userInputSystem = providedInput;
    };

    ~MockBaseApplication()
    {
        //delete userInputSystem;
    }
};

class SomeClassFixture: public testing::Test
{
public:
    void SetUp() override
    {
        mockedInstance = new MockBaseApplication();
    };
    void TearDown() override
    {
        delete mockedInstance;
    };
protected:
    MockBaseApplication* mockedInstance;
};

TEST(TestSample, TestMock)
{
    MockInput input;
    EXPECT_CALL(input, someMethod(testing::_)).Times(2).WillRepeatedly(testing::SetArgReferee<0>('f'));
    EXPECT_CALL(input, someAnotherMethod(testing::_)).Times(2).WillRepeatedly(testing::SetArrayArgument<0>(a, a + 3));

    MockBaseApplication mc(&input);
    char d = mc.anotherMethod();
    ASSERT_EQ('f', d);
    char e = mc.anotherMethodCString();
    ASSERT_EQ('m', e);

    char b = '1';
    mc.userInputSystem->someMethod(b);
    ASSERT_EQ('f', b);
    char c[5] = "abc";
    mc.userInputSystem->someAnotherMethod(c);
    ASSERT_EQ('m', c[1]);
}

class Login
{
public:
    BaseApplication* context;
    char a = '1';
    char b = '2';

    explicit Login(BaseApplication* newContext) : context(newContext) {};

    virtual char anotherMethodCString () {
        char a[10] = "ke";
        context->userInputSystem->someAnotherMethod(a);
        return a[1];
    }

    virtual char anotherMethod () {
        char a = 'k';
        context->userInputSystem->someMethod(a);
        return a;
    }

    virtual void run()
    {
        a = anotherMethod();
        b = anotherMethodCString();
    }
};

class MockLogin : public Login
{
public:
    MockBaseApplication* context;
    explicit MockLogin(MockBaseApplication *newContext) : context(newContext), Login(newContext) {};
};

TEST(TestSample, TestMockAnother)
{
    MockInput input;
    MockBaseApplication mockedInstance(&input);
    EXPECT_CALL(*(mockedInstance.userInputSystem), someMethod(testing::_)).WillOnce(testing::SetArgReferee<0>('f'));
    char a[10] = "mmm";
    EXPECT_CALL(*(mockedInstance.userInputSystem), someAnotherMethod(testing::_)).WillOnce(testing::SetArrayArgument<0>(a, a + 3));
    MockLogin loginView(&mockedInstance);

    loginView.run();
    ASSERT_EQ('f', loginView.a);
    ASSERT_EQ('m', loginView.b);
}

int add (int a, int b)
{
    return a+b;
}

TEST(TestSample, TestAddition)
{
    ASSERT_EQ(2, add(1, 1));
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

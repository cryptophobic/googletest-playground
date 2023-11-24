#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class SomeClass
{
public:
    SomeClass() = default;
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

    char anotherMethodCString () {
        char a[10] = "ke";
        someAnotherMethod(a);
        return a[1];
    }

    char anotherMethod () {
        char a = 'k';
        someMethod(a);
        return a;
    }
};

class MockedClass : public SomeClass
{
public:
    MockedClass() = default;

    MOCK_METHOD(void, someMethod, (char& c), (override));
    MOCK_METHOD(void, someAnotherMethod, (char* c), (override));
};

class SomeClassFixture: public testing::Test
{
public:
    void SetUp() override
    {
        mockedClass = new MockedClass();
    };
    void TearDown() override
    {
        delete mockedClass;
    };
protected:
    MockedClass* mockedClass;
};


TEST(TestSample, TestMock)
{
    MockedClass mc;
    EXPECT_CALL(mc, someMethod(testing::_)).WillOnce(testing::SetArgReferee<0>('f'));
    char a[10] = "mmm";
    EXPECT_CALL(mc, someAnotherMethod(testing::_)).WillOnce(testing::SetArrayArgument<0>(a, a + 3));

    //EXPECT_CALL(mc, someMethod()).Times(1);
    char b = mc.anotherMethod();
    ASSERT_EQ('f', b);
    char c = mc.anotherMethodCString();
    ASSERT_EQ('m', c);
}

class SomeAnotherClass
{
public:
    SomeClass* someClass;
    char a = '1';
    char b = '2';

    explicit SomeAnotherClass(SomeClass* cl) : someClass(cl) {};

    virtual void run()
    {
        a = someClass->anotherMethod();
        b = someClass->anotherMethodCString();
    }
};

class MockSomeAnotherClass : public SomeAnotherClass
{
public:
    MockedClass* someClass;
    explicit MockSomeAnotherClass(MockedClass *cl) : someClass(cl), SomeAnotherClass(cl) {};
};

TEST_F(SomeClassFixture, TestMockAnother)
{
    MockSomeAnotherClass cl(mockedClass);
    EXPECT_CALL(*mockedClass, someMethod(testing::_)).WillOnce(testing::SetArgReferee<0>('f'));
    char a[10] = "mmm";
    EXPECT_CALL(*mockedClass, someAnotherMethod(testing::_)).WillOnce(testing::SetArrayArgument<0>(a, a + 3));

    cl.run();
    ASSERT_EQ('f', cl.a);
    ASSERT_EQ('m', cl.b);
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

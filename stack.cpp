#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>

class Base
{
public:
    virtual ~Base() = default;

    virtual int foo()
    {
        std::cout << "WRONG" << std::endl;
        return 3;
    }
    virtual void bar(char& c)
    {
        c = 'a';
        std::cout << "WRONG" << std::endl;
    }
};

class BaseMock: public Base {
public:
    MOCK_METHOD(int, foo, (), (override));
    MOCK_METHOD(void, bar, (char& c), (override));
};

class A {
public:
    A(std::unique_ptr<Base> providedBase = std::make_unique<Base>())  {
        pointer = std::move(providedBase);
    }

    int dosomething()
    {
        return pointer->foo();
    }

    char dosomethingelse()
    {
        char a = 'b';
        pointer->bar(a);
        return a;
    }

private:
    std::unique_ptr<Base> pointer;
};

TEST(A_Test, dosomethingWorksCorrectly)
{
    std::unique_ptr<BaseMock> mck = std::make_unique<BaseMock>();
    //char a = 'l';
    EXPECT_CALL(*(mck.get()), foo()).WillOnce(testing::Return(13));
    EXPECT_CALL(*(mck.get()), bar(testing::_)).WillOnce(testing::SetArgReferee<0>('f'));
    A uut(std::move(mck));

    int b = uut.dosomething();
    char c = uut.dosomethingelse();
    ASSERT_EQ(13, b);
    ASSERT_EQ('f', c);
}

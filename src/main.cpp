#include "mymemory.h"

#include <iostream>
#include <memory>

class Foo
{
public:
    Foo()
    {
        std::cout << "constructor\n";
    }

    Foo(int a)
    {
        std::cout << "constuctor param:" << a << '\n';
    }

    ~Foo()
    {
        std::cout << "destructor\n";
    }

    void foo()
    {
        std::cout << "foo\n";
    }
};

void fooCustomDelete(Foo* foo)
{
    std::cout << "Foo custom delete!\n";
    delete foo;
}

void fooArrCustomDelete(Foo* foo)
{
    std::cout << "Foo arr custom delete!\n";
    delete[] foo;
}

int main()
{
    my::unique_ptr<Foo[], void(*)(Foo*)> qoo(new Foo[15], fooArrCustomDelete);
    my::unique_ptr<Foo, void(*)(Foo*)> woo(new Foo(123), fooCustomDelete);
}
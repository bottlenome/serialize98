#include "serialize98.h"
#include <vector>
#include <fstream>

#define SIZE 4

struct A {
    int b;
    float c;
};

struct D {
    A a;
    int b;
};

struct E {
    int b[10];
    float c;
};

struct F {
    E e[10];
};

SERIALIZE2(A, SERIALIZE_(b), SERIALIZE_(c));
SERIALIZE2(D, SERIALIZE_(a), SERIALIZE_(b));
SERIALIZE2(E, SERIALIZE_A_(b, 10), SERIALIZE_(c));
SERIALIZE1(F, SERIALIZE_A_(e, 10));

int main()
{
    A a;
    a.b = 1;
    a.c = 2.0;

    Node node("", "root");
    node.add(toNodes(a, "a"));
    node.write(std::cout);
   
    A a_a[SIZE];

    Node node2("", "root");
    node2.add(toNodes(a_a, "a", SIZE));
    node2.write(std::cout);

    {
        Serializer s(std::cout);
        s.add(a_a, "a", SIZE);
        s.add(a, "a");
    }

    std::ofstream os("test.txt");
    {
        Serializer s(os);
        s.add(a_a, "a", SIZE);
        s.add(a, "a");
    }
    os.close();

    D d;
    {
        Serializer s(std::cout);
        s.add(d, "d");
    }

    E e;
    {
        Serializer s(std::cout);
        s.add(e, "e");
    }

    F f;
    {
        Serializer s(std::cout);
        s.add(f, "f");
    }
    return 0;
}

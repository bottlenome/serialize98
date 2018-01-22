#include "serialize98.h"
#include <vector>
#include <fstream>

#define SIZE 4

struct A {
    int b;
    float c;
};

SERIALIZE(A, b, c)

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

    return 0;
}

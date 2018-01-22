#include "serialize98.h"
#include <vector>

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
    node.print();
   
    A a_a[SIZE];

    Node node2("", "root");
    node2.add(toNodes(a_a, "a", SIZE));
    node2.print();

    return 0;
}

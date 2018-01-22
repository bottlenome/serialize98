#ifndef SERIALIZER_98_H__
#define SERIALIZER_98_H__

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

template <class T>
class Parameter {
 public:
    std::string name_;
    T value_;
 public:
    Parameter(T &value, std::string name)
    {
        value_ = value;
        name_ = name;
    }
};

class Node {
 public:
    std::string name_;
    std::string value_;
    std::vector<Node> childs_;
 public:
    Node(std::string value, std::string name)
    {
        value_ = value;
        name_ = name;
    }
   
    void add(std::vector<Node> nodes)
    {
        for (int i = 0; i < nodes.size(); i++) {
            childs_.push_back(nodes[i]);
        }
    }

    void print(std::string shift = "")
    {
        std::cout << shift << name_ << ": ";
        if (value_ != "") {
            std::cout << value_;
        }
        std::cout << std::endl;
        for (int i = 0; i < childs_.size(); i++) {
            childs_[i].print(shift + "  ");
        }
    }
};

template<class T>
std::string toString(T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <class T>
std::vector<Node> toNodes(Parameter<T> p);

template <class T>
std::vector<Node> toNodes(T p[], std::string name, size_t size)
{
    std::vector<Node> ret;
    Node node("", name);
    for (int i = 0; i < size; i++) {
        Node child("", "value" + toString(i));
        Parameter<T> param(p[i], "");
        child.add(toNodes(param));
        
        std::vector<Node> inner;
        inner.push_back(child);
        node.add(inner);
    }
    ret.push_back(node);

    return ret;
}

template <class T>
std::vector<Node> toNodes(T &p, std::string name)
{
    std::vector<Node> ret;
    Node node("", name);

    Parameter<T> param(p, name);
    node.add(toNodes(param));

    ret.push_back(node);
    return ret;
}

#define SERIALIZE(Type, P1, P2) \
template<> \
std::vector<Node> toNodes(Parameter<Type> p) \
{ \
    std::vector<Node> ret; \
\
    Node P1(toString(p.value_.P1), #P1); \
    ret.push_back(P1); \
\
    Node P2(toString(p.value_.P2), #P2); \
    ret.push_back(P2); \
\
    return ret; \
}


#endif

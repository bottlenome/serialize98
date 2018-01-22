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
 private:
    std::string name_;
    std::string value_;
    std::vector<Node> childs_;
 public:
    Node(std::string value, std::string name)
    {
        value_ = value;
        name_ = name;
    }

    Node()
    {
        value_ = "";
        name_ = "root";
    }
   
    void add(std::vector<Node> nodes)
    {
        for (int i = 0; i < nodes.size(); i++) {
            childs_.push_back(nodes[i]);
        }
    }

    void write(std::ostream &os, std::string shift = "")
    {
        os << shift << name_ << ": ";
        if (value_ != "") {
            os << value_;
        }
        os << std::endl;
        for (int i = 0; i < childs_.size(); i++) {
            childs_[i].write(os, shift + "  ");
        }
    }
};

class Serializer {
 private:
    std::ostream *os_;
    Node root_;
 public:
    Serializer(std::ostream &os)
    {
        os_ = &os;
    }

    ~Serializer()
    {
        write();
    }

    void write()
    {
        root_.write(*os_);
    }

    template<class T>
    void add(T p[], std::string name, size_t size)
    {
        root_.add(toNodes(p, name, size));
    }

    template<class T>
    void add(T &p, std::string name)
    {
        root_.add(toNodes(p, name));
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

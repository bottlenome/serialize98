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

    bool hasValue()
    {
        return value_ != "";
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
    return "";
}

#define SPECIALIZE_TO_STRING(Type) \
template<> \
std::string toString(Type value) \
{ \
    std::ostringstream oss; \
    oss << value; \
    return oss.str(); \
}

SPECIALIZE_TO_STRING(char);
SPECIALIZE_TO_STRING(short);
SPECIALIZE_TO_STRING(int);
SPECIALIZE_TO_STRING(long);
SPECIALIZE_TO_STRING(unsigned char);
SPECIALIZE_TO_STRING(unsigned short);
SPECIALIZE_TO_STRING(unsigned int);
SPECIALIZE_TO_STRING(unsigned long);
SPECIALIZE_TO_STRING(float);
SPECIALIZE_TO_STRING(double);

template <class T>
std::vector<Node> toNodes(Parameter<T> p);

#define SPECIALIZE_PRIMITIVE_TO_NODES(Type) \
template <> \
std::vector<Node> toNodes(Parameter<Type> p) \
{ \
    std::vector<Node> ret; \
    Node node(toString(p.value_), p.name_); \
    ret.push_back(node); \
    return ret; \
}

SPECIALIZE_PRIMITIVE_TO_NODES(char);
SPECIALIZE_PRIMITIVE_TO_NODES(short);
SPECIALIZE_PRIMITIVE_TO_NODES(int);
SPECIALIZE_PRIMITIVE_TO_NODES(long);
SPECIALIZE_PRIMITIVE_TO_NODES(unsigned char);
SPECIALIZE_PRIMITIVE_TO_NODES(unsigned short);
SPECIALIZE_PRIMITIVE_TO_NODES(unsigned int);
SPECIALIZE_PRIMITIVE_TO_NODES(unsigned long);
SPECIALIZE_PRIMITIVE_TO_NODES(float);
SPECIALIZE_PRIMITIVE_TO_NODES(double);

template <class T>
std::vector<Node> toNodes(T p[], std::string name, size_t size)
{
    std::vector<Node> ret;
    Node node("", name);
    for (int i = 0; i < size; i++) {
        Node child(toString(p[i]), "value" + toString(i));
        if (!child.hasValue()) { // not primitive type
            Parameter<T> param(p[i], "");
            child.add(toNodes(param));
        }
        
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

#define SPECIALIZE_TO_NODES(Type) \
template <> \
std::vector<Node> toNodes(Type &p, std::string name) \
{ \
    std::vector<Node> ret; \
    return ret; \
}

SPECIALIZE_TO_NODES(char)
SPECIALIZE_TO_NODES(short)
SPECIALIZE_TO_NODES(int)
SPECIALIZE_TO_NODES(long)
SPECIALIZE_TO_NODES(unsigned char)
SPECIALIZE_TO_NODES(unsigned short)
SPECIALIZE_TO_NODES(unsigned int)
SPECIALIZE_TO_NODES(unsigned long)
SPECIALIZE_TO_NODES(float)
SPECIALIZE_TO_NODES(double)

#define SERIALIZE_(P) \
    Node P(toString(p.value_.P), #P); \
    if (!P.hasValue()) \
    { \
        std::vector<Node> tmp = toNodes(p.value_.P, #P); \
        for (int i = 0; i < tmp.size(); i++) { \
            ret.push_back(tmp[i]); \
        } \
    } else { \
        ret.push_back(P); \
    }

#define SERIALIZE(Type, P1, P2) \
template<> \
std::vector<Node> toNodes(Parameter<Type> p) \
{ \
    std::vector<Node> ret; \
\
    SERIALIZE_(P1) \
    SERIALIZE_(P2) \
\
    return ret; \
}

#define SERIALIZE_A_(P, S) \
    std::vector<Node> tmp = toNodes(p.value_.P, #P, S); \
    for (int i = 0; i < tmp.size(); i++) { \
        ret.push_back(tmp[i]); \
    }

#define SERIALIZE_A(Type, P1, P2, S1) \
template<> \
std::vector<Node> toNodes(Parameter<Type> p) \
{ \
    std::vector<Node> ret; \
\
    SERIALIZE_A_(P1, S1) \
    SERIALIZE_(P2) \
\
    return ret; \
}

#endif

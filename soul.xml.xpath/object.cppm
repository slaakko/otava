// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.xpath.object;

import std;
import soul.xml.node;
import soul.xml.element;

export namespace soul::xml::xpath {

enum class ObjectKind 
{
    nodeSet, boolean, number, string
};

std::string ObjectKindStr(ObjectKind kind);

class Object
{
public:
    Object(ObjectKind kind_);
    virtual ~Object();
    inline ObjectKind Kind() const noexcept { return kind; }
    inline bool IsNodeSet() const noexcept { return kind == ObjectKind::nodeSet; }
    inline bool IsBoolean() const noexcept { return kind == ObjectKind::boolean; }
    inline bool IsNumber() const noexcept { return kind == ObjectKind::number; }
    inline bool IsString() const noexcept { return kind == ObjectKind::string; }
    virtual soul::xml::Element* ToXmlElement() const = 0;
private:
    ObjectKind kind;
};

class NodeSet : public Object
{
public:
    NodeSet();
    inline const std::vector<soul::xml::Node*>& Nodes() const noexcept { return nodes; }
    inline int Count() const noexcept { return nodes.size(); }
    void Add(soul::xml::Node* node);
    Node* GetNode(int index) const;
    soul::xml::Element* ToXmlElement() const override;
private:
    std::vector<soul::xml::Node*> nodes;
};

class Boolean : public Object
{
public:
    Boolean() noexcept;
    Boolean(bool value_) noexcept;
    inline bool Value() const noexcept { return value; }
    soul::xml::Element* ToXmlElement() const override;
private:
    bool value;
};

class Number : public Object
{
public:
    Number() noexcept;
    Number(double value_) noexcept;
    inline double Value() const noexcept { return value; }
    soul::xml::Element* ToXmlElement() const override;
private:
    double value;
};

class String : public Object
{
public:
    String() noexcept;
    String(const std::string& value_);
    inline const std::string& Value() const noexcept { return value; }
    soul::xml::Element* ToXmlElement() const override;
private:
    std::string value;
};

} // namespace soul::xml::xpath

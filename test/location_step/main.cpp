import std;

class Object
{
public:
    Object();
private:
    std::string s;
};

Object::Object() : s("bar")
{
}

class NodeSet : public Object
{
public:
    NodeSet();
private:
    std::string s;
};

NodeSet::NodeSet() : s("foo") 
{
}

std::unique_ptr<Object> foo()
{
    std::unique_ptr<NodeSet> ns(new NodeSet());
    return std::unique_ptr<Object>(ns.release());
}

int main()
{
    std::unique_ptr<Object> o = foo();
}

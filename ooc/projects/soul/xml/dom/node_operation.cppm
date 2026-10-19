export module soul.xml.node_operation;

import std;
import soul.xml.node;

export namespace soul::xml {

class NodeOperation
{
public:
    virtual ~NodeOperation();
    virtual void Apply(Node* node) = 0;
};

} // namespace soul::xml

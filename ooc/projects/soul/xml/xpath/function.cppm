export module soul.xml.xpath.function;

import std;
import soul.xml.node;
import soul.xml.xpath.object;
import soul.xml.xpath.context;

export namespace soul::xml::xpath {

std::string StringValue(soul::xml::Node* node);

enum class FunctionKind : int
{
    boolean, number, string, last, position, count, max
};

std::string FunctionName(FunctionKind kind);

class Function
{
public:
    Function(FunctionKind kind_);
    Function(FunctionKind kind_, int minArity_, int maxArity_);
    virtual ~Function();
    inline FunctionKind Kind() const noexcept { return kind; }
    inline const std::string& Name() const noexcept { return name; }
    inline int MinArity() const noexcept { return minArity; }
    inline int MaxArity() const noexcept { return maxArity; }
    std::unique_ptr<Object> Evaluate(Context& context, const std::vector<Object*>& arguments);
    std::string ArityStr() const;
protected:
    virtual std::unique_ptr<Object> DoEvaluate(Context& context, const std::vector<Object*>& arguments) = 0;
private:
    FunctionKind kind;
    std::string name;
    int minArity;
    int maxArity;
};

Function* GetFunction(FunctionKind kind);
Function* GetFunction(const std::string& name);

} // namespace soul::xml::xpath

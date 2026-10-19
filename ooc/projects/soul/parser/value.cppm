export module soul.parser.value;

import std;

export namespace soul::parser {

template<class T>
struct Value
{
    Value(const T& value_) : value(value_) {}
    T value;
};

} // namespace soul::parser

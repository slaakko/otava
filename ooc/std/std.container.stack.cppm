export module std.container.stack;

import std.type.fundamental;

export namespace std {

template<typename T, typename Container = vector<T>>
class stack
{
public:
    using value_type = typename Container::value_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type = typename Container::size_type;
    using container_type = Container;
protected:
    Container c;
public: 
    stack() noexcept : c()
    {
    }
    explicit stack(const Container& c_) : c(c_)
    {
    }
    explicit stack(Container&& that) noexcept : c(std::move(that.c))
    {
    }
    inline bool empty() const noexcept { return c.empty(); }
    inline size_type size() const noexcept { return c.size(); }
    inline reference top() noexcept { return c.back(); }
    inline const_reference top() const noexcept { return c.back(); }
    inline void push(const value_type& x) { c.push_back(x); }
    inline void push(value_type&& x) { c.push_back(std::move(x)); }
    inline void pop() { c.pop_back(); }
};

} // namespace std

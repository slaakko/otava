export module std.container.map;

import std.type.fundamental;
import std.functional;
import std.utilities.pair;
import std.container.rb_tree;

export namespace std {

template<class Key, class T, class Compare = less<Key>>
class map
{
public:
    using key_type = Key;
    using const_key_type = const Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using key_compare = Compare;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = int64_t;
    using difference_type = int64_t;
    using tree_type = std::rb_tree<key_type, value_type, std::select_first<const_key_type, mapped_type>, key_compare>;
    using iterator = tree_type::iterator;
    using const_iterator = tree_type::const_iterator;
    
    map() : tree() {}
    map(const map& x) : tree(x.tree) {}
    map(map&& x) noexcept : tree(std::move(x.tree)) {}
    map& operator=(const map& x) { tree = x.tree; return *this; }
    map& operator=(map&& x) noexcept { std::swap(tree, x.tree); return *this; }
    
    inline iterator begin() noexcept { return tree.begin(); }
    inline const_iterator begin() const noexcept { return tree.cbegin(); }
    inline iterator end() noexcept { return tree.end(); }
    inline const_iterator end() const noexcept { return tree.cend(); }
    inline const_iterator cbegin() const noexcept { return tree.cbegin(); }
    inline const_iterator cend() const noexcept { return tree.cend(); }
    
    inline bool empty() const noexcept { return tree.empty(); }
    inline size_type size() const noexcept { return tree.size(); }
    inline const key_compare& key_comp() const noexcept { return tree.cmp(); }
    
    mapped_type& operator[](const key_type& x)
    {
        key_type xc = x;
        value_type value(std::move(xc), std::move(mapped_type()));
        std::pair<iterator, bool> ib = insert(std::move(value));
        iterator it = ib.first;
        return it->second;
    }
    mapped_type& operator[](key_type&& x)
    {
        value_type value(std::move(x), std::move(mapped_type()));
        std::pair<iterator, bool> ib = insert(std::move(value));
        iterator it = ib.first;
        return it->second;
    }
    inline pair<iterator, bool> insert(const value_type& x)
    {
        return tree.insert(x);
    }
    inline pair<iterator, bool> insert(value_type&& x)
    {
        return tree.insert(std::move(x));
    }
    inline iterator erase(iterator position)
    {
        return tree.erase(position);
    }
    inline iterator erase(const_iterator position)
    {
        return tree.erase(position);
    }
    size_type erase(const key_type& x)
    {
        iterator n = find(x);
        if (n != end())
        {
            erase(n);
            return 1;
        }
        return 0;
    }
    iterator erase(iterator first, iterator last)
    {
        iterator r = end();
        while (first != last)
        {
            iterator n = next(first);
            r = erase(first);
            first = n;
        }
        return r;
    }
    iterator erase(const_iterator first, const_iterator last)
    {
        iterator r = end();
        while (first != last)
        {
            const_iterator n = next(first);
            r = erase(first);
            first = n;
        }
        return r;
    }
    inline void clear() noexcept { tree.clear(); }
    inline key_compare key_comp() const noexcept { return tree.cmp(); }
    
    inline iterator find(const key_type& x) noexcept { return tree.find(x); }
    inline const_iterator find(const key_type& x) const noexcept { return tree.find(x); }
    inline size_type count(const key_type& x) const noexcept
    {
        if (contains(x)) return 1;
        return 0;
    }
    inline bool contains(const key_type& x) const noexcept
    {
        const_iterator i = find(x);
        if (i != cend()) return true;
        return false;
    }
    inline iterator lower_bound(const key_type& x) noexcept { return tree.lower_bound(x); }
    inline const_iterator lower_bound(const key_type& x) const noexcept { return tree.lower_bound(x); }
    inline iterator upper_bound(const key_type& x) noexcept { return tree.upper_bound(x); }
    inline const_iterator upper_bound(const key_type& x) const noexcept { return tree.upper_bound(x); }
private:
    tree_type tree;
};

template<class Key, class T, class Compare>
bool operator==(const map<Key, T, Compare>& x, const map<Key, T, Compare>& y) noexcept
{
    return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
}

template<class Key, class T, class Compare>
bool operator<(const map<Key, T, Compare>& x, const map<Key, T, Compare>& y) noexcept
{
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

} // namespace std

export module std.container.rb_tree;

import std.type.fundamental;
import std.iterator;
import std.utilities.pair;
import std.utilities.utility;
import std.utilities.unique_ptr;
import std.new_delete_op;

export namespace std {

enum class rb_node_color
{
    red, black
};

class rb_node_base
{
public:
    explicit rb_node_base(rb_node_base* parent_) noexcept;
    virtual ~rb_node_base();
    inline rb_node_color get_color() const noexcept { return color; }
    inline void set_color(rb_node_color color_) noexcept { color = color_; }
    inline rb_node_base* get_parent() const noexcept { return parent; }
    inline rb_node_base** parent_ptr() const noexcept { return &parent; }
    inline void set_parent(rb_node_base* parent_) noexcept { parent = parent_; }
    inline rb_node_base* get_left() const noexcept { return left; }
    inline rb_node_base** left_ptr() noexcept { return &left; }
    inline void set_left(rb_node_base* left_) noexcept { left = left_; }
    inline rb_node_base* get_right() const noexcept { return right; }
    inline rb_node_base** right_ptr() noexcept { return &right; }
    inline void set_right(rb_node_base* right_) noexcept { right = right_; }
    bool is_header_node() const noexcept;
private:
    rb_node_base* parent;
    rb_node_base* left;
    rb_node_base* right;
    rb_node_color color;
};

rb_node_base* min(rb_node_base* n) noexcept;
rb_node_base* max(rb_node_base* n) noexcept;
rb_node_base* get_prev(rb_node_base* n) noexcept;
rb_node_base* get_next(rb_node_base* n) noexcept;
void rotate_left(rb_node_base* n, rb_node_base** root_ptr) noexcept;
void rotate_right(rb_node_base* n, rb_node_base** root_ptr) noexcept;
void rebalance_after_insert(rb_node_base* n, rb_node_base** root_ptr) noexcept;
rb_node_base* rebalance_for_remove(rb_node_base* z, rb_node_base** root_ptr, rb_node_base** leftmost_ptr, rb_node_base** rightmost_ptr) noexcept;

template<typename T>
class rb_node : public rb_node_base
{
public:
    using value_type = T;

    rb_node(const value_type& value_, rb_node_base* parent_) : rb_node_base(parent_), val(value_) {}
    rb_node(value_type&& value_, rb_node_base* parent_) noexcept : rb_node_base(parent_), val(std::move(value_)) {}
    inline const value_type& value() const noexcept { return val; }
    inline value_type& value() noexcept { return val; }
    inline value_type* value_ptr() noexcept { return &val; }
private:
    value_type val;
};

template<typename T, typename R, typename P>
class rb_node_iterator
{
public:
    using difference_type = int64_t;
    using value_type = T;
    using pointer = P;
    using reference = R;
    using iterator_category = bidirectional_iterator_tag;
    using node_type = rb_node<value_type>;

    inline rb_node_iterator() noexcept : n(nullptr) {}
    inline rb_node_iterator(node_type* node_) noexcept : n(node_) {}
    inline reference operator*() noexcept { return n->value(); }
    inline pointer operator->() noexcept { return n->value_ptr(); }
    inline rb_node_iterator& operator++() noexcept { n = static_cast<node_type*>(get_next(n)); return *this; }
    inline rb_node_iterator operator++(int) noexcept { rb_node_iterator p = *this; n = static_cast<node_type*>(get_next(n)); return p; }
    inline rb_node_iterator& operator--() noexcept { n = static_cast<node_type*>(get_prev(n)); return *this; }
    inline rb_node_iterator operator--(int) noexcept { rb_node_iterator p = *this; n = static_cast<node_type*>(get_prev(n)); return p; }
    inline node_type* node() const noexcept { return n; }
private:
    node_type* n;
};

template<typename T, typename R, typename P>
inline bool operator==(const rb_node_iterator<T, R, P>& left, const rb_node_iterator<T, R, P>& right) noexcept
{
    return left.node() == right.node();
}

template<typename KeyType, typename ValueType, typename KeyOfValue, typename Compare>
class rb_tree
{
public:
    using key_type = KeyType;
    using value_type = ValueType;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using node_type = rb_node<value_type>;
    using size_type = int64_t;
    using difference_type = int64_t;
    using key_of_value = KeyOfValue;
    using compare = Compare;
    using iterator = rb_node_iterator<value_type, value_type&, value_type*>;
    using const_iterator = rb_node_iterator<value_type, const value_type&, const value_type*>;

    rb_tree() : header(), sz(0), key_of(), comp()
    {
        init();
    }
    ~rb_tree()
    {
        clear();
    }
    rb_tree(const rb_tree& that) : header(), sz(0), key_of(), comp()
    {
        init();
        copy_from(that);
    }
    rb_tree(rb_tree&& that) noexcept : header(std::move(that.header)), sz(that.sz), key_of(that.key_of), comp(that.comp)
    {
        that.sz = 0;
    }
    rb_tree& operator=(const rb_tree& that)
    {
        clear();
        init();
        copy_from(that);
        return *this;
    }
    rb_tree& operator=(rb_tree&& that) noexcept
    {
        std::swap(header, that.header);
        std::swap(sz, that.sz);
        std::swap(key_of, that.key_of);
        std::swap(comp, that.comp);
        return *this;
    }
    void clear() noexcept
    {
        if (!header)
        {
            return;
        }
        node_type* r = root();
        if (r)
        {
            delete r;
            set_root(static_cast<node_type*>(nullptr));
        }
        set_leftmost(header.get());
        set_rightmost(header.get());
        sz = 0;
    }
    inline bool empty() const noexcept { return sz == 0; }
    inline size_type size() const noexcept { return sz; }
    inline iterator begin() noexcept
    {
        if (!header) return end();
        return iterator(leftmost());
    }
    inline const_iterator begin() const noexcept
    {
        if (!header) return cend();
        return const_iterator(leftmost());
    }
    inline const_iterator cbegin() const noexcept
    {
        if (!header) return cend();
        return const_iterator(leftmost());
    }
    inline iterator end() noexcept
    {
        return iterator(header.get());
    }
    inline const_iterator end() const noexcept
    {
        return const_iterator(header.get());
    }
    inline const_iterator cend() const noexcept
    {
        return const_iterator(header.get());
    }
    inline const compare& cmp() noexcept { return comp; }
    std::pair<iterator, bool> insert(const value_type& value)
    {
        if (!header)
        {
            init();
        }
        node_type* x = root();
        node_type* p = header.get();
        bool c = true;
        while (x)
        {
            p = x;
            c = comp(key_of(value), key_of(x->value()));
            if (c)
            {
                x = static_cast<node_type*>(x->get_left());
            }
            else
            {
                x = static_cast<node_type*>(x->get_right());
            }
        }
        iterator j = p;
        if (c)
        {
            if (j == begin())
            {
                iterator it = insert(x, p, value);
                return std::make_pair(it, true);
            }
            else
            {
                --j;
            }
        }
        if (comp(key_of(j.node()->value()), key_of(value)))
        {
            iterator it = insert(x, p, value);
            return std::make_pair(it, true);
        }
        return std::make_pair(j, false);
    }
    std::pair<iterator, bool> insert(value_type&& value)
    {
        if (!header)
        {
            init();
        }
        node_type* x = root();
        node_type* p = header.get();
        bool c = true;
        while (x)
        {
            p = x;
            c = comp(key_of(value), key_of(x->value()));
            if (c)
            {
                x = static_cast<node_type*>(x->get_left());
            }
            else
            {
                x = static_cast<node_type*>(x->get_right());
            }
        }
        iterator j = p;
        if (c)
        {
            if (j == begin())
            {
                iterator it = insert(x, p, std::move(value));
                return std::make_pair(it, true);
            }
            else
            {
                --j;
            }
        }
        if (comp(key_of(j.node()->value()), key_of(value)))
        {
            iterator it = insert(x, p, std::move(value));
            return std::make_pair(it, true);
        }
        return std::make_pair(j, false);
    }
    iterator find(const key_type& key) noexcept
    {
        if (empty()) return end();
        node_type* y = header.get();
        node_type* x = root();
        while (x)
        {
            if (!comp(key_of(x->value()), key))
            {
                y = x; 
                x = static_cast<node_type*>(x->get_left());
            }
            else
            {
                x = static_cast<node_type*>(x->get_right());
            }
        }
        iterator j = iterator(y);
        if (j == end() || comp(key, key_of(j.node()->value())))
        {
            return end();
        }
        else
        {
            return j;
        }
    }
    const_iterator find(const key_type& key) const noexcept
    {
        if (empty()) return cend();
        node_type* y = header.get();
        node_type* x = root();
        while (x)
        {
            if (!comp(key_of(x->value()), key))
            {
                y = x;
                x = static_cast<node_type*>(x->get_left());
            }
            else
            {
                x = static_cast<node_type*>(x->get_right());
            }
        }
        const_iterator j = const_iterator(y);
        if (j == cend() || comp(key, key_of(j.node()->value())))
        {
            return cend();
        }
        else
        {
            return j;
        }
    }
    iterator lower_bound(const key_type& key) noexcept
    {
        if (empty()) return end();
        node_type* y = header.get();
        node_type* x = root();
        while (x)
        {
            if (!comp(key_of(x->value()), key))
            {
                y = x;
                x = static_cast<node_type*>(x->get_left());
            }
            else
            {
                x = static_cast<node_type*>(x->get_right());
            }
        }
        return iterator(y);
    }
    const_iterator lower_bound(const key_type& key) const noexcept
    {
        if (empty()) return end();
        node_type* y = header.get();
        node_type* x = root();
        while (x)
        {
            if (!comp(key_of(x->value()), key))
            {
                y = x;
                x = static_cast<node_type*>(x->get_left());
            }
            else
            {
                x = static_cast<node_type*>(x->get_right());
            }
        }
        return const_iterator(y);
    }
    inline bool keys_equal(const key_type& k1, const key_type& k2) noexcept
    {
        return !comp(k1, k2) && !comp(k2, k1);
    }
    iterator upper_bound(const key_type& key) noexcept
    {
        iterator f = find(key);
        while (f != end() && keys_equal(key, key_of(f.node()->value())))
        {
            ++f;
        }
        return f;
    }
    const_iterator upper_bound(const key_type& key) const noexcept
    {
        const_iterator f = find(key);
        while (f != cend() && keys_equal(key, key_of(f.node()->value())))
        {
            ++f;
        }
        return f;
    }
    iterator erase(iterator position)
    {
        iterator n = next(position);
        node_type* r = static_cast<node_type*>(rebalance_for_remove(position.node(), root_ptr(), leftmost_ptr(), rightmost_ptr()));
        r->set_left(nullptr);
        r->set_right(nullptr);
        delete r;
        --sz;
        return n;
    }
    iterator erase(const_iterator position)
    {
        const_iterator n = next(position);
        node_type* r = static_cast<node_type*>(rebalance_for_remove(position.node(), root_ptr(), leftmost_ptr(), rightmost_ptr()));
        r->set_left(nullptr);
        r->set_right(nullptr);
        delete r;
        --sz;
        return iterator(n.node());
    }
    inline node_type* root() noexcept
    {
        return static_cast<node_type*>(header->get_parent());
    }
private:
    void init()
    {
        header.reset(new rb_node<value_type>(value_type(), nullptr));
        header->set_color(rb_node_color::red);
        set_leftmost(header.get());
        set_rightmost(header.get());
    }
    inline void set_root(node_type* r) noexcept
    {
        header->set_parent(r);
    }
    inline rb_node_base** root_ptr() noexcept
    {
        return header->parent_ptr();
    }
    inline node_type* leftmost() noexcept
    {
        return static_cast<node_type*>(header->get_left());
    }
    inline void set_leftmost(node_type* lm) noexcept
    {
        header->set_left(lm);
    }
    inline rb_node_base** leftmost_ptr() noexcept
    {
        return header->left_ptr();
    }
    inline node_type* rightmost() noexcept
    {
        return static_cast<node_type*>(header->get_right());
    }
    inline void set_rightmost(node_type* rm) noexcept
    {
        header->set_right(rm);
    }
    inline rb_node_base** rightmost_ptr() noexcept
    {
        return header->right_ptr();
    }
    iterator insert(node_type* x, node_type* p, const value_type& value)
    {
        if (!header)
        {
            init();
        }
        node_type* n = new node_type(value, p);
        if (p == header.get() || x != nullptr || comp(key_of(value), key_of(p->value())))
        {
            p->set_left(n);
            if (p == header.get())
            {
                set_root(n);
                set_rightmost(n);
            }
            else if (p == leftmost())
            {
                set_leftmost(n);
            }
        }
        else
        {
            p->set_right(n);
            if (p == rightmost())
            {
                set_rightmost(n);
            }
        }
        rebalance_after_insert(n, root_ptr());
        ++sz;
        return iterator(n);
    }
    iterator insert(node_type* x, node_type* p, value_type&& value)
    {
        if (!header)
        {
            init();
        }
        node_type* n = new node_type(std::move(value), p);
        if (p == header.get() || x != nullptr || comp(key_of(n->value()), key_of(p->value())))
        {
            p->set_left(n);
            if (p == header.get())
            {
                set_root(n);
                set_rightmost(n);
            }
            else if (p == leftmost())
            {
                set_leftmost(n);
            }
        }
        else
        {
            p->set_right(n);
            if (p == rightmost())
            {
                set_rightmost(n);
            }
        }
        rebalance_after_insert(n, root_ptr());
        ++sz;
        return iterator(n);
    }
    void copy_from(const rb_tree& that)
    {
        if (that.root())
        {
            set_root(copy(that.root(), header.get()));
            set_leftmost(static_cast<node_type*>(min(root())));
            set_rightmost(static_cast<node_type*>(max(root())));
            sz = that.sz;
        }
    }
    node_type* copy(node_type* x, node_type* p)
    {
        node_type* top = clone_node(x, p);
        if (x->get_right())
        {
            top->set_right(copy(static_cast<node_type*>(x->get_right()), top));
        }
        p = top;
        x = static_cast<node_type*>(x->get_left());
        while (x)
        {
            node_type* y = clone_node(x, p);
            p->set_left(y);
            if (x->get_right())
            {
                y->set_right(copy(static_cast<node_type*>(x->get_right()), y));
            }
            p = y;
            x = static_cast<node_type*>(x->get_left());
        }
        return top;
    }
    node_type* clone_node(node_type* x, node_type* p)
    {
        node_type* clone = new node_type(x->value(), p);
        clone->set_color(x->get_color());
        return clone;
    }
    std::unique_ptr<node_type> header;
    size_type sz;
    key_of_value key_of;
    compare comp;
};

} // namespace std

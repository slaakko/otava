module std.container.rb_tree;

import std.stream;

namespace std {

rb_node_base::rb_node_base(rb_node_base* parent_) : parent(parent_), left(nullptr), right(nullptr), color(rb_node_color::black)
{
}

rb_node_base::~rb_node_base()
{
#ifdef DEBUG_RB_TREE
    std::cout << ">delete " << static_cast<std::uint64_t>(static_cast<void*>(this)) << "\n";
#endif
    if (left && left != this)
    {
#ifdef DEBUG_RB_TREE
        std::cout << "delete left " << static_cast<std::uint64_t>(static_cast<void*>(left)) << "\n";
#endif
        delete left;
    }
    if (right && right != this)
    {
#ifdef DEBUG_RB_TREE
        std::cout << "delete right " << static_cast<std::uint64_t>(static_cast<void*>(right)) << "\n";
#endif
        delete right;
    }
#ifdef DEBUG_RB_TREE
    std::cout << "<delete " << static_cast<std::uint64_t>(static_cast<void*>(this)) << "\n";
#endif
}
    
bool rb_node_base::is_header_node() const
{
    return color == rb_node_color::red && parent && parent->get_parent() == this;
}

rb_node_base* min(rb_node_base* n)
{
    rb_node_base* left = n->get_left();
    while (left)
    {
        n = left;
        left = n->get_left();
    }
    return n;
}

rb_node_base* max(rb_node_base* n)
{
    rb_node_base* right = n->get_right();
    while (right)
    {
        n = right;
        right = n->get_right();
    }
    return n;
}

rb_node_base* get_prev(rb_node_base* n)
{
    if (n->is_header_node())
    {
        return n->get_right();
    }
    else
    {
        rb_node_base* left = n->get_left();
        if (left)
        {
            return max(left);
        }
        else
        {
            rb_node_base* u = n->get_parent();
            rb_node_base* left = u->get_left();
            while (n == left)
            {
                n = u;
                u = u->get_parent();
                left = u->get_left();
            }
            return u;
        }
    }
}

rb_node_base* get_next(rb_node_base* n)
{
    rb_node_base* right = n->get_right();
    if (right)
    {
        return min(right);
    }
    else
    {
        rb_node_base* u = n->get_parent();
        rb_node_base* right = u->get_right();
        while (n == right)
        {
            n = u;
            u = u->get_parent();
            right = u->get_right();
        }
        if (n->get_right() != u)
        {
            return u;
        }
        return n;
    }
}

void rotate_left(rb_node_base* n, rb_node_base** root_ptr)
{
    rb_node_base* u = n->get_right();
    n->set_right(u->get_left());
    if (u->get_left())
    {
        u->get_left()->set_parent(n);
    }
    u->set_parent(n->get_parent());
    if (n == *root_ptr)
    {
        *root_ptr = u;
    }
    else if (n == n->get_parent()->get_left())
    {
        n->get_parent()->set_left(u);
    }
    else
    {
        n->get_parent()->set_right(u);
    }
    u->set_left(n);
    n->set_parent(u);
}

void rotate_right(rb_node_base* n, rb_node_base** root_ptr)
{
    rb_node_base* u = n->get_left();
    n->set_left(u->get_right());
    if (u->get_right())
    {
        u->get_right()->set_parent(n);
    }
    u->set_parent(n->get_parent());
    if (n == *root_ptr)
    {
        *root_ptr = u;
    }
    else if (n == n->get_parent()->get_right())
    {
        n->get_parent()->set_right(u);
    }
    else
    {
        n->get_parent()->set_left(u);
    }
    u->set_right(n);
    n->set_parent(u);
}

void rebalance_after_insert(rb_node_base* n, rb_node_base** root_ptr)
{
    n->set_color(rb_node_color::red);
    while (n != *root_ptr && n->get_parent()->get_color() == rb_node_color::red)
    {
        if (n->get_parent() == n->get_parent()->get_parent()->get_left())
        {
            rb_node_base* u = n->get_parent()->get_parent()->get_right();
            if (u && u->get_color() == rb_node_color::red)
            {
                n->get_parent()->set_color(rb_node_color::black);
                u->set_color(rb_node_color::black);
                n->get_parent()->get_parent()->set_color(rb_node_color::red);
                n = n->get_parent()->get_parent();
            }
            else
            {
                if (n == n->get_parent()->get_right())
                {
                    n = n->get_parent();
                    rotate_left(n, root_ptr);
                }
                n->get_parent()->set_color(rb_node_color::black);
                n->get_parent()->get_parent()->set_color(rb_node_color::red);
                rotate_right(n->get_parent()->get_parent(), root_ptr);
            }
        }
        else
        {
            rb_node_base* u = n->get_parent()->get_parent()->get_left();
            if (u && u->get_color() == rb_node_color::red)
            {
                n->get_parent()->set_color(rb_node_color::black);
                u->set_color(rb_node_color::black);
                n->get_parent()->get_parent()->set_color(rb_node_color::red);
                n = n->get_parent()->get_parent();
            }
            else
            {
                if (n == n->get_parent()->get_left())
                {
                    n = n->get_parent();
                    rotate_right(n, root_ptr);
                }
                n->get_parent()->set_color(rb_node_color::black);
                n->get_parent()->get_parent()->set_color(rb_node_color::red);
                rotate_left(n->get_parent()->get_parent(), root_ptr);
            }
        }
    }
    rb_node_base* root = *root_ptr;
    root->set_color(rb_node_color::black);
}

rb_node_base* rebalance_for_remove(rb_node_base* z, rb_node_base** root_ptr, rb_node_base** leftmost_ptr, rb_node_base** rightmost_ptr)
{
    rb_node_base* y = z;
    rb_node_base* x = nullptr;
    rb_node_base* p = nullptr;
    if (!y->get_left())
    {
        x = y->get_right();
    }
    else
    {
        if (!y->get_right())
        {
            x = y->get_left();
        }
        else
        {
            y = y->get_right();
            while (y->get_left())
            {
                y = y->get_left();
            }
            x = y->get_right();
        }
    }
    if (y != z)
    {
        z->get_left()->set_parent(y);
        y->set_left(z->get_left());
        if (y != z->get_right())
        {
            p = y->get_parent();
            if (x)
            {
                x->set_parent(y->get_parent());
            }
            y->get_parent()->set_left(x);
            y->set_right(z->get_right());
            z->get_right()->set_parent(y);
        }
        else
        {
            p = y;
        }
        if (*root_ptr == z)
        {
            *root_ptr = y;
        }
        else 
        {
            if (z->get_parent()->get_left() == z)
            {
                z->get_parent()->set_left(y);
            }
            else
            {
                z->get_parent()->set_right(y);
            }
        }
        y->set_parent(z->get_parent());
        rb_node_color c = y->get_color();
        y->set_color(z->get_color());
        z->set_color(c);
        y = z;
    }
    else
    {
        p = y->get_parent();
        if (x)
        {
            x->set_parent(y->get_parent());
        }
        if (*root_ptr == z)
        {
            *root_ptr = x;
        }
        else
        {
            if (z->get_parent()->get_left() == z)
            {
                z->get_parent()->set_left(x);
            }
            else
            {
                z->get_parent()->set_right(x);
            }
        }
        if (*leftmost_ptr == z)
        {
            if (!z->get_right())
            {
                *leftmost_ptr = z->get_parent();
            }
            else
            { 
                *leftmost_ptr = min(x);
            }
        }
        if (*rightmost_ptr == z)
        {
            if (!z->get_left())
            {
                *rightmost_ptr = z->get_parent();
            }
            else
            { 
                *rightmost_ptr = max(x);
            }
        }
    }
    if (y->get_color() != rb_node_color::red)
    {
        while (x != *root_ptr && (x == nullptr || x->get_color() == rb_node_color::black))
        {
            if (x == p->get_left())
            {
                rb_node_base* w = p->get_right();
                if (w->get_color() == rb_node_color::red)
                {
                    w->set_color(rb_node_color::black);
                    p->set_color(rb_node_color::red);
                    rotate_left(p, root_ptr);
                    w = p->get_right();
                }
                if ((w->get_left() == nullptr || w->get_left()->get_color() == rb_node_color::black) &&
                    (w->get_right() == nullptr || w->get_right()->get_color() == rb_node_color::black))
                {
                    w->set_color(rb_node_color::red);
                    x = p;
                    p = p->get_parent();
                }
                else
                { 
                    if (w->get_right() == nullptr || w->get_right()->get_color() == rb_node_color::black)
                    {
                        if (w->get_left())
                        {
                            w->get_left()->set_color(rb_node_color::black);
                        }
                        w->set_color(rb_node_color::red);
                        rotate_right(w, root_ptr);
                        w = p->get_right();
                    }
                    w->set_color(p->get_color());
                    p->set_color(rb_node_color::black);
                    if (w->get_right())
                    {
                        w->get_right()->set_color(rb_node_color::black);
                    }
                    rotate_left(p, root_ptr);
                    break;
                }
            }
            else
            {
                rb_node_base* w = p->get_left();
                if (w->get_color() == rb_node_color::red)
                {
                    w->set_color(rb_node_color::black);
                    p->set_color(rb_node_color::red);
                    rotate_right(p, root_ptr);
                    w = p->get_left();
                }
                if ((w->get_right() == nullptr || w->get_right()->get_color() == rb_node_color::black) &&
                    (w->get_left() == nullptr || w->get_left()->get_color() == rb_node_color::black))
                {
                    w->set_color(rb_node_color::red);
                    x = p;
                    p = p->get_parent();
                }
                else
                {
                    if (w->get_left() == nullptr || w->get_left()->get_color() == rb_node_color::black)
                    {
                        if (w->get_right())
                        {
                            w->get_right()->set_color(rb_node_color::black);
                        }
                        w->set_color(rb_node_color::red);
                        rotate_left(w, root_ptr);
                        w = p->get_left();
                    }
                    w->set_color(p->get_color());
                    p->set_color(rb_node_color::black);
                    if (w->get_left())
                    {
                        w->get_left()->set_color(rb_node_color::black);
                    }
                    rotate_right(p, root_ptr);
                    break;
                }
            }
        }
    }
    if (x)
    {
        x->set_color(rb_node_color::black);
    }
    return y;
}

} // namespace std

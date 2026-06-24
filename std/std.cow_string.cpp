module std.cow_string;

namespace std {

cow_string::cow_string() : chars()
{
}

cow_string::cow_string(const char* s) : chars(new vector<char>())
{
    set_chars(s);
}

cow_string& cow_string::operator=(const cow_string& that) 
{
    ensure_owning();
    set_chars(that.c_str());
    return *this;
}

cow_string& cow_string::operator=(const char* that)
{
    ensure_owning();
    set_chars(that);
    return *this;
}

cow_string& cow_string::operator=(const string& that)
{
    ensure_owning();
    set_chars(that.c_str());
    return *this;
}

void cow_string::ensure_owning()
{
    if (chars.use_count() > 1)
    {
        shared_ptr<vector<char>> old_chars;
        std::swap(chars, old_chars);
        if (old_chars)
        {
            set_chars(old_chars->data());
        }
    }
}

void cow_string::set_chars(const char* s)
{
    vector<char>* v = chars.get();
    v->clear();
    if (s)
    {
        while (*s)
        {
            v->push_back(*s);
            ++s;
        }
    }
    v->push_back('\0');
}

} // namespace std

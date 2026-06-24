export module std.cow_string;

import std.type.fundamental;
import std.basic_string;
import std.container.vector;
import std.utilities.shared_ptr;

export namespace std {

class cow_string
{
public:
    using size_type = int64_t;

    cow_string();
    cow_string(const char* s);
    inline cow_string(const string& s) : cow_string(s.c_str()) {}
    inline cow_string(const cow_string& s) noexcept : chars(s.chars) {}
    inline size_type length() const noexcept { return chars ? chars->size() - 1 : 0; }
    inline const char* c_str() const noexcept { return chars ? chars->data() : ""; }
    cow_string& operator=(const cow_string& that);
    cow_string& operator=(const char* that);
    cow_string& operator=(const string& that);
private:
    shared_ptr<std::vector<char>> chars;
    void ensure_owning();
    void set_chars(const char* s);
};

} // namespace std

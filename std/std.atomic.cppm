export module std.atomic;

import std.type.fundamental;

export namespace std {

class atomic_bool
{
public:
    atomic_bool();
    ~atomic_bool();
    operator bool();
    atomic_bool& operator=(bool value);
private:
    void* handle;
};

} // namespace std

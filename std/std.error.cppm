export module std.error;

import std.type.fundamental;
import std.basic_string;

export namespace std {

class error_code
{
public:
    error_code();
    ~error_code();
    void clear();
    void set_val(int val_) { val = val_; }
    void set_msg(const char* msg_) { msg = msg_; }
    int value() const { return val; }
    string message() const;
    explicit operator bool() const { return val != 0; }
private:
    int val;
    const char* msg;
};

} // namespace std

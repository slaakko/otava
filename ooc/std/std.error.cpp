module std.error; 

namespace std {

extern "C" void destroy_str(const char* str);

error_code::error_code() : val(0), msg(nullptr) 
{
}

error_code::error_code(int val_, const char* msg_) : val(val_), msg(msg_)
{
}

error_code::~error_code()
{
    if (msg)
    {
        destroy_str(msg);
    }
}

void error_code::clear()
{
    val = 0;
    if (msg)
    {
        destroy_str(msg);
    }
    msg = nullptr;
}

string error_code::message() const
{ 
    if (msg)
    {
        return std::string(msg);
    }
    else
    {
        return std::string();
    }
}

} // namespace std

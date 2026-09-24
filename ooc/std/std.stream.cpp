module std.stream; 

namespace std {

ios_base::ios_base() : fl(std::ios_base::dec)
{
}

ios_base::~ios_base()
{
}

ios_base::fmtflags ios_base::flags() const
{
    return fl;
}

void ios_base::flags(ios_base::fmtflags f)
{
    fl = f; 
}

void ios_base::setf(ios_base::fmtflags f)
{ 
    fl = fmtflags(int(fl) | int(f));
}

void ios_base::unsetf(ios_base::fmtflags f)
{ 
    fl = fmtflags(int(fl) & ~int(f));
}

void ios_base::init() noexcept
{
    flags(ios_base::fmtflags::dec);
}

ios_base& dec(std::ios_base& strm)
{
    strm.unsetf(std::ios_base::basefield);
    strm.setf(std::ios_base::dec);
    return strm;
}

ios_base& hex(std::ios_base& strm)
{
    strm.unsetf(std::ios_base::basefield);
    strm.setf(std::ios_base::hex);
    return strm;
}

ios_base& oct(std::ios_base& strm)
{
    strm.unsetf(std::ios_base::basefield);
    strm.setf(std::ios_base::oct);
    return strm;
}

ostream cout(1);
ostream cerr(2);

} // namespace std

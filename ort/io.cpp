// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module ort.io;

class io_table
{
public:
    static io_table& instance();
    std::ostream* get_ostream(int handle);
    int open(const char* file);
    void close(int handle);
    void clear();
private:
    io_table();
    std::map<int, std::ostream*> ostream_map;
    std::map<int, std::unique_ptr<std::fstream>> fstream_map;
    std::mutex mtx;
    int nextHandle;
};

io_table& io_table::instance()
{
    static io_table instance;
    return instance;
}
;
io_table::io_table() : nextHandle(3)
{
    ostream_map[1] = &std::cout;
    ostream_map[2] = &std::cerr;
}

std::ostream* io_table::get_ostream(int handle)
{
    auto it = ostream_map.find(handle);
    if (it != ostream_map.end())
    {
        return it->second;
    }
    else
    {
        std::cerr << "get_ostream: invalid handle";
        return nullptr;
    }
}

int io_table::open(const char* file)
{
    std::lock_guard<std::mutex> lock(mtx);
    int handle = nextHandle++;
    std::fstream* f = new std::fstream(file);
    fstream_map[handle] = std::unique_ptr<std::fstream>(f);
    ostream_map[handle] = f;
    return handle;
}

void io_table::clear()
{
    fstream_map.clear();
    ostream_map.clear();
}

void ort_io_init()
{
}

void ort_io_done()
{
    io_table::instance().clear();
}

void ort_io_write(int handle, const char* s)
{
    std::ostream* os = io_table::instance().get_ostream(handle);
    if (os)
    {
        (*os) << s;
    }
}

// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module ort.memory;

import ort;
import ort.trace;

namespace ort::memory {

struct memory_info
{
    memory_info(void* block_, std::int64_t size_, int serial_);
    void* block;
    std::int64_t size;
    int serial;
    bool freed;
    const char* function;
    const char* sourceFilePath;
    int line;
};

void print_info(memory_info* info, std::ostream* s)
{
    (*s) << "block=" << info->block << ";size=" << info->size << ";serial=" << info->serial;
    if (info->function)
    {
        (*s) << ";function=" << info->function;
    }
    if (info->sourceFilePath)
    {
        (*s) << ";sourceFilePath=" << info->sourceFilePath;
    }
    if (info->line != 0)
    {
        (*s) << ";line=" << info->line;
    }
    *s << "\n";
}

memory_info::memory_info(void* block_, std::int64_t size_, int serial_) :
    block(block_), size(size_), serial(serial_), freed(false), function(nullptr), sourceFilePath(nullptr), line(0)
{
}

class memory_map
{
public:
    static memory_map& instance();
    memory_info* get(void* block) const;
    memory_info* allocate(void* block, std::int64_t size);
    bool free(void* block);
    void set_info(void* block, const char* function, const char* sourceFilePath, int line);
    void print_leaks(std::ostream* s);
private:
    std::map<void*, memory_info*> map;
    int serial;
    memory_map();
};

memory_map& memory_map::instance()
{
    static memory_map inst;
    return inst;
}

memory_map::memory_map() : map(), serial(0)
{
}

memory_info* memory_map::get(void* block) const
{
    auto it = map.find(block);
    if (it != map.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

memory_info* memory_map::allocate(void* block, std::int64_t size)
{
    memory_info* info = get(block);
    if (info)
    {
        if (!info->freed)
        {
            std::cout << "memory block=" << block << ", size=" << info->size << ", serial=" << info->serial << " not freed" << "\n";
            ort_debug_break();
        }
        delete info;
    }
    info = new memory_info(block, size, serial++);
    map[block] = info;
    return info;
}

bool memory_map::free(void* block)
{
    memory_info* info = get(block);
    if (!info)
    {
        std::cout << "memory block=" << block << " not found" << "\n";
        return false;
    }
    else
    {
        if (info->freed)
        {
            std::cout << "memory block=" << block << ", size=" << info->size << ", serial=" << info->serial << " already freed" << "\n";
            return false;
        }
        else
        {
            info->freed = true;
        }
    }
    return true;
}

void memory_map::set_info(void* block, const char* function, const char* sourceFilePath, int line)
{
    memory_info* info = get(block);
    if (info)
    {
        info->function = function;
        info->sourceFilePath = sourceFilePath;
        info->line = line;
    }
    else
    {
        std::cout << "memory block=" << block << " not found" << "\n";
        ort_debug_break();
    }
}

struct serial_less
{
    inline bool operator()(memory_info* left, memory_info* right) const noexcept
    {
        return left->serial < right->serial;
    }
};

void memory_map::print_leaks(std::ostream* s)
{
    std::vector<memory_info*> leaks;
    for (const auto& mi : map)
    {
        memory_info* info = mi.second;
        if (!info->freed)
        {
            leaks.push_back(info);
        }
    }
    std::sort(leaks.begin(), leaks.end(), serial_less());
    if (leaks.empty())
    {
        std::cout << "no memory leaks detected" << "\n";
    }
    else
    {
        (*s) << leaks.size() << " memory leaks:" << "\n";
        for (memory_info* leak : leaks)
        {
            print_info(leak, s);
        }
    }
    for (const auto& mi : map)
    {
        memory_info* info = mi.second;
        delete info;
    }
    map.clear();
}

int allocation_serial = -1;

void set_allocation_serial(int serial)
{
    allocation_serial = serial;
}

void allocate(void* block, std::int64_t size)
{
    memory_info* info = memory_map::instance().allocate(block, size);
    if (info->serial == allocation_serial)
    {
        ort_debug_break();
    }
}

bool free(void* block)
{
    return memory_map::instance().free(block);
}

void set_info(void* block, const char* function, const char* sourceFilePath, int line)
{
    memory_map::instance().set_info(block, function, sourceFilePath, line);
}

void print_leaks(std::ostream* s)
{
    memory_map::instance().print_leaks(s);
}

} // namespace ort::memory

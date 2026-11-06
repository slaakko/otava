// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module ort.profile;

import std;

export namespace ort::profile {

class fn_profile
{
public:
    inline fn_profile() noexcept : id(0), count(0), time(0), ctime(0) {}
    inline fn_profile(std::uint64_t id_, std::uint64_t time_, std::uint64_t ctime_) noexcept : id(id_), count(1), time(time_), ctime(ctime_) {}
    inline fn_profile(std::uint64_t id_, std::uint64_t count_, std::uint64_t time_, std::uint64_t ctime_) noexcept : id(id_), count(count_), time(time_), ctime(ctime_) {}
    inline std::uint64_t get_id() const noexcept { return id; }
    inline std::uint64_t get_count() const noexcept { return count; }
    inline void inc_count() noexcept { ++count; }
    inline std::uint64_t get_time() const noexcept { return time; }
    inline void add_time(std::uint64_t t) noexcept { time += t; }
    inline std::uint64_t get_ctime() const noexcept { return ctime; }
    inline void add_ctime(std::uint64_t t) noexcept { ctime += t; }
private:
    std::uint64_t id;
    std::uint64_t count;
    std::uint64_t time;
    std::uint64_t ctime;
};

void add_to_profile(std::uint64_t fn, std::uint64_t time, std::uint64_t ctime);
void write_profile(const char* dir);

} // namespace ort::profile


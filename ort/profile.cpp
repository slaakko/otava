// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module ort.profile;

import util.path;

namespace ort::profile {

std::unordered_map<std::uint64_t, fn_profile> pmap;

void add_to_profile(std::uint64_t fn, std::uint64_t time, std::uint64_t ctime)
{
    auto it = pmap.find(fn);
    if (it != pmap.end())
    {
        fn_profile& profile = it->second;
        profile.inc_count();
        profile.add_time(time);
        profile.add_ctime(ctime);
    }
    else
    {
        pmap[fn] = fn_profile(fn, time, ctime);
    }
}

std::string three_digit_str(int x)
{
    std::string s;
    s.append(1, '0' + (x / 100) % 10);
    s.append(1, '0' + (x / 10) % 10);
    s.append(1, '0' + (x % 10));
    return s;
}

std::string make_profile_file_path(const char* dir, int i)
{
    std::string profileFilePath = util::GetFullPath(util::Path::Combine(dir, "profile." + three_digit_str(i) + ".txt"));
    return profileFilePath;
}

void write_profile_file(const std::string& profileFilePath)
{
    std::ofstream profileFile(profileFilePath);
    if (!profileFile)
    {
        std::cerr << "could not create file '" << profileFilePath << "'" << "\n";
        return;
    }
    for (const auto& p : pmap)
    {
        const fn_profile& profile = p.second;
        profileFile << profile.get_id() << ";" << profile.get_count() << ";" << profile.get_time() << ";" << profile.get_ctime() << "\n";
    }
}

void write_profile(const char* dir)
{
    for (int i = 0; i < 1000; ++i)
    {
        std::string profileFilePath = make_profile_file_path(dir, i);
        if (!util::FileExists(profileFilePath))
        {
            write_profile_file(profileFilePath);
            return;
        }
    }
    std::string profileFilePath = make_profile_file_path(dir, 999);
    write_profile_file(profileFilePath);
}

} // namespace ort::profile

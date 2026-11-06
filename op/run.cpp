// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.profiler.run;

import soul.rex.match;
import soul.rex.context;
import util.file_stream;
import util.text_util;
import util.time;
import ort.profile;

namespace otava::profiler {

std::string get_last_profile_file_path()
{
    std::string filePattern = "profile.*.txt";
    std::vector<std::string> profileFiles;
    std::filesystem::path current_dir = ".";
    for (const auto& dir_entry : std::filesystem::directory_iterator(current_dir))
    { 
        std::filesystem::path file = dir_entry;
        std::string filePath = std::filesystem::absolute(file).generic_string();
        if (soul::rex::FilePatternMatch(file.filename().generic_string(), filePattern))
        {
            profileFiles.push_back(filePath);
        }
    }
    std::ranges::sort(profileFiles);
    if (!profileFiles.empty())
    {
        return profileFiles.back();
    }
    throw std::runtime_error("no files matching 'profile.*.txt' found from the current directory");
}

std::string get_profile_file_path(const std::map<std::string, std::string>& commandMap)
{
    auto it = commandMap.find("file");
    if (it != commandMap.end())
    {
        std::string filePath = std::filesystem::absolute(it->second).generic_string();
        return filePath;
    }
    else
    {
        return get_last_profile_file_path();
    }
}

std::string get_profile_file_content(const std::map<std::string, std::string>& commandMap, std::string& profileFilePath)
{
    profileFilePath = get_profile_file_path(commandMap);
    if (!std::filesystem::exists(profileFilePath))
    {
        throw std::runtime_error("profile file '" + profileFilePath + "' does not exist");
    }
    std::string profile_file_content = util::ReadFile(profileFilePath);
    if (profile_file_content.empty() || (profile_file_content.back() != '\r' && profile_file_content.back() != '\n'))
    {
        profile_file_content.append(1, '\n');
    }
    return profile_file_content;
}

std::vector<ort::profile::fn_profile> get_function_profiles(const std::string& profile_file_path, const std::string& profile_file_content)
{
    std::vector<ort::profile::fn_profile> function_profiles;
    int state = 0;
    int field_index = 0;
    int line = 1;
    std::uint64_t id = 0;
    std::uint64_t count = 0;
    std::uint64_t time = 0;
    std::uint64_t ctime = 0;
    std::string field;
    for (char c : profile_file_content)
    {
        switch (state)
        {
        case 0:
        {
            if (c != ';' && c != '\n' && c != '\r')
            {
                field.append(1, c);
            }
            else if (c == ';' || c == '\n' || c == '\r')
            {
                std::uint64_t lfield = std::stoull(field);
                switch (field_index)
                {
                case 0:
                {
                    id = lfield;
                    break;
                }
                case 1:
                {
                    count = lfield;
                    break;
                }
                case 2:
                {
                    time = lfield;
                    break;
                }
                case 3:
                {
                    ctime = lfield;
                    break;
                }
                }
                field.clear();
                ++field_index;
            }
            else
            {
                throw std::runtime_error("invalid character in profile '" + profile_file_path + "' line " + std::to_string(line));
            }
            if (c == '\n' || c == '\r')
            {
                if (field_index != 4)
                {
                    throw std::runtime_error("invalid number of fields in profile '" + profile_file_path + "' line " + std::to_string(line));
                }
                function_profiles.push_back(ort::profile::fn_profile(id, count, time, ctime));
                field_index = 0;
                state = 1;
                ++line;
            }
            break;
        }
        case 1:
        {
            if (c != '\n' && c != '\r')
            {
                field.clear();
                field.append(1, c);
                state = 0;
            }
            break;
        }
        }
    }
    if (state != 1)
    {
        throw std::runtime_error("invalid profile '" + profile_file_path + "'");
    }
    return function_profiles;
}

std::vector<std::string> get_fn_file_paths(const std::string& fn_file_path, const std::string& fn_file_paths_file_content)
{
    std::vector<std::string> fn_file_paths;
    std::string current_fn_file_path;
    int state = 0;
    for (char c : fn_file_paths_file_content)
    {
        switch (state)
        {
        case 0:
        {
            if (c != '\n' && c != '\r')
            {
                current_fn_file_path.append(1, c);
            }
            else
            {
                fn_file_paths.push_back(current_fn_file_path);
                current_fn_file_path.clear();
                state = 1;
            }
            break;
        }
        case 1:
        {
            if (c != '\n' && c != '\r')
            {
                current_fn_file_path.append(1, c);
                state = 0;
            }
            break;
        }
        }
    }
    if (state != 1)
    {
        throw std::runtime_error("invalid file path file '" + fn_file_path + "'");
    }
    return fn_file_paths;
}

void read_fn_file(std::unordered_map<std::uint64_t, std::string>& function_name_map, const std::string& fn_file_path)
{
    if (std::filesystem::exists(fn_file_path))
    {
        std::string fn_file_content = util::ReadFile(fn_file_path);
        if (fn_file_content.empty() || (fn_file_content.back() != '\n' && fn_file_content.back() != '\r'))
        {
            fn_file_content.append(1, '\n');
        }
        int state = 0;
        std::string ids;
        std::string name;
        for (char c : fn_file_content)
        {
            switch (state)
            {
            case 0:
            {
                if (c == '\r' || c == '\n')
                {
                    state = 1;
                }
                else if (c == ';')
                {
                    state = 2;
                }
                else
                {
                    ids.append(1, c);
                }
                break;
            }
            case 1:
            {
                if (c != '\r' && c != '\n')
                {
                    ids.append(1, c);
                    state = 0;
                }
                break;
            }
            case 2:
            {
                if (c != '\r' && c != '\n')
                {
                    name.append(1, c);
                }
                else
                {
                    std::uint64_t id = std::stoull(ids);
                    function_name_map[id] = name;
                    ids.clear();
                    name.clear();
                    state = 1;
                }
                break;
            }
            }
        }
    }
    else
    {
        throw std::runtime_error("function file '" + fn_file_path + "' does not exist");
    }
}

std::unordered_map<std::uint64_t, std::string> get_function_name_map(const std::string& profile_file_path)
{
    std::unordered_map<std::uint64_t, std::string> function_name_map;
    std::filesystem::path fn_file_paths_file_path = std::filesystem::absolute(
        std::filesystem::path(profile_file_path).parent_path() / std::filesystem::path("fn_file_paths.txt"));
    if (std::filesystem::exists(fn_file_paths_file_path))
    {
        std::string fn_file_paths_file_content = util::ReadFile(fn_file_paths_file_path.generic_string());
        if (fn_file_paths_file_content.empty() || (fn_file_paths_file_content.back() != '\r' && fn_file_paths_file_content.back() != '\n'))
        {
            fn_file_paths_file_content.append(1, '\n');
        }
        std::vector<std::string> fn_file_paths = get_fn_file_paths(fn_file_paths_file_path.generic_string(), fn_file_paths_file_content);
        for (const auto& fn_file_path : fn_file_paths)
        {
            read_fn_file(function_name_map, fn_file_path);
        }
        return function_name_map;
    }
    else
    {
        throw std::runtime_error("file path file '" + fn_file_paths_file_path.generic_string() + "' does not exist");
    }
}

constexpr int percentField = 0;
constexpr int timeField = 1;
constexpr int countField = 2;
constexpr int workField = 3;
constexpr int idField = 4;
constexpr int nameField = 5;
constexpr int sortField = timeField;

int get_count(const std::map<std::string, std::string>& commandMap, const std::vector<ort::profile::fn_profile>& function_profiles)
{
    auto it = commandMap.find("count");
    if (it != commandMap.end())
    {
        return std::stoi(it->second);
    }
    return int(function_profiles.size());
}

std::string get_filter(const std::map<std::string, std::string>& commandMap)
{
    auto it = commandMap.find("filter");
    if (it != commandMap.end())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}

int get_sort_field(const std::map<std::string, std::string>& commandMap, const std::map<std::string, int>& fieldMap, const std::map<int, bool>& ascMap, bool& asc)
{
    bool ascSet = false;
    int field = sortField;
    auto it = commandMap.find("sort");
    if (it != commandMap.end())
    {
        std::string fieldStr = it->second;
        if (fieldStr.find(':') != std::string::npos)
        {
            std::vector<std::string> components = util::Split(fieldStr, ':');
            if (components.size() == 2)
            {
                fieldStr = components[0];
                std::string ascStr = components[1];
                if (ascStr == "asc")
                {
                    asc = true;
                    ascSet = true;
                }
                else if (ascStr == "desc")
                {
                    asc = false;
                    ascSet = true;
                }
            }
        }
        auto fieldIt = fieldMap.find(fieldStr);
        if (fieldIt != fieldMap.end())
        {
            field = fieldIt->second;
        }
    }
    if (!ascSet)
    {
        auto ascIt = ascMap.find(field);
        if (ascIt != ascMap.end())
        {
            asc = ascIt->second;
        }
    }
    return field;
}

std::string get_name(std::uint64_t id, const std::unordered_map<std::uint64_t, std::string>& nameMap)
{
    auto it = nameMap.find(id);
    if (it != nameMap.end())
    {
        return it->second;
    }
    return "<NOT FOUND>";
}

class function_comparison
{
public:
    function_comparison(int sortField_, bool asc_, const std::unordered_map<std::uint64_t, std::string>& nameMap_) : sortField(sortField_), asc(asc_), nameMap(nameMap_) {}
    bool operator()(const ort::profile::fn_profile& left, const ort::profile::fn_profile& right) const
    {
        switch (sortField)
        {
        case percentField: case timeField: default:
        {
            if (asc)
            {
                return left.get_time() < right.get_time();
            }
            else
            {
                return left.get_time() > right.get_time();
            }
            break;
        }
        case countField:
        {
            if (asc)
            {
                return left.get_count() < right.get_count();
            }
            else
            {
                return left.get_count() > right.get_count();
            }
            break;
        }
        case workField:
        {
            std::uint64_t leftWork = left.get_time() - left.get_ctime();
            std::uint64_t rightWork = right.get_time() - right.get_ctime();
            if (asc)
            {
                return leftWork < rightWork;
            }
            else
            {
                return leftWork > rightWork;
            }
            break;
        }
        case idField:
        {
            if (asc)
            {
                return left.get_id() < right.get_id();
            }
            else
            {
                return left.get_id() > right.get_id();
            }
            break;
        }
        case nameField:
        {
            std::string leftName = get_name(left.get_id(), nameMap);
            std::string rightName = get_name(right.get_id(), nameMap);
            if (asc)
            {
                return leftName < rightName;
            }
            else
            {
                return leftName > rightName;
            }
            break;
        }
        }
    }
private:
    int sortField;
    bool asc;
    const std::unordered_map<std::uint64_t, std::string>& nameMap;
};

void sort_profiles(std::vector<ort::profile::fn_profile>& function_profiles, const std::unordered_map<std::uint64_t, std::string>& function_name_map, 
    int sortField, bool asc)
{
    function_comparison comparison(sortField, asc, function_name_map);
    std::ranges::sort(function_profiles, comparison);
}

std::string get_percent(std::uint64_t time, std::uint64_t max_time)
{
    if (max_time == 0) return "100.0";
    double percent = 100.0 * time / (max_time * 1.0);
    std::uint64_t percentTimesTen = static_cast<std::uint64_t>((percent * 10.0) + 0.5);
    std::string percentStr;
    percentStr.append(1, '0' + (percentTimesTen / 1000) % 10);
    percentStr.append(1, '0' + (percentTimesTen / 100) % 10);
    percentStr.append(1, '0' + (percentTimesTen / 10) % 10);
    percentStr.append(1, '.');
    percentStr.append(1, '0' + percentTimesTen % 10);
    return percentStr;
}

std::string get_time(std::uint64_t time)
{
    std::chrono::nanoseconds ns(time);
    return util::DurationStr(ns);
}

std::string get_count(std::uint64_t count)
{
    return  std::to_string(count);
}

std::string get_work(std::uint64_t work)
{
    std::chrono::nanoseconds ns(work);
    return util::DurationStr(ns);
}

std::string get_id(std::uint64_t id)
{
    return std::to_string(id);
}

std::vector<int> get_field_widths(std::vector<ort::profile::fn_profile>& prn_profiles, std::uint64_t max_time)
{
    std::vector<int> fieldWidths;
    fieldWidths.push_back(0);
    fieldWidths.push_back(0);
    fieldWidths.push_back(0);
    fieldWidths.push_back(0);
    fieldWidths.push_back(0);
    fieldWidths.push_back(0);
    for (ort::profile::fn_profile& profile : prn_profiles)
    {
        std::string percentStr = get_percent(profile.get_time(), max_time);
        fieldWidths[0] = std::max(fieldWidths[0], int(percentStr.length()));
        std::string timeStr = get_time(profile.get_time());
        fieldWidths[1] = std::max(fieldWidths[1], int(timeStr.length()));
        std::string countStr = get_count(profile.get_count());
        fieldWidths[2] = std::max(fieldWidths[2], int(countStr.length()));
        std::string workStr = get_work(profile.get_time() - profile.get_ctime());
        fieldWidths[3] = std::max(fieldWidths[3], int(workStr.length()));
        std::string idStr = get_id(profile.get_id());
        fieldWidths[4] = std::max(fieldWidths[4], int(idStr.length()));
    }
    return fieldWidths;
}

void print_profiles(const std::vector<ort::profile::fn_profile>& prn_profiles, const std::unordered_map<std::uint64_t, std::string>& function_name_map,
    const std::vector<int>& fieldWidths, std::uint64_t max_time)
{
    for (const ort::profile::fn_profile& profile : prn_profiles)
    {
        std::string line;
        std::string percentStr = get_percent(profile.get_time(), max_time);
        line.append(util::Format(percentStr, fieldWidths[0], util::FormatWidth::min, util::FormatJustify::right, '0'));
        line.append(1, ';');
        std::string timeStr = get_time(profile.get_time());
        line.append(util::Format(timeStr, fieldWidths[1], util::FormatWidth::min, util::FormatJustify::right, '0'));
        line.append(1, ';');
        std::string countStr = get_count(profile.get_count());
        line.append(util::Format(countStr, fieldWidths[2], util::FormatWidth::min, util::FormatJustify::right, '0'));
        line.append(1, ';');
        std::string workStr = get_work(profile.get_time() - profile.get_ctime());
        line.append(util::Format(workStr, fieldWidths[3], util::FormatWidth::min, util::FormatJustify::right, '0'));
        line.append(1, ';');
        std::string idStr = get_id(profile.get_id());
        line.append(util::Format(idStr, fieldWidths[4], util::FormatWidth::min, util::FormatJustify::right, '0'));
        line.append(1, ';');
        std::string nameStr = get_name(profile.get_id(), function_name_map);
        line.append(nameStr);
        std::cout << line << "\n";
    }
}

std::vector<ort::profile::fn_profile> filter_profiles(const std::vector<ort::profile::fn_profile>& function_profiles, 
    const std::unordered_map<std::uint64_t, std::string>& function_name_map, const std::string& filter)
{
    std::vector<ort::profile::fn_profile> filtered_profiles;
    soul::rex::context::Context context;
    soul::rex::nfa::Nfa nfa = soul::rex::CompileFilePattern(context, filter);
    for (const auto& profile : function_profiles)
    {
        std::string name = get_name(profile.get_id(), function_name_map);
        bool match = soul::rex::PatternMatch(name, nfa);
        if (match)
        {
            filtered_profiles.push_back(profile);
        }
    }
    return filtered_profiles;
}

void print_profiles(const std::map<std::string, std::string>& commandMap, const std::unordered_map<std::uint64_t, std::string>& function_name_map,
    std::vector<ort::profile::fn_profile>& function_profiles)
{
    std::map<std::string, int> fieldMap;
    fieldMap["percent"] = percentField;
    fieldMap["time"] = timeField;
    fieldMap["count"] = countField;
    fieldMap["work"] = workField;
    fieldMap["id"] = idField;
    fieldMap["name"] = nameField;
    std::map<int, bool> ascMap;
    ascMap[percentField] = false;
    ascMap[timeField] = false;
    ascMap[countField] = false;
    ascMap[workField] = false;
    ascMap[idField] = true;
    ascMap[nameField] = true;
    int count = get_count(commandMap, function_profiles);
    bool asc = false;
    int sortField = get_sort_field(commandMap, fieldMap, ascMap, asc);
    sort_profiles(function_profiles, function_name_map, sortField, asc);
    std::uint64_t max_time = 0;
    if (sortField == timeField && !asc && !function_profiles.empty())
    {
        max_time = function_profiles.front().get_time();
    }
    else if (sortField == timeField && asc && !function_profiles.empty())
    {
        max_time = function_profiles.back().get_time();
    }
    else
    {
        if (!function_profiles.empty())
        {
            std::vector<ort::profile::fn_profile> sorted_profiles = function_profiles;
            std::ranges::sort(sorted_profiles, function_comparison(timeField, false, function_name_map));
            max_time = sorted_profiles.front().get_time();
        }
    }
    std::string filter = get_filter(commandMap);
    if (!filter.empty())
    {
        function_profiles = filter_profiles(function_profiles, function_name_map, filter);
    }
    std::vector<ort::profile::fn_profile> prn_profiles;
    count = std::min(count, int(function_profiles.size()));
    for (int i = 0; i < count; ++i)
    {
        prn_profiles.push_back(function_profiles[i]);
    }
    std::vector<int> fieldWidths = get_field_widths(prn_profiles, max_time);
    print_profiles(prn_profiles, function_name_map, fieldWidths, max_time);
}

void run(const std::map<std::string, std::string>& commandMap)
{
    std::string profile_file_path;
    std::string profile_file_content = get_profile_file_content(commandMap, profile_file_path);
    std::unordered_map<std::uint64_t, std::string> function_name_map = get_function_name_map(profile_file_path);
    std::vector<ort::profile::fn_profile> function_profiles = get_function_profiles(profile_file_path, profile_file_content);
    print_profiles(commandMap, function_name_map, function_profiles);
}

} // otava::profiler

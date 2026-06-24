// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.filesystem;

import util.text_util;

void* fs_path_default_ctor()
{
    return new std::filesystem::path();
}

void* fs_path_copy_ctor(void* p)
{
    return new std::filesystem::path(*static_cast<std::filesystem::path*>(p));
}

void* fs_path_string_ctor(const char* source)
{
    return new std::filesystem::path(source);
}

void fs_path_dtor(void* p)
{
    std::filesystem::path* path = static_cast<std::filesystem::path*>(p);
    delete path;
}

void fs_path_cat(void* p, void* q)
{
    std::filesystem::path& left = *static_cast<std::filesystem::path*>(p);
    std::filesystem::path& right = *static_cast<std::filesystem::path*>(q);
    left /= right;
}

void fs_path_append(void* p, void* q)
{
    std::filesystem::path& left = *static_cast<std::filesystem::path*>(p);
    std::filesystem::path& right = *static_cast<std::filesystem::path*>(q);
    left += right;
}

void fs_path_append_str(void* p, const char* x)
{
    std::filesystem::path& left = *static_cast<std::filesystem::path*>(p);
    left += x;
}

void fs_path_append_char(void* p, char x)
{
    std::filesystem::path& left = *static_cast<std::filesystem::path*>(p);
    left += x;
}

void fs_path_clear(void* p)
{
    std::filesystem::path& path = *static_cast<std::filesystem::path*>(p);
    path.clear();
}

void fs_path_make_preferred(void* p)
{
    std::filesystem::path& path = *static_cast<std::filesystem::path*>(p);
    path.make_preferred();
}

void fs_path_remove_filename(void* p)
{
    std::filesystem::path& path = *static_cast<std::filesystem::path*>(p);
    path.remove_filename();
}

void fs_path_replace_filename(void* p, void* q)
{
    std::filesystem::path& left = *static_cast<std::filesystem::path*>(p);
    std::filesystem::path& right = *static_cast<std::filesystem::path*>(q);
    left.replace_filename(right);
}

void fs_path_replace_extension(void* p, void* q)
{
    std::filesystem::path& left = *static_cast<std::filesystem::path*>(p);
    std::filesystem::path& right = *static_cast<std::filesystem::path*>(q);
    left.replace_extension(right);
}

void fs_path_remove_extension(void* p)
{
    std::filesystem::path& path = *static_cast<std::filesystem::path*>(p);
    path.replace_extension();
}

bool fs_path_equal(void* p, void* q)
{
    std::filesystem::path& left = *static_cast<std::filesystem::path*>(p);
    std::filesystem::path& right = *static_cast<std::filesystem::path*>(q);
    return left == right;
}

void* fs_path_cat2(void* p, void* q)
{
    std::filesystem::path& left = *static_cast<std::filesystem::path*>(p);
    std::filesystem::path& right = *static_cast<std::filesystem::path*>(q);
    std::filesystem::path result = left / right;
    return new std::filesystem::path(std::move(result));
}

const char* fs_path_c_str(void* p)
{
    std::filesystem::path& path = *static_cast<std::filesystem::path*>(p);
    std::string s = path.string();
    char* c_str = static_cast<char*>(std::malloc(s.length() + 1));
#pragma warning(suppress : 4996)
    std::strcpy(c_str, s.c_str());
    return c_str;
}

const char* fs_path_generic_str(void* p)
{
    std::filesystem::path& path = *static_cast<std::filesystem::path*>(p);
    std::string s = path.generic_string();
    char* c_str = static_cast<char*>(std::malloc(s.length() + 1));
#pragma warning(suppress : 4996)
    std::strcpy(c_str, s.c_str());
    return c_str;
}

void fs_path_destroy_str(const char* s)
{
    delete s;
}

void* fs_path_root_name(void* p)
{
    return new std::filesystem::path(static_cast<std::filesystem::path*>(p)->root_name());
}

void* fs_path_root_directory(void* p)
{
    return new std::filesystem::path(static_cast<std::filesystem::path*>(p)->root_directory());
}

void* fs_path_root_path(void* p)
{
    return new std::filesystem::path(static_cast<std::filesystem::path*>(p)->root_path());
}

void* fs_path_relative_path(void* p)
{
    return new std::filesystem::path(static_cast<std::filesystem::path*>(p)->relative_path());
}

void* fs_path_parent_path(void* p)
{
    return new std::filesystem::path(static_cast<std::filesystem::path*>(p)->parent_path());
}

void* fs_path_filename(void* p)
{
    return new std::filesystem::path(static_cast<std::filesystem::path*>(p)->filename());
}

void* fs_path_stem(void* p)
{
    return new std::filesystem::path(static_cast<std::filesystem::path*>(p)->stem());
}

void* fs_path_extension(void* p)
{
    return new std::filesystem::path(static_cast<std::filesystem::path*>(p)->extension());
}

bool fs_path_empty(void* p)
{
    return static_cast<std::filesystem::path*>(p)->empty();
}

bool fs_path_has_root_name(void* p)
{
    return static_cast<std::filesystem::path*>(p)->has_root_name();
}

bool fs_path_has_root_directory(void* p)
{
    return static_cast<std::filesystem::path*>(p)->has_root_directory();
}

bool fs_path_has_root_path(void* p)
{
    return static_cast<std::filesystem::path*>(p)->has_root_path();
}

bool fs_path_has_relative_path(void* p)
{
    return static_cast<std::filesystem::path*>(p)->has_relative_path();
}

bool fs_path_has_parent_path(void* p)
{
    return static_cast<std::filesystem::path*>(p)->has_parent_path();
}

bool fs_path_has_filename(void* p)
{
    return static_cast<std::filesystem::path*>(p)->has_filename();
}

bool fs_path_has_stem(void* p)
{
    return static_cast<std::filesystem::path*>(p)->has_stem();
}

bool fs_path_has_extension(void* p)
{
    return static_cast<std::filesystem::path*>(p)->has_extension();
}

bool fs_path_is_absolute(void* p)
{
    return static_cast<std::filesystem::path*>(p)->is_absolute();
}

bool fs_path_is_relative(void* p)
{
    return static_cast<std::filesystem::path*>(p)->is_relative();
}

void* fs_path_absolute(void* p)
{
    try
    {
        return new std::filesystem::path(std::filesystem::absolute(*static_cast<std::filesystem::path*>(p)));
    }
    catch (...)
    {
        return nullptr;
    }
}

void* fs_path_absolute_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    std::filesystem::path path = std::filesystem::absolute(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return new std::filesystem::path(std::move(path));
}

void* fs_path_canonical(void* p)
{
    try
    {
        return new std::filesystem::path(std::filesystem::canonical(*static_cast<std::filesystem::path*>(p)));
    }
    catch (...)
    {
        return nullptr;
    }
}

void* fs_path_canonical_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    std::filesystem::path path = std::filesystem::canonical(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return new std::filesystem::path(std::move(path));
}

bool fs_copy(void* from, void* to)
{
    try
    {
        std::filesystem::copy(*static_cast<std::filesystem::path*>(from), *static_cast<std::filesystem::path*>(to));
    }
    catch (...)
    {
        return false;
    }
    return true;
}

void fs_copy_error(void* from, void* to, int* error, char** msg)
{
    std::error_code ec;
    std::filesystem::copy(*static_cast<std::filesystem::path*>(from), *static_cast<std::filesystem::path*>(to), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
}

bool fs_copy_file(void* from, void* to)
{
    try
    {
        return std::filesystem::copy_file(*static_cast<std::filesystem::path*>(from), *static_cast<std::filesystem::path*>(to));
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool fs_copy_file_error(void* from, void* to, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::copy_file(*static_cast<std::filesystem::path*>(from), *static_cast<std::filesystem::path*>(to), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

bool fs_create_directories(void* path)
{
    try
    {
        return std::filesystem::create_directories(*static_cast<std::filesystem::path*>(path));
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool fs_create_directories_error(void* path, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::create_directories(*static_cast<std::filesystem::path*>(path), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

void* fs_current_path()
{
    try
    {
        return new std::filesystem::path(std::filesystem::current_path());
    }
    catch (...)
    {
        return nullptr;
    }
}

void* fs_current_path_error(int* error, char** msg)
{
    std::error_code ec;
    void* retval = new std::filesystem::path(std::filesystem::current_path(ec));
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

bool fs_set_current_path(void* p)
{
    try
    {
        std::filesystem::current_path(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return false;
    }
    return true;
}

void fs_set_current_path_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    std::filesystem::current_path(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
}

bool fs_equivalent(void* p1, void* p2)
{
    try
    {
        std::filesystem::path& left = *static_cast<std::filesystem::path*>(p1);
        std::filesystem::path& right = *static_cast<std::filesystem::path*>(p2);
        return std::filesystem::equivalent(left, right);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool fs_equivalent_error(void* p1, void* p2, int* error, char** msg)
{
    std::error_code ec;
    std::filesystem::path& left = *static_cast<std::filesystem::path*>(p1);
    std::filesystem::path& right = *static_cast<std::filesystem::path*>(p2);
    bool retval = std::filesystem::equivalent(left, right, ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        strcpy(*msg, m.c_str());
    }
    return retval;
}

bool fs_exists(void* p)
{
    try
    {
        std::filesystem::path path = *static_cast<std::filesystem::path*>(p);
        return std::filesystem::exists(path);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool fs_exists_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::exists(*static_cast<std::filesystem::path*>(p));
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        strcpy(*msg, m.c_str());
    }
    return retval;
}

std::uint64_t fs_file_size(void* p)
{
    try
    {
        return std::filesystem::file_size(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return 0;
    }
    return 0;
}

std::uint64_t fs_file_size_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    std::uint64_t retval = std::filesystem::file_size(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

bool fs_is_block_file(void* p)
{
    try
    {
        return std::filesystem::is_block_file(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return 0;
    }
    return 0;
}

bool fs_is_block_file_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::is_block_file(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

bool fs_is_character_file(void* p)
{
    try
    {
        return std::filesystem::is_character_file(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return 0;
    }
    return 0;
}

bool fs_is_character_file_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::is_character_file(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

bool fs_is_directory(void* p)
{
    try
    {
        return std::filesystem::is_directory(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return 0;
    }
    return 0;
}

bool fs_is_directory_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::is_directory(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

bool fs_is_empty(void* p)
{
    try
    {
        return std::filesystem::is_empty(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return 0;
    }
    return 0;
}

bool fs_is_empty_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::is_empty(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

bool fs_is_fifo(void* p)
{
    try
    {
        return std::filesystem::is_fifo(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return 0;
    }
    return 0;
}

bool fs_is_fifo_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::is_fifo(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

bool fs_is_other(void* p)
{
    try
    {
        return std::filesystem::is_other(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return 0;
    }
    return 0;
}

bool fs_is_other_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::is_other(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

bool fs_is_regular_file(void* p)
{
    try
    {
        return std::filesystem::is_regular_file(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return 0;
    }
    return 0;
}

bool fs_is_regular_file_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::is_regular_file(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

bool fs_is_socket(void* p)
{
    try
    {
        return std::filesystem::is_socket(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return 0;
    }
    return 0;
}

bool fs_is_socket_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::is_socket(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

bool fs_is_symlink(void* p)
{
    try
    {
        return std::filesystem::is_symlink(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return 0;
    }
    return 0;
}

bool fs_is_symlink_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::is_symlink(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

std::int64_t fs_last_write_time(void* p)
{
    try
    {
        auto time = std::filesystem::last_write_time(*static_cast<std::filesystem::path*>(p));
        std::int64_t nsecs = std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count();
        return nsecs;
    }
    catch (...)
    {
        return -1;
    }
}

std::int64_t fs_last_write_time_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    auto time = std::filesystem::last_write_time(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    std::int64_t nsecs = std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count();
    return nsecs;
}

void* fs_relative(void* p)
{
    try
    {
        return new std::filesystem::path(std::filesystem::relative(*static_cast<std::filesystem::path*>(p)));
    }
    catch (...)
    {
        return nullptr;
    }
}

void* fs_relative_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    std::filesystem::path path = std::filesystem::relative(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return new std::filesystem::path(std::move(path));
}

void* fs_relative_base(void* p, void* base)
{
    try
    {
        return new std::filesystem::path(std::filesystem::relative(*static_cast<std::filesystem::path*>(p), *static_cast<std::filesystem::path*>(base)));
    }
    catch (...)
    {
        return nullptr;
    }
}

void* fs_relative_base_error(void* p, void* base, int* error, char** msg)
{
    std::error_code ec;
    std::filesystem::path path = std::filesystem::relative(*static_cast<std::filesystem::path*>(p), *static_cast<std::filesystem::path*>(base), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return new std::filesystem::path(std::move(path));
}

bool fs_remove(void* p)
{
    try
    {
        return std::filesystem::remove(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return false;
    }
}

bool fs_remove_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    bool retval = std::filesystem::remove(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

std::uint64_t fs_remove_all(void* p)
{
    try
    {
        return std::filesystem::remove_all(*static_cast<std::filesystem::path*>(p));
    }
    catch (...)
    {
        return -1;
    }
}

std::uint64_t fs_remove_all_error(void* p, int* error, char** msg)
{
    std::error_code ec;
    std::uint64_t retval = std::filesystem::remove_all(*static_cast<std::filesystem::path*>(p), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
    return retval;
}

void fs_rename(void* from, void* to)
{
    try
    {
        std::filesystem::rename(*static_cast<std::filesystem::path*>(from), *static_cast<std::filesystem::path*>(to));
    }
    catch (...)
    {
    }
}

void fs_rename_error(void* from, void* to, int* error, char** msg)
{
    std::error_code ec;
    std::filesystem::rename(*static_cast<std::filesystem::path*>(from), *static_cast<std::filesystem::path*>(to), ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
}

void fs_resize_file(void* p, std::uint64_t size)
{
    try
    {
        std::filesystem::resize_file(*static_cast<std::filesystem::path*>(p), size);
    }
    catch (...)
    {
    }
}

void fs_resize_file_error(void* p, std::uint64_t size, int* error, char** msg)
{
    std::error_code ec;
    std::filesystem::resize_file(*static_cast<std::filesystem::path*>(p), size, ec);
    if (ec)
    {
        *error = ec.value();
        std::string m = util::PlatformStringToUtf8(ec.message());
        *msg = static_cast<char*>(std::malloc(m.length() + 1));
#pragma warning(suppress : 4996)
        std::strcpy(*msg, m.c_str());
    }
}

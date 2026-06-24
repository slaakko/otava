module std.filesystem;

import std.utf;

namespace std::filesystem {

extern "C" void* fs_path_default_ctor();
extern "C" void* fs_path_copy_ctor(void* p);
extern "C" void* fs_path_string_ctor(const char* source);
extern "C" void fs_path_dtor(void* p);
extern "C" void fs_path_cat(void* p, void* q);
extern "C" void fs_path_append(void* p, void* q);
extern "C" void fs_path_append_str(void* p, const char* x);
extern "C" void fs_path_append_char(void* p, char x);
extern "C" void fs_path_clear(void* p);
extern "C" void fs_path_make_preferred(void* p);
extern "C" void fs_path_remove_filename(void* p);
extern "C" void fs_path_replace_filename(void* p, void* q);
extern "C" void fs_path_replace_extension(void* p, void* q);
extern "C" void fs_path_remove_extension(void* p);
extern "C" bool fs_path_equal(void* p, void* q);
extern "C" void* fs_path_cat2(void* p, void* q);
extern "C" const char* fs_path_c_str(void* p);
extern "C" const char* fs_path_generic_str(void* p);
extern "C" void fs_path_destroy_str(const char* s);
extern "C" void* fs_path_root_name(void* p);
extern "C" void* fs_path_root_directory(void* p);
extern "C" void* fs_path_root_path(void* p);
extern "C" void* fs_path_relative_path(void* p);
extern "C" void* fs_path_parent_path(void* p);
extern "C" void* fs_path_filename(void* p);
extern "C" void* fs_path_stem(void* p);
extern "C" void* fs_path_extension(void* p);
extern "C" bool fs_path_empty(void* p);
extern "C" bool fs_path_has_root_name(void* p);
extern "C" bool fs_path_has_root_directory(void* p);
extern "C" bool fs_path_has_root_path(void* p);
extern "C" bool fs_path_has_relative_path(void* p);
extern "C" bool fs_path_has_parent_path(void* p);
extern "C" bool fs_path_has_filename(void* p);
extern "C" bool fs_path_has_stem(void* p);
extern "C" bool fs_path_has_extension(void* p);
extern "C" bool fs_path_is_absolute(void* p);
extern "C" bool fs_path_is_relative(void* p);
extern "C" void* fs_path_absolute(void* p);
extern "C" void* fs_path_absolute_error(void* p, int* error, char** msg);
extern "C" void* fs_path_canonical(void* p);
extern "C" void* fs_path_canonical_error(void* p, int* error, char** msg);
extern "C" void fs_copy(void* from, void* to);
extern "C" void fs_copy_error(void* from, void* to, int* error, char** msg);
extern "C" bool fs_copy_file(void* from, void* to);
extern "C" bool fs_copy_file_error(void* from, void* to, int* error, char** msg);
extern "C" bool fs_create_directories(void* path);
extern "C" bool fs_create_directories_error(void* path, int* error, char** msg);
extern "C" void* fs_current_path();
extern "C" void* fs_current_path_error(int* error, char** msg);
extern "C" bool fs_set_current_path(void* p);
extern "C" void fs_set_current_path_error(void* p, int* error, char** msg);
extern "C" bool fs_equivalent(void* p1, void* p2);
extern "C" bool fs_equivalent_error(void* p1, void* p2, int* error, char** msg);
extern "C" bool fs_exists(void* p);
extern "C" bool fs_exists_error(void* p, int* error, char** msg);
extern "C" uint64_t fs_file_size(void* p);
extern "C" uint64_t fs_file_size_error(void* p, int* error, char** msg);
extern "C" bool fs_is_block_file(void* p);
extern "C" bool fs_is_block_file_error(void* p, int* error, char** msg);
extern "C" bool fs_is_character_file(void* p);
extern "C" bool fs_is_character_file_error(void* p, int* error, char** msg);
extern "C" bool fs_is_directory(void* p);
extern "C" bool fs_is_directory_error(void* p, int* error, char** msg);
extern "C" bool fs_is_empty(void* p);
extern "C" bool fs_is_empty_error(void* p, int* error, char** msg);
extern "C" bool fs_is_fifo(void* p);
extern "C" bool fs_is_fifo_error(void* p, int* error, char** msg);
extern "C" bool fs_is_other(void* p);
extern "C" bool fs_is_other_error(void* p, int* error, char** msg);
extern "C" bool fs_is_regular_file(void* p);
extern "C" bool fs_is_regular_file_error(void* p, int* error, char** msg);
extern "C" bool fs_is_socket(void* p);
extern "C" bool fs_is_socket_error(void* p, int* error, char** msg);
extern "C" bool fs_is_symlink(void* p);
extern "C" bool fs_is_symlink_error(void* p, int* error, char** msg);
extern "C" int64_t fs_last_write_time(void* p);
extern "C" int64_t fs_last_write_time_error(void* p, int* error, char** msg);
extern "C" void* fs_relative(void* p);
extern "C" void* fs_relative_error(void* p, int* error, char** msg);
extern "C" void* fs_relative_base(void* p, void* base);
extern "C" void* fs_relative_base_error(void* p, void* base, int* error, char** msg);
extern "C" bool fs_remove(void* p);
extern "C" bool fs_remove_error(void* p, int* error, char** msg);
extern "C" uint64_t fs_remove_all(void* p);
extern "C" uint64_t fs_remove_all_error(void* p, int* error, char** msg);
extern "C" void fs_rename(void* from, void* to);
extern "C" void fs_rename_error(void* from, void* to, int* error, char** msg);
extern "C" void fs_resize_file(void* p, uint64_t size);
extern "C" void fs_resize_file_error(void* p, uint64_t size, int* error, char** msg);

path::path() : handle(fs_path_default_ctor())
{
}

path::path(void* handle_) noexcept : handle(handle_)
{
}

bool path::valid() const noexcept
{ 
    return handle != nullptr; 
}

path::path(const path& p) : handle(fs_path_copy_ctor(p.handle))
{
}

path::path(path&& that) noexcept : handle(that.handle)
{
    that.handle = nullptr;
}

path::path(const path::string_type& s) : handle(fs_path_string_ctor(s.c_str()))
{
}

path::path(const char* source) : handle(fs_path_string_ctor(source))
{
}

path::~path()
{
    if (handle)
    {
        fs_path_dtor(handle);
    }
}

path& path::operator=(const path& p)
{
    fs_path_dtor(handle);
    handle = fs_path_copy_ctor(p.handle);
    return *this;
}

path& path::operator=(path&& p) noexcept
{
    std::swap(handle, p.handle);
    return *this;
}

path& path::operator=(const char* source)
{
    if (handle)
    {
        fs_path_dtor(handle);
    }
    handle = fs_path_string_ctor(source);
    return *this;
}

path& path::assign(const char* source)
{
    if (handle)
    {
        fs_path_dtor(handle);
    }
    handle = fs_path_string_ctor(source);
    return *this;
}

path& path::operator/=(const path& p)
{
    fs_path_cat(handle, p.handle);
    return *this;
}

path& path::operator+=(const path& p)
{
    fs_path_append(handle, p.handle);
    return *this;
}

path& path::operator+=(const char* x)
{
    fs_path_append_str(handle, x);
    return *this;
}

path& path::operator+=(path::value_type x)
{
    fs_path_append_char(handle, x);
    return *this;
}

void path::clear() noexcept
{
    fs_path_clear(handle);
}

path& path::make_preferred()
{
    fs_path_make_preferred(handle);
    return *this;
}

path& path::remove_filename()
{
    fs_path_remove_filename(handle);
    return *this;
}

path& path::replace_filename(const path& p)
{
    fs_path_replace_filename(handle, p.handle);
    return *this;
}

path& path::replace_extension(const path& ext)
{
    fs_path_replace_extension(handle, ext.handle);
    return* this; 
}

path& path::replace_extension()
{
    fs_path_remove_extension(handle);
    return *this;
}

std::string path::string() const
{
    const char* s = fs_path_c_str(handle);
    std::string str(s);
    fs_path_destroy_str(s);
    return str;
}

std::u16string path::u16string() const
{
    const char* s = fs_path_c_str(handle);
    std::string str(s);
    std::u16string u16str = to_utf16(str);
    fs_path_destroy_str(s);
    return u16str;
}

std::u32string path::u32string() const
{
    const char* s = fs_path_c_str(handle);
    std::string str(s);
    std::u32string u32str = to_utf32(str);
    fs_path_destroy_str(s);
    return u32str;
}

std::string path::generic_string() const
{
    const char* s = fs_path_generic_str(handle);
    std::string str(s);
    fs_path_destroy_str(s);
    return str;
}

std::u16string path::generic_u16string() const
{
    const char* s = fs_path_generic_str(handle);
    std::string str(s);
    fs_path_destroy_str(s);
    return to_utf16(str);
}

std::u32string path::generic_u32string() const
{
    const char* s = fs_path_generic_str(handle);
    std::string str(s);
    fs_path_destroy_str(s);
    return to_utf32(str);
}

path path::root_name() const
{
    return path(fs_path_root_name(handle));
}

path path::root_directory() const
{
    return path(fs_path_root_directory(handle));
}

path path::root_path() const
{
    return path(fs_path_root_path(handle));
}

path path::relative_path() const
{
    return path(fs_path_relative_path(handle));
}

path path::parent_path() const
{
    return path(fs_path_parent_path(handle));
}

path path::filename() const
{
    return path(fs_path_filename(handle));
}

path path::stem() const
{
    return path(fs_path_stem(handle));
}

path path::extension() const
{
    return path(fs_path_extension(handle));
}

bool path::empty() const noexcept
{
    return fs_path_empty(handle);
}

bool path::has_root_name() const noexcept
{
    return fs_path_has_root_name(handle);
}

bool path::has_root_directory() const noexcept
{
    return fs_path_has_root_directory(handle);
}

bool path::has_root_path() const noexcept
{
    return fs_path_has_root_path(handle);
}

bool path::has_relative_path() const noexcept
{
    return fs_path_has_relative_path(handle);
}

bool path::has_parent_path() const noexcept
{
    return fs_path_has_parent_path(handle);
}

bool path::has_filename() const noexcept
{
    return fs_path_has_filename(handle);
}

bool path::has_stem() const noexcept
{
    return fs_path_has_stem(handle);
}

bool path::has_extension() const noexcept
{
    return fs_path_has_extension(handle);
}

bool path::is_absolute() const noexcept
{
    return fs_path_is_absolute(handle);
}

bool path::is_relative() const noexcept
{
    return fs_path_is_relative(handle);
}

bool operator==(const path& lhs, const path& rhs) noexcept
{
    return fs_path_equal(lhs.get_handle(), rhs.get_handle());
}

path operator/(const path& lhs, const path& rhs)
{
    return path(fs_path_cat2(lhs.get_handle(), rhs.get_handle()));
}

path absolute(const path& p)
{
    return path(fs_path_absolute(p.handle));
}

path absolute(const path& p, std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    path p(fs_path_absolute_error(p.handle, &error, &msg));
    ec.set_val(error);
    ec.set_msg(msg);
    return p;
}

path canonical(const path& p)
{
    return path(fs_path_canonical(p.handle));
}

void copy(const path& from, const path& to)
{
    fs_copy(from.handle, to.handle);
}

void copy(const path& from, const path& to, std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    fs_copy_error(from.handle, to.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
}

bool copy_file(const path& from, const path& to) 
{
    return fs_copy_file(from.handle, to.handle);
}

bool copy_file(const path& from, const path& to, std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_copy_file_error(from.handle, to.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

bool create_directories(const path& p)
{
    return fs_create_directories(p.handle);
}

bool create_directories(const path& p, std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_create_directories_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

path current_path()
{
    return path(fs_current_path());
}

path current_path(std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    path p(fs_current_path_error(&error, &msg));
    ec.set_val(error);
    ec.set_msg(msg);
    return p;
}

bool current_path(const path& p)
{
    return fs_set_current_path(p.handle);
}

void current_path(const path& p, std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    fs_set_current_path_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
}

bool equivalent(const path& p1, const path& p2)
{
    return fs_equivalent(p1.handle, p2.handle);
}

bool equivalent(const path& p1, const path& p2, std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_equivalent_error(p1.handle, p2.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

bool exists(const path& p) noexcept
{
    return fs_exists(p.handle);
}

bool exists(const path& p, std::error_code& ec) noexcept
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_exists_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

uint64_t file_size(const path& p) noexcept
{
    return fs_file_size(p.handle);
}

uint64_t file_size(const path& p, std::error_code& ec) noexcept
{
    int error = 0;
    char* msg = nullptr;
    uint64_t retval = fs_file_size_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

bool is_block_file(const path& p) noexcept
{
    return fs_is_block_file(p.handle);
}

bool is_block_file(const path& p, std::error_code& ec) noexcept
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_is_block_file_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

bool is_character_file(const path& p) noexcept
{
    return fs_is_character_file(p.handle);
}

bool is_character_file(const path& p, std::error_code& ec) noexcept
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_is_character_file_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

bool is_directory(const path& p) noexcept
{
    return fs_is_directory(p.handle);
}

bool is_directory(const path& p, std::error_code& ec) noexcept
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_is_directory_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

bool is_empty(const path& p) noexcept
{
    return fs_is_empty(p.handle);
}

bool is_empty(const path& p, std::error_code& ec) noexcept
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_is_empty_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

bool is_fifo(const path& p) noexcept
{
    return fs_is_fifo(p.handle);
}

bool is_fifo(const path& p, std::error_code& ec) noexcept
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_is_fifo_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

bool is_other(const path& p) noexcept
{
    return fs_is_other(p.handle);
}

bool is_other(const path& p, std::error_code& ec) noexcept
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_is_other_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

bool is_regular_file(const path& p) noexcept
{
    return fs_is_regular_file(p.handle);
}

bool is_regular_file(const path& p, std::error_code& ec) noexcept
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_is_regular_file_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

bool is_socket(const path& p) noexcept
{
    return fs_is_socket(p.handle);
}

bool is_socket(const path& p, std::error_code& ec) noexcept
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_is_socket_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

bool is_symlink(const path& p) noexcept
{
    return fs_is_symlink(p.handle);
}

bool is_symlink(const path& p, std::error_code& ec) noexcept
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_is_symlink_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

file_time_type last_write_time(const path& p)
{
    return fs_last_write_time(p.handle);
}

file_time_type last_write_time(const path& p, std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    file_time_type retval = fs_last_write_time_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

path relative(const path& p)
{
    path pth(fs_relative(p.handle));
    return pth;
}

path relative(const path& p, std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    path pth(fs_relative_error(p.handle, &error, &msg));
    ec.set_val(error);
    ec.set_msg(msg);
    return pth;
}

path relative(const path& p, const path& base)
{
    path pth(fs_relative_base(p.handle, base.handle));
    return pth;
}

path relative(const path& p, const path& base, std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    path pth(fs_relative_base_error(p.handle, base.handle, &error, &msg));
    ec.set_val(error);
    ec.set_msg(msg);
    return pth;
}

bool remove(const path& p) noexcept
{
    return fs_remove(p.handle);
}

bool remove(const path& p, std::error_code& ec)  noexcept
{
    int error = 0;
    char* msg = nullptr;
    bool retval = fs_remove_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

uint64_t remove_all(const path& p)
{
    return fs_remove_all(p.handle);
}

uint64_t remove_all(const path& p, std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    uint64_t retval = fs_remove_all_error(p.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
    return retval;
}

void rename(const path& from, const path& to)
{
    fs_rename(from.handle, to.handle);
}

void rename(const path& from, const path& to, std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    fs_rename_error(from.handle, to.handle, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
}

void resize_file(const path& p, uint64_t size)
{
    fs_resize_file(p.handle, size);
}

void resize_file(const path& p, uint64_t size, std::error_code& ec)
{
    int error = 0;
    char* msg = nullptr;
    fs_resize_file_error(p.handle, size, &error, &msg);
    ec.set_val(error);
    ec.set_msg(msg);
}

} // namespace std::filesystem

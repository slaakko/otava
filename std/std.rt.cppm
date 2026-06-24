export module std.rt;

import std.type.fundamental;

export {

using TryFn = void(*)(void*);
using HandlerFn = void(*)(void*);
using InvokeFn = void(*)(void*);
using CleanUpFn = void(*)(void*);

extern "C" void* ort_malloc(size_t size);
extern "C" void ort_free(void* ptr);
extern "C" void ort_debug_memory();
extern "C" void* ort_debug_operator_new(void* block, const char* function, const char* sourceFilePath, int line);
extern "C" void ort_io_write(int handle, const char* s);
extern "C" void* ort_open_file(const char* path, const char* mode);
extern "C" int ort_close_file(void* stream);
extern "C" int ort_fgetc(void* file);
extern "C" int ort_fputc(int ch, void* file);
extern "C" int ort_fputs(const char* str, void* file);
extern "C" bool ort_isspace(int c);
extern "C" bool ort_isalpha(int c);
extern "C" bool ort_isdigit(int c);
extern "C" bool ort_isalnum(int c);
extern "C" int ort_tolower(int c);
extern "C" int ort_toupper(int c);
extern "C" std::int64_t ort_fread(void* buf, std::int64_t size, std::int64_t count, void* file);
extern "C" std::int64_t ort_fwrite(void* buffer, std::int64_t size, std::int64_t count, void* file);
extern "C" int ort_feof(void* file);
extern "C" int ort_ferror(void* file);
extern "C" int ort_fflush(void* file);
extern "C" int ort_flush_handle(int handle);
extern "C" int ort_fseek(void* file, long offset, int origin);
extern "C" long ort_ftell(void* file);
extern "C" std::uint64_t ort_file_size(const char* filePath);
extern "C" std::int64_t ort_last_write_time(const char* filePath);
extern "C" void ort_set_last_write_time(const char* dest, const char* source);
extern "C" void ort_create_directories(const char* directoryPath);
extern "C" std::uint32_t ort_random();
extern "C" std::uint64_t ort_random64();
extern "C" std::uint8_t ort_get_random_byte();
extern "C" const char* ort_get_cwd(char* buf, int bufSize);
extern "C" int ort_error_number();
extern "C" int ort_platform_string_to_utf16_string(const char* platform_string, char16_t* buf, int bufSize);
extern "C" int ort_utf16_string_to_platform_string(const char16_t* utf16String, char* buf, int bufSize);
extern "C" char* ort_error_str(int errnum);
extern "C" const char* ort_get_env(const char* env);
extern "C" void* ort_create_mutex();
extern "C" void ort_destroy_mutex(void* mtx);
extern "C" void ort_lock_mutex(void* mtx);
extern "C" void ort_unlock_mutex(void* mtx);
extern "C" bool ort_trylock_mutex(void* mtx);
extern "C" void* ort_create_recursive_mutex();
extern "C" void ort_destroy_recursive_mutex(void* mtx);
extern "C" void ort_lock_recursive_mutex(void* mtx);
extern "C" void ort_unlock_recursive_mutex(void* mtx);
extern "C" bool ort_trylock_recursive_mutex(void* mtx);
extern "C" void* ort_create_atomic_bool();
extern "C" void ort_destroy_atomic_bool(void* a);
extern "C" bool ort_load_atomic_bool(void* a);
extern "C" void ort_store_atomic_bool(void* a, bool value);
extern "C" bool ort_is_atty(int handle);
extern "C" int64_t ort_current_time();
extern "C" void ort_current_date(int& yyyy, int& month, int& day);
extern "C" void ort_current_date_time(int& yyyy, int& month, int& day, int& seconds);
extern "C" int64_t ort_make_time(int yyyy, int month, int day, int seconds);
extern "C" int64_t ort_current_ms();
extern "C" void* ort_get_std_output_handle();
extern "C" void* ort_get_std_error_handle();
extern "C" std::uint16_t ort_get_console_attrs(void* handle);
extern "C" bool ort_set_console_text_attribute(void* handle, std::uint16_t attrs);
extern "C" void* ort_load_library_as_data_file(const char16_t* moduleName);
extern "C" void* ort_get_module_handle(const char16_t* moduleName);
extern "C" std::uint32_t ort_get_last_windows_error();
extern "C" void ort_get_windows_error_message(std::uint32_t error, char* buf, int bufSize);
extern "C" void* ort_find_resource(void* moduleHandle, const char16_t* resourceName);
extern "C" void* ort_load_resource(void* moduleHandle, void* resourceHandle);
extern "C" std::uint8_t* ort_lock_resource(void* handle);
extern "C" std::int64_t ort_size_of_resource(void* moduleHandle, void* resource);
extern "C" void ort_set_std_handles_to_utf16_mode();
extern "C" void ort_set_std_handles_to_narrow_mode();
extern "C" bool ort_is_handle_redirected(int handle);
extern "C" int ort_zlib_init(std::int32_t mode, int compressionLevel, void** handle);
extern "C" void ort_zlib_done(std::int32_t mode, void* handle);
extern "C" void ort_zlib_set_input(void* inChunk, uint32_t inAvail, void* handle);
extern "C" int32_t ort_zlib_deflate(void* outChunk, uint32_t outChunkSize, uint32_t* have, uint32_t* outAvail, void* handle, int32_t flush);
extern "C" int32_t ort_zlib_inflate(void* outChunk, uint32_t outChunkSize, uint32_t* have, uint32_t* outAvail, uint32_t* inAvail, void* handle);
extern "C" const char* ort_zlib_retval_str(int32_t retVal);
extern "C" void* ort_convert_thread_to_fiber(void* param);
extern "C" void* ort_create_fiber(std::uint64_t stackSize, void* startAddress, void* param);
extern "C" void ort_switch_to_fiber(void* fiber);
extern "C" void* ort_get_fiber_data();
extern "C" void ort_delete_fiber(void* fiber);
extern "C" std::time_t ort_time(std::time_t* arg);
extern "C" void* ort_localtime(const std::time_t* time);
extern "C" std::time_t ort_mktime(void* time);
extern "C" void ort_strftime(char* buffer, int bufSize, const char* format, void* time);
extern "C" bool ort_init_sockets();
extern "C" void ort_done_sockets();
extern "C" std::uint32_t ort_get_last_socket_error();
extern "C" std::int64_t ort_socket();
extern "C" bool ort_bind_socket(std::int64_t socket, int port);
extern "C" bool ort_listen_socket(std::int64_t socket, int backlog);
extern "C" std::int64_t ort_accept_socket(std::int64_t socket);
extern "C" bool ort_close_socket(std::int64_t socket);
extern "C" bool ort_shutdown_socket(std::int64_t socket, int how);
extern "C" std::int64_t ort_connect_socket(const char* node, const char* service);
extern "C" int ort_send_socket(std::int64_t socket, const std::uint8_t* buf, int len, int flags);
extern "C" int ort_recv_socket(std::int64_t socket, std::uint8_t* buf, int len, int flags);
extern "C" bool ort_get_path_to_executable(char* buf, int bufSize);
extern "C" double ort_pow(double x, int exponent);
extern "C" float ort_remainder(float x, float y);
extern "C" double ort_remainderd(double x, double y);
extern "C" float ort_stof(const char* s, std::size_t* idx);
extern "C" double ort_stod(const char* s, std::size_t* idx);
extern "C" double ort_strtod(const char* str, char** str_end);
extern "C" void ort_exit(int exitCode);
extern "C" int ort_run_process(const char* commandLine);
extern "C" void ort_set_random_seed(std::uint64_t seed);
extern "C" void ort_reset_rng();
extern "C" void ort_debug_break();
extern "C" void ort_push_function(void* traceEntry);
extern "C" void ort_pop_function(void* traceEntry);
extern "C" int ort_generate_stack_trace();
extern "C" const char* ort_get_stack_trace(int traceId);
extern "C" void ort_free_stack_trace(int traceId);
extern "C" void ort_throw(void* ex, std::uint32_t ext);
extern "C" void ort_rethrow();
extern "C" void ort_try(TryFn tryFn, HandlerFn handlerFn, void* parentFrame);
extern "C" void ort_invoke(InvokeFn invokeFn, CleanUpFn cleanupFn, void* parentFrame);
extern "C" bool ort_begin_catch(std::uint32_t ext);
extern "C" void ort_end_catch();
extern "C" void ort_resume();
extern "C" void* ort_get_exception();
extern "C" void ort_print_exception_stack_trace();
extern "C" void ort_set_bad_alloc(void* ex, std::uint32_t ext);
extern "C" bool ort_is_bad_alloc(std::uint32_t ext);
extern "C" bool ort_current_ex_is_bad_alloc();
extern "C" void* ort_get_bad_alloc();
extern "C" bool ort_path_exists(const char* path);
int stdin_ = 0;
int stdout_ = 1;
int stderr_ = 2;
void* stdin;
void* stdout;
void* stderr;
int SEEK_SET = 0;
int SEEK_CUR = 1;
int SEEK_END = 2;
int EOF = -1;

} // export 

export namespace std {

inline void* malloc(size_t size) noexcept
{
    return ort_malloc(size);
}

inline void free(void* ptr) noexcept
{
    ort_free(ptr);
}

inline void* fopen(const char* path, const char* mode) noexcept
{
    return ort_open_file(path, mode);
}

inline int fclose(void* file) noexcept
{
    return ort_close_file(file);
}

inline int fgetc(void* file) noexcept
{
    return ort_fgetc(file);
}

inline int fputc(int ch, void* file) noexcept
{
    return ort_fputc(ch, file);
}

inline int fputs(const char* str, void* file) noexcept
{
    return ort_fputs(str, file);
}

inline bool isspace(int c) noexcept
{
    return ort_isspace(c);
}

inline bool isalpha(int c) noexcept
{
    return ort_isalpha(c);
}

inline bool isdigit(int c) noexcept
{
    return ort_isdigit(c);
}

inline bool isalnum(int c) noexcept
{
    return ort_isalnum(c);
}

inline int tolower(int c) noexcept
{
    return ort_tolower(c);
}

inline int toupper(int c) noexcept
{
    return ort_toupper(c);
}

inline std::int64_t fread(void* buf, std::int64_t size, std::int64_t count, void* file) noexcept
{
    return ort_fread(buf, size, count, file);
}

inline std::int64_t fwrite(void* buffer, std::int64_t size, std::int64_t count, void* file) noexcept
{
    return ort_fwrite(buffer, size, count, file);
}

inline int feof(void* file) noexcept
{
    return ort_feof(file);
}

inline int ferror(void* file) noexcept
{
    return ort_ferror(file);
}

inline int fflush(void* file) noexcept
{
    return ort_fflush(file);
}

inline int fseek(void* file, long offset, int origin) noexcept
{
    return ort_fseek(file, offset, origin);
}

inline long ftell(void* file) noexcept
{
    return ort_ftell(file);
}

inline const char* getenv(const char* env) noexcept
{
    return ort_get_env(env);
}

inline double pow(double x, int exponent) noexcept
{
    return ort_pow(x, exponent);
}

inline float remainder(float x, float y) noexcept
{
    return ort_remainder(x, y);
}

inline double remainder(double x, double y) noexcept
{
    return ort_remainderd(x, y);
}

inline void exit(int exitCode) noexcept
{
    ort_exit(exitCode);
}

struct rt_init
{
    rt_init() noexcept;
};

} // namespace std

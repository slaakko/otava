// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <cstdio>
#include <cstring>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <winsock2.h>
#include <ws2tcpip.h>    
#include <Windows.h>
#undef min
#undef max
#include <util/ZLibInterface.h>
#ifdef NDEBUG
#pragma comment(lib, "zlibstat.lib")
#else 
#pragma comment(lib, "zlibstatd.lib")
#endif
#pragma comment(lib, "ws2_32.lib")

module ort;

import class_info_index;
import ort.memory;
import util;

bool initialized = false;

void ort_flush_handle(int handle)
{
    switch (handle)
    {
    case 1:
    {
        std::fflush(stdout);
        break;
    }
    case 2:
    {
        std::fflush(stderr);
        break;
    }
    default:
    {
        std::fflush(nullptr);
        break;
    }
    }
}

class Rng
{
public:
    Rng(std::uint64_t seed_);
    std::uint8_t Get() { return dist(mt); }
private:
    std::random_device rd;
    std::uint64_t seed;
    std::mt19937_64 mt;
    std::uniform_int_distribution<> dist;
};

std::uint64_t GetSeed(std::uint64_t seed, std::random_device& rd)
{
    if (seed == -1)
    {
        return static_cast<std::uint64_t>(rd()) ^ static_cast<std::uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    }
    else
    {
        return seed;
    }
}

Rng::Rng(std::uint64_t seed_) : rd(), seed(GetSeed(seed_, rd)), mt(seed), dist(0, 255)
{
}

#ifdef _WIN32

__declspec(thread) std::uint64_t init_seed = -1;
__declspec(thread) Rng* rng = nullptr;

#else

__thread std::uint64_t init_seed = -1;
__thread Rng* rng = nullptr;

#endif

void ort_set_rand_seed(std::uint64_t seed)
{
    init_seed = seed;
}

std::uint8_t ort_get_random_byte()
{
    if (!rng)
    {
        rng = new Rng(init_seed);
    }
    return rng->Get();
}

const char* ort_get_cwd(char* buf, int bufSize)
{
    const char* wd = _getcwd(buf, bufSize);
    return wd;
}

bool debugMemory = false;

void ort_debug_memory()
{
    debugMemory = true;
}

std::string outFilePath;

void* ort_malloc(std::int64_t size)
{
    void* block = std::malloc(size);
    if (debugMemory)
    {
        ort::memory::allocate(block, size);
    }
    return block;
}

void* ort_debug_operator_new(void* block, const char* function, const char* sourceFilePath, int line)
{
    ort::memory::set_info(block, function, sourceFilePath, line);
    return block;
}

void ort_preprocess_args(int argc, const char** argv, int* pargc, const char*** pargv)
{
    std::vector<std::string> args;
    for (int i = 0; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (i > 0)
        {
            if (arg.starts_with("--"))
            {
                if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "--ort-break")
                        {
                            int serial = std::stoi(components[1]);
                            ort::memory::set_allocation_serial(serial);
                            continue;
                        }
                        else if (components[0] == "--ort-out")
                        {
                            outFilePath = components[1];
                            continue;
                        }
                    }
                }
            }
        }
        args.push_back(arg);
    }
    int n = args.size();
    int m = (n + 1) * sizeof(char*);
    int size = m;
    for (int i = 0; i < n; ++i)
    {
        size += args[i].length() + 1;
    }
    void* block = operator new(size);
    const char** v = static_cast<const char**>(block);
    char* p = static_cast<char*>(block);
    p += m;
    for (int i = 0; i < n; ++i)
    {
#pragma warning(disable : 4996)
        std::strcpy(p, args[i].c_str());
#pragma warning(default : 4996)
        v[i] = p;
        p += args[i].length() + 1;
    }
    v[n] = nullptr;
    *pargc = n;
    *pargv = v;
}

void ort_print_memory_leaks()
{
    std::ostream* s = nullptr;
    std::ofstream out;
    if (!outFilePath.empty())
    {
        out.open(outFilePath);
        s = &out;
        std::cout << "memory leaks written to file '" + outFilePath + "'" << "\n";
    }
    else
    {
        s = &std::cout;
    }
    ort::memory::print_leaks(s);
}

void ort_free(void* ptr)
{
    if (debugMemory)
    {
        bool do_free = ort::memory::free(ptr);
        if (!do_free) return;
    }
    std::free(ptr);
}

void* ort_open_file(const char* path, const char* mode)
{
#pragma warning(suppress : 4996)
    return std::fopen(path, mode);
}

int ort_close_file(void* stream)
{
    return std::fclose(static_cast<FILE*>(stream));
}

char* ort_error_str(int errnum)
{
#pragma warning(suppress : 4996)
    return std::strerror(errnum);
#pragma warning(default : 4996)
}

const char* ort_get_env(const char* env)
{
#pragma warning(suppress : 4996)
    return std::getenv(env);
#pragma warning(default : 4996)
}

int ort_error_number()
{
    return errno;
}

int ort_fgetc(void* file)
{
    return std::fgetc(static_cast<FILE*>(file));
}

int ort_fputc(int ch, void* file)
{
    return std::fputc(ch, static_cast<FILE*>(file));
}

int ort_fputs(const char* str, void* file)
{
    return std::fputs(str, static_cast<FILE*>(file));
}

std::int64_t ort_fread(void* buffer, std::int64_t size, std::int64_t count, void* file)
{
    return std::fread(buffer, size, count, static_cast<FILE*>(file));
}

std::int64_t ort_fwrite(void* buffer, std::int64_t size, std::int64_t count, void* file)
{
    return std::fwrite(buffer, size, count, static_cast<FILE*>(file));
}

bool ort_isspace(int c)
{
    return std::isspace(c);
}

bool ort_isalpha(int c)
{
    return std::isalpha(c);
}

bool ort_isdigit(int c)
{
    return std::isdigit(c);
}

bool ort_isalnum(int c)
{
    return std::isalnum(c);
}

int ort_tolower(int c)
{
    return std::tolower(c);
}

int ort_toupper(int c)
{
    return std::toupper(c);
}

int ort_feof(void* file)
{
    return std::feof(static_cast<FILE*>(file));
}

int ort_ferror(void* file)
{
    return std::ferror(static_cast<FILE*>(file));
}

int ort_fflush(void* file)
{
    return std::fflush(static_cast<FILE*>(file));
}

int ort_fseek(void* file, long offset, int origin)
{
    return std::fseek(static_cast<FILE*>(file), offset, origin);
}

long ort_ftell(void* file)
{
    return std::ftell(static_cast<FILE*>(file));
}

std::uint64_t ort_file_size(const char* filePath)
{
    try
    {
        return std::filesystem::file_size(filePath);
    }
    catch (...)
    {
        return -1;
    }
}

std::int64_t ort_last_write_time(const char* filePath)
{
    try
    {
        return static_cast<std::int64_t>(std::filesystem::last_write_time(filePath).time_since_epoch().count());
    }
    catch (...)
    {
        return -1;
    }
}

void ort_set_last_write_time(const char* dest, const char* source)
{
    try
    {
        std::filesystem::last_write_time(dest, std::filesystem::last_write_time(source));
    }
    catch (...)
    {
    }
}

void ort_create_directories(const char* directoryPath)
{
    try
    {
        std::filesystem::create_directories(directoryPath);
    }
    catch (...)
    {
    }
}

int ort_platform_string_to_utf16_string(const char* platformString, char16_t* buf, int bufSize)
{
    int result = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, platformString, -1, (LPWSTR)buf, bufSize);
    return result;
}

int ort_utf16_string_to_platform_string(const char16_t* utf16String, char* buf, int bufSize)
{
    int result = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)utf16String, -1, (LPSTR)buf, bufSize, nullptr, nullptr);
    return result;
}

std::int64_t ort_current_time()
{
    return std::time(nullptr);
}

void ort_current_date(int& yyyy, int& month, int& day)
{
    util::Date date = util::GetCurrentDate();
    yyyy = date.Year();
    month = static_cast<std::int8_t>(date.GetMonth());
    day = date.Day();
}

void ort_current_date_time(int& yyyy, int& month, int& day, int& seconds)
{
    util::DateTime dateTime = util::GetCurrentDateTime();
    util::Date date = dateTime.GetDate();
    yyyy = date.Year();
    month = static_cast<std::int8_t>(date.GetMonth());
    day = date.Day();
    seconds = dateTime.Seconds();
}

std::int64_t ort_make_time(int yyyy, int month, int day, int seconds)
{
    util::Date date(yyyy, static_cast<util::Month>(month), static_cast<std::int8_t>(day));
    util::DateTime dt(date, seconds);
    return util::MkTime(dt);
}

std::int64_t ort_current_ms()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - std::chrono::steady_clock::time_point()).count();
}

void* ort_get_std_output_handle()
{
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

void* ort_get_std_error_handle()
{
    return GetStdHandle(STD_ERROR_HANDLE);
}

std::uint16_t ort_get_console_attrs(void* handle)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(handle, &info))
    {
        return info.wAttributes;
    }
    else
    {
        return static_cast<std::uint16_t>(-1);
    }
}

bool ort_set_console_text_attribute(void* handle, std::uint16_t attrs)
{
    return SetConsoleTextAttribute(handle, attrs);
}

void* ort_load_library_as_data_file(const char16_t* moduleName)
{
    return LoadLibraryExW((LPCWSTR)moduleName, 0, LOAD_LIBRARY_AS_DATAFILE);
}

void* ort_get_module_handle(const char16_t* moduleName)
{
    return GetModuleHandleW((LPCWSTR)moduleName);
}

std::uint32_t ort_get_last_windows_error()
{
    return GetLastError();
}

std::uint32_t ort_get_last_socket_error()
{
    return WSAGetLastError();
}

void ort_get_windows_error_message(std::uint32_t error, char* buf, int bufSize)
{
    std::unique_ptr<char16_t[]> buffer(new char16_t[bufSize * 2]);
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, static_cast<DWORD>(error), LANG_SYSTEM_DEFAULT, (LPWSTR)buffer.get(), 4096, nullptr);
    std::string s = util::ToUtf8(buffer.get());
    std::string::size_type n = s.copy(buf, std::min(static_cast<int>(s.length()), bufSize - 1));
    buf[n] = '\0';
}

void* ort_find_resource(void* moduleHandle, const char16_t* resourceName)
{
    void* resource = FindResourceW((HMODULE)moduleHandle, (LPWSTR)resourceName, RT_RCDATA);
    return resource;
}

void* ort_load_resource(void* moduleHandle, void* resource)
{
    HGLOBAL handle = LoadResource((HMODULE)moduleHandle, (HRSRC)resource);
    return handle;
}

std::uint8_t* ort_lock_resource(void* handle)
{
    return static_cast<std::uint8_t*>(LockResource(handle));
}

std::int64_t ort_size_of_resource(void* moduleHandle, void* resource)
{
    return SizeofResource((HMODULE)moduleHandle, (HRSRC)resource);
}

void ort_set_std_handles_to_utf16_mode()
{
    _setmode(0, _O_U16TEXT);
    _setmode(1, _O_U16TEXT);
    _setmode(2, _O_U16TEXT);
}

void ort_set_std_handles_to_narrow_mode()
{
    _setmode(0, _O_TEXT);
    _setmode(1, _O_TEXT);
    _setmode(2, _O_TEXT);
}

bool ort_is_handle_redirected(int handle)
{
    return !_isatty(handle);
}

int ort_zlib_init(std::int32_t mode, int compressionLevel, void** handle)
{
    return zlib_init(mode, compressionLevel, handle);
}

void ort_zlib_done(std::int32_t mode, void* handle)
{
    zlib_done(mode, handle);
}

void ort_zlib_set_input(void* inChunk, uint32_t inAvail, void* handle)
{
    zlib_set_input(inChunk, inAvail, handle);
}

int32_t ort_zlib_deflate(void* outChunk, uint32_t outChunkSize, uint32_t* have, uint32_t* outAvail, void* handle, int32_t flush)
{
    return zlib_deflate(outChunk, outChunkSize, have, outAvail, handle, flush);
}

int32_t ort_zlib_inflate(void* outChunk, uint32_t outChunkSize, uint32_t* have, uint32_t* outAvail, uint32_t* inAvail, void* handle)
{
    return zlib_inflate(outChunk, outChunkSize, have, outAvail, inAvail, handle);
}

const char* ort_zlib_retval_str(int32_t retVal)
{
    return zlib_retval_str(retVal);
}

void* ort_convert_thread_to_fiber(void* param)
{
    return ::ConvertThreadToFiber(param);
}

void* ort_create_fiber(std::uint64_t stackSize, void* startAddress, void* param)
{
    return ::CreateFiber(stackSize, (LPFIBER_START_ROUTINE)startAddress, param);
}

void ort_switch_to_fiber(void* fiber)
{
    ::SwitchToFiber(fiber);
}

void* ort_get_fiber_data()
{
    return ::GetFiberData();
}

void ort_delete_fiber(void* fiber)
{
    ::DeleteFiber(fiber);
}

std::time_t ort_time(std::time_t* arg)
{
    std::time_t t = std::time(arg);
    return t;
}

void* ort_localtime(const std::time_t* time)
{
#pragma warning(disable : 4996)
    std::tm* t = std::localtime(time);
#pragma warning(default : 4996)
    return t;
}

std::time_t ort_mktime(void* time)
{
    return std::mktime(static_cast<std::tm*>(time));
}

void ort_strftime(char* buffer, int bufSize, const char* format, void* time)
{
    std::strftime(buffer, bufSize, format, static_cast<std::tm*>(time));
}

bool ort_init_sockets()
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(ver, &wsaData) != 0)
    {
        return false;
    }
    return true;
}

void ort_done_sockets()
{
    WSACleanup();
}

std::int64_t ort_socket()
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET)
    {
        return -1;
    }
    return std::int64_t(s);
}

bool ort_bind_socket(std::int64_t socket, int port)
{
    SOCKET s = static_cast<SOCKET>(socket);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
#pragma warning(suppress : 4996)
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
#pragma warning(default : 4996)
    addr.sin_port = htons(port);
    int result = bind(s, (struct sockaddr*)&addr, sizeof(addr));
    if (result != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool ort_listen_socket(std::int64_t socket, int backlog)
{
    SOCKET s = static_cast<SOCKET>(socket);
    int result = listen(s, backlog);
    if (result != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::int64_t ort_accept_socket(std::int64_t socket)
{
    SOCKET s = static_cast<SOCKET>(socket);
    SOCKET a = accept(s, NULL, NULL);
    if (a == INVALID_SOCKET)
    {
        return std::int64_t(-1);
    }
    else
    {
        return std::int64_t(a);
    }
}

bool ort_close_socket(std::int64_t socket)
{
    SOCKET s = static_cast<SOCKET>(socket);
    int result = closesocket(s);
    if (result != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool ort_shutdown_socket(std::int64_t socket, int how)
{
    SOCKET s = static_cast<SOCKET>(socket);
    int result = shutdown(s, how);
    if (result != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::int64_t ort_connect_socket(const char* node, const char* service)
{
    struct addrinfo hint;
    struct addrinfo* rp;
    struct addrinfo* res;
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_flags = 0;
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = IPPROTO_TCP;
    hint.ai_addrlen = 0;
    hint.ai_addr = 0;
    hint.ai_canonname = 0;
    hint.ai_next = 0;
    int result = getaddrinfo(node, service, &hint, &res);
    if (result != 0)
    {
        return std::int64_t(-1);
    }
    else
    {
        for (rp = res; rp != 0; rp = rp->ai_next)
        {
            SOCKET s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (s == -1)
            {
                continue;
            }
            int result = connect(s, rp->ai_addr, (int)rp->ai_addrlen);
            if (result == 0)
            {
                freeaddrinfo(res);
                return static_cast<std::int64_t>(s);
            }
            else
            {
                return std::int64_t(-1);
            }
        }
    }
    return std::int64_t(-1);
}

int ort_send_socket(std::int64_t socket, const std::uint8_t* buf, int len, int flags)
{
    SOCKET s = static_cast<SOCKET>(socket);
    return send(s, (const char*)buf, len, flags);
}

int ort_recv_socket(std::int64_t socket, std::uint8_t* buf, int len, int flags)
{
    SOCKET s = static_cast<SOCKET>(socket);
    return recv(s, (char*)buf, len, flags);
}

bool ort_get_path_to_executable(char* buf, int bufSize)
{
    DWORD result = GetModuleFileNameA(NULL, buf, bufSize);
    if (result == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

double ort_pow(double x, int exponent)
{
    return std::pow(x, exponent);
}

float ort_remainder(float x, float y)
{
    return std::remainder(x, y);
}

double ort_remainderd(double x, double y)
{
    return std::remainder(x, y);
}

float ort_stof(const char* s, std::size_t* pos)
{
    return std::stof(s, pos);
}

double ort_stod(const char* s, std::size_t* pos)
{
    return std::stod(s, pos);
}

double ort_strtod(const char* str, char** str_end)
{
    return std::strtod(str, str_end);
}

void ort_exit(int exitCode)
{
    std::exit(exitCode);
}

int ort_run_process(const char* commandLine)
{
    try
    {
        return util::RunProcess(commandLine);
    }
    catch (const std::exception&)
    {
        return 999;
    }
}

void ort_debug_break()
{
    __debugbreak();
}

bool ort_path_exists(const char* path)
{
    return std::filesystem::exists(path);
}

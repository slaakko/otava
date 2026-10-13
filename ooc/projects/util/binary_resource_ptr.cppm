export module util.binary_resource_ptr;

import std;

export namespace util {

enum class ResourceFlags
{
    none = 0, loadLibraryAsDataFile = 1 << 0
};

constexpr ResourceFlags operator|(ResourceFlags left, ResourceFlags right) noexcept
{
    return ResourceFlags(int(left) | int(right));
}

constexpr ResourceFlags operator&(ResourceFlags left, ResourceFlags right) noexcept
{
    return ResourceFlags(int(left) & int(right));
}

constexpr ResourceFlags operator~(ResourceFlags flags) noexcept
{
    return ResourceFlags(~int(flags));
}

class BinaryResourcePtr
{
public:
    BinaryResourcePtr(const std::string& moduleName, const std::string& resourceName_);
    BinaryResourcePtr(const std::string& moduleName, const std::string& resourceName_, ResourceFlags flags);
    inline const std::string& ResourceName() const noexcept { return resourceName; }
    inline std::uint8_t* Data() const noexcept { return data; }
    inline std::int64_t Size() const noexcept { return size; }
    inline bool Valid() const noexcept { return error == 0; }
    inline int Error() const noexcept { return error; }
    inline explicit operator bool() const noexcept { return Valid(); }
private:
    std::string resourceName;
    std::uint8_t* data;
    std::int64_t size;
    int error;
};

} // util

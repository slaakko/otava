// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.uuid;

import std;

export namespace util {

struct uuid
{
    using value_type = std::uint8_t;
    uuid() noexcept;
    uuid(const uuid& that) noexcept;
    uuid& operator=(const uuid& that) noexcept;
    uuid(uuid&& that) noexcept;
    uuid& operator=(uuid&& that) noexcept;
    static uuid random() noexcept;
    static constexpr int static_size() noexcept { return 16; }
    inline const std::uint8_t* begin() const noexcept { return &data[0]; }
    inline const std::uint8_t* end() const noexcept { return &data[static_size()]; }
    inline std::uint8_t* begin() noexcept { return &data[0]; }
    inline std::uint8_t* end() noexcept { return &data[static_size()]; }
    bool is_nil() const noexcept;
    std::uint8_t data[16];
};

bool operator==(const uuid& left, const uuid& right) noexcept;
inline bool operator!=(const uuid& left, const uuid& right) noexcept { return !(left == right); }
bool operator<(const uuid& left, const uuid& right) noexcept;

void UuidToInts(const uuid& id, std::uint64_t& int1, std::uint64_t& int2) noexcept;
void IntsToUuid(std::uint64_t int1, std::uint64_t int2, uuid& id) noexcept;
void RandomUuid(uuid& id) noexcept;
std::string ToString(const uuid& uuid);
uuid ParseUuid(const std::string& str);
uuid ToUuid(const std::u32string& str);

inline uuid nil_uuid() noexcept
{
    return uuid();
}

inline uuid random_uuid() noexcept
{
    return uuid::random();
}

void Rotate(uuid& id, int index) noexcept;
void Xor(uuid& id, const uuid& that) noexcept;

} // namespace util

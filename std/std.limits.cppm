export module std.limits;

export namespace std {

constexpr int8_t int8_min = -128;
constexpr int8_t int8_max = 127;
constexpr uint8_t uint8_min = 0u;
constexpr uint8_t uint8_max = 255u;
constexpr int16_t int16_min = -32768;
constexpr int16_t int16_max = 32767;
constexpr uint16_t uint16_min = 0u;
constexpr uint16_t uint16_max = 65535u;
constexpr int32_t int32_min = -2147483648;
constexpr int32_t int32_max = 2147483647;
constexpr uint32_t uint32_min = 0u;
constexpr uint32_t uint32_max = 4294967295u;
constexpr int64_t int64_min = -9223372036854775808ll;
constexpr int64_t int64_max = 9223372036854775807ll;
constexpr uint64_t uint64_min = 0u;
constexpr uint64_t uint64_max = 18446744073709551615u;

} // std

module util.rand;

namespace util {

void set_rand_seed(std::uint64_t seed) noexcept
{
    ort_set_random_seed(seed);
}

void reset_rng() noexcept
{
    ort_reset_rng();
}

std::uint8_t get_random_byte() noexcept
{
    return ort_get_random_byte();
}

std::uint32_t Random() noexcept
{
    return ort_random();
}

std::uint64_t Random64() noexcept
{
    return ort_random64();
}

} // namespace util

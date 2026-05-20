// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.rand;

namespace util {

class Rng
{
public:
    Rng(std::uint64_t seed_) noexcept;
    inline std::uint8_t Get() noexcept { return dist(mt); }
private:
    std::random_device rd;
    std::uint64_t seed;
    std::mt19937_64 mt;
    std::uniform_int_distribution<> dist;
};

std::uint64_t GetSeed(std::uint64_t seed, std::random_device& rd) noexcept
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

Rng::Rng(std::uint64_t seed_) noexcept : rd(), seed(GetSeed(seed_, rd)), mt(seed), dist(0, 255)
{
}

class Rng32
{
public:
    Rng32(std::uint64_t seed_) noexcept;
    inline std::uint32_t Get() noexcept  { return dist(mt); }
    inline std::uint64_t Seed() const { return seed; }
private:
    std::random_device rd;
    std::uint64_t seed;
    std::mt19937_64 mt;
    std::uniform_int_distribution<std::uint32_t> dist;
};

Rng32::Rng32(std::uint64_t seed_) noexcept : rd(), seed(GetSeed(seed_, rd)), mt(seed), dist(0, std::numeric_limits<std::uint32_t>::max())
{
}

class Rng64
{
public:
    Rng64(std::uint64_t seed_) noexcept;
    std::uint64_t Get() noexcept { return dist(mt); }
    std::uint64_t Seed() const noexcept { return seed; }
private:
    std::random_device rd;
    std::uint64_t seed;
    std::mt19937_64 mt;
    std::uniform_int_distribution<std::uint64_t> dist;
};

Rng64::Rng64(std::uint64_t seed_) noexcept : rd(), seed(GetSeed(seed_, rd)), mt(seed), dist(0, std::numeric_limits<std::uint64_t>::max())
{
}

thread_local std::uint64_t init_seed = -1;
thread_local Rng* rng = nullptr;
thread_local Rng32* rng32 = nullptr;
thread_local Rng64* rng64 = nullptr;

void set_rand_seed(std::uint64_t seed) noexcept
{
    init_seed = seed;
}

void reset_rng() noexcept
{
    if (rng)
    {
        delete rng;
        rng = nullptr;
    }
    if (rng32)
    {
        delete rng32;
        rng32 = nullptr;
    }
    if (rng64)
    {
        delete rng64;
        rng64 = nullptr;
    }
}

std::uint8_t get_random_byte() noexcept
{
    if (!rng)
    {
        rng = new Rng(init_seed);
    }
    return rng->Get();
}

std::uint32_t Random() noexcept
{
    if (!rng32)
    {
        rng32 = new Rng32(init_seed);
    }
    return rng32->Get();
}

std::uint64_t Random64() noexcept
{
    if (!rng64)
    {
        rng64 = new Rng64(init_seed);
    }
    return rng64->Get();
}

} // namespace util

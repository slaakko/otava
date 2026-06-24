module std.hash;

namespace std {

size_t hash_offset = 14695981039346656037ULL;
size_t hash_prime = 1099511628211ULL;

size_t hash_bytes(uint8_t* x, size_t size)
{
    size_t value = hash_offset;
    for (size_t i = 0; i < size; ++i)
    {
        value ^= static_cast<size_t>(x[i]);
        value *= hash_prime;
    }
    return value;
}

} // namespace std

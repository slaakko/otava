module std.container.hash_table;

import std.algorithm;

namespace std::detail {

int64_t hash_table_primes[] =
{
    53ll, 97ll, 193ll, 389ll, 769ll, 1543ll, 3079ll, 6151ll,
    12289ll, 24593ll, 49157ll, 98317ll, 196613ll, 393241ll, 786433ll, 1572869ll,
    3145739ll, 6291469ll, 12582917ll, 25165843ll, 50331653ll, 100663319ll, 201326611ll, 402653189ll,
    805306457ll, 1610612741ll
};

int64_t next_hash_table_prime(int64_t n)
{
    int64_t* p = std::lower_bound(hash_table_primes.begin(), hash_table_primes.end(), n);
    if (p < hash_table_primes.end())
    {
        return *p;
    }
    return *(hash_table_primes.end() - 1);
}

} // namespace std

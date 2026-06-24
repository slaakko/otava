export module std.container.unordered_set;

import std.type.fundamental;
import std.functional;
import std.utilities.pair;
import std.container.hash_table;

export namespace std {

template<typename  Key, typename Hash, typename Compare>
class unordered_set
{
public:
    using key_type = Key;
    using value_type = Key;
    using hash_fn = Hash;
    using key_equal = Compare;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using table_type = hash_table<key_type, value_type, identity<value_type>, hash_fn, key_equal>;
    using size_type = typename table_type::size_type;
    using difference_type = int64_t;
    using iterator = typename table_type::iterator;
    using const_iterator = typename table_type::const_iterator;
    using local_iterator = iterator;
    using const_local_iterator = const_iterator;
    using self = std::unordered_set<key_type, value_type, hash_fn, key_equal>;

    inline unordered_set() : tbl() {}
    inline unordered_set(const self& that) : tbl(that.tbl) {}
    inline unordered_set(self&& that) noexcept : tbl(std::move(that.tbl)) {}
    inline self& operator=(const self& that) { tbl = that.tbl; return *this; }
    inline self& operator=(self&& that) noexcept { tbl = std::move(that.tbl); return *this; }

    inline iterator begin() noexcept { return tbl.begin(); }
    inline const_iterator begin() const noexcept { return tbl.begin(); }
    inline iterator end() noexcept { return tbl.end(); }
    inline const_iterator end() const noexcept { return tbl.end(); }
    inline const_iterator cbegin() const noexcept { return tbl.cbegin(); }
    inline const_iterator cend() const noexcept { return tbl.cend(); }

    inline bool empty() const noexcept { return tbl.empty(); }
    inline size_type size() const noexcept { return tbl.size(); }

    inline pair<iterator, bool> insert(const value_type& x) { return tbl.insert(x); }
    inline pair<iterator, bool> insert(value_type&& x) { return tbl.insert(x); }

    inline iterator erase(iterator position) { return tbl.erase(position); }
    inline iterator erase(const_iterator position) { return tbl.erase(position); }
    inline size_type erase(const key_type& key) { return tbl.erase(key); }
    inline iterator erase(const_iterator first, const_iterator last) { return tbl.erase(first, last); }
    inline void swap(self& that) noexcept { std::swap(tbl, that.tbl); }
    inline void clear() noexcept { tbl.clear(); }

    inline hash_fn hash_function() const noexcept { return tbl.get_hash_function(); }
    inline key_equal key_eq() const noexcept { return tbl.get_key_eq(); }

    inline iterator find(const key_type& key) noexcept { return tbl.find(key); }
    inline const_iterator find(const key_type& key) const noexcept { return tbl.find(key); }
    inline bool contains(const key_type& key) const noexcept { return tbl.contains(key); }

    inline size_type bucket_count() const noexcept { return tbl.bucket_count(); }
    inline size_type bucket_size(size_type n) const noexcept { return tbl.bucket_size(n); }
    inline size_type bucket(const key_type& k) const noexcept { return tbl.bucket(k); }

    inline float load_factor() const noexcept { return tbl.load_factor(); }
    inline float max_load_factor() const noexcept { return tbl.max_load_factor(); }
    inline void max_load_factor(float z) noexcept { return tbl.max_load_factor(z); }
private:
    table_type tbl;
};

} // namespace std 

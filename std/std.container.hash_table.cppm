export module std.container.hash_table;

import std.type.fundamental;
import std.algorithm;
import std.functional;
import std.iterator;
import std.new_delete_op;
import std.container.vector;

export namespace std::detail {

template<typename T>
class bucket
{
public:
    using value_type = T;
    using pointer = bucket<value_type>*;
    bucket(const value_type& value_, pointer next_) : val(value_), nxt(next_) {}
    inline const value_type& value() const noexcept { return val; }
    inline value_type& value() noexcept { return val; }
    inline value_type* value_ptr() const noexcept { return &val; }
    inline pointer get_next() const noexcept { return nxt; }
    inline void set_next(pointer next_) noexcept { nxt = next_; }
private:
    value_type val;
    pointer nxt;
};

int64_t next_hash_table_prime(int64_t n) noexcept;

} // namespace std::detail

namespace std {

template<typename T, typename Table>
class hash_table_iterator
{
public:
    using value_type = T;
    using pointer = value_type*;
    using table_type = Table;
    using self = hash_table_iterator<value_type, table_type>;
    using iterator_category = std::forward_iterator_tag;

    inline hash_table_iterator() noexcept : tbl(nullptr), bkt(nullptr) {}
    inline hash_table_iterator(table_type* table_, std::detail::bucket<value_type>* bucket_) noexcept : tbl(table_), bkt(bucket_) {}
    inline value_type& operator*() noexcept { return bkt->value(); }
    inline pointer operator->() noexcept { return bkt->value_ptr(); }
    self& operator++() noexcept
    {
        std::detail::bucket<value_type>* old = bkt;
        bkt = bkt->get_next();
        if (!bkt)
        {
            int64_t index = tbl->bucket_index(old->value());
            ++index;
            int64_t n = tbl->bucket_count();
            while (!bkt && index < n)
            {
                bkt = tbl->get_bucket(index);
                ++index;
            }
            if (bkt == old)
            {
                bkt = nullptr;
            }
        }
        return *this;
    }
    self operator++(int) noexcept
    {
        self it = *this;
        ++*this;
        return it;
    }
    inline std::detail::bucket<value_type>* get_bucket() const noexcept { return bkt; }
private:
    table_type* tbl;
    std::detail::bucket<value_type>* bkt;
};

template<typename T, typename Table>
constexpr bool operator==(const hash_table_iterator<T, Table>& left, const hash_table_iterator<T, Table>& right) noexcept
{
    return left.get_bucket() == right.get_bucket();
}

template<typename Key, typename Value, typename KeyOfValue, typename Hash = std::hash<Key>, typename Compare = std::equal_to<Key>>
class hash_table
{
public:
    using key_type = Key;
    using value_type = Value;
    using key_of_function = KeyOfValue;
    using hash_fn = Hash;
    using key_equal = Compare;
    using size_type = size_t;
    using self = hash_table<key_type, value_type, key_of_function, hash_fn, key_equal>;
    using iterator = hash_table_iterator<value_type, self>;
    using const_iterator = hash_table_iterator<value_type, self>;
    using bucket_ptr = std::detail::bucket<value_type>*;

    hash_table() : buckets(), count(0), loadf(0.0f), max_loadf(0.8f), key_of_value(), hash_function(), key_eq() {}
    hash_table(const hash_table& that) : buckets(), count(0), loadf(that.loadf), max_loadf(that.max_loadf),
        key_of_value(that.key_of_value), hash_function(that.hash_function), key_eq(that.key_eq)
    {
        copy_from(that);
    }
    hash_table(self&& that) noexcept : buckets(std::move(that.buckets)), count(that.count), loadf(that.loadf), max_loadf(that.max_loadf),
        key_of_value(that.key_of_value), hash_function(that.hash_function), key_eq(that.key_eq)
    {
        that.count = 0;
    }
    ~hash_table() { clear(); }
    hash_table& operator=(const hash_table& that) { clear(); copy_from(that); return *this; }
    hash_table& operator=(hash_table&& that) noexcept
    {
        std::swap(buckets, that.buckets);
        std::swap(count, that.count);
        std::swap(loadf, that.loadf);
        std::swap(max_loadf, that.max_loadf);
        std::swap(key_of_value, that.key_of_value);
        std::swap(hash_function, that.hash_function);
        std::swap(key_eq, that.key_eq);
        return *this;
    }
    inline size_type size() const noexcept { return count; }
    inline bool empty() const noexcept { return count == 0; }
    void clear() noexcept
    {
        std::vector<bucket_ptr>::size_type n = buckets.size();
        for (std::vector<bucket_ptr>::size_type i = 0; i < n; ++i)
        {
            bucket_ptr bkt = buckets[i];
            while (bkt)
            {
                bucket_ptr nxt = bkt->get_next();
                delete bkt;
                bkt = nxt;
            }
            buckets[i] = nullptr;
        }
        count = 0;
    }
    inline iterator begin() noexcept { return iterator(this, first_bucket()); }
    inline const_iterator begin() const noexcept { return const_iterator(this, first_bucket()); }
    inline const_iterator cbegin() const noexcept { return const_iterator(this, first_bucket()); }
    inline iterator end() noexcept { return iterator(this, static_cast<bucket_ptr>(nullptr)); }
    inline const_iterator end() const noexcept { return const_iterator(this, static_cast<bucket_ptr>(nullptr)); }
    inline const_iterator cend() const noexcept { return const_iterator(this, static_cast<bucket_ptr>(nullptr)); }
    inline size_type bucket_count() const noexcept { return buckets.size(); }
    inline size_type bucket_index(const value_type& value) noexcept { return do_hash(key_of(value)); }
    inline bucket_ptr get_bucket(size_type index) const noexcept { return buckets[index]; }
    size_type bucket_size(size_type n) const noexcept
    {
        size_type sz = 0;
        bucket_ptr bkt = buckets[n];
        while (bkt)
        {
            ++sz;
            bkt = bkt->get_next();
        }
        return sz;
    }
    inline size_type bucket(const key_type& k) noexcept { return do_hash(k); }
    inline hash_fn get_hash_function() const noexcept { return hash_function; }
    inline key_equal get_key_eq() const noexcept { return key_eq; }
    inline float load_factor() const noexcept { return loadf; }
    inline float max_load_factor() const noexcept { return max_loadf; }
    inline void max_load_factor(float z) noexcept { max_loadf = z; }
    pair<iterator, bool> insert(const value_type& value)
    {
        if (buckets.empty())
        {
            buckets.resize(std::detail::next_hash_table_prime(0));
        }
        const key_type& key = key_of(value);
        size_type index = do_hash(key);
        bucket_ptr bkt = buckets[index];
        while (bkt)
        {
            if (keys_equal(key, key_of(bkt->value())))
            {
                return make_pair(iterator(this, bkt), false);
            }
            bkt = bkt->get_next();
        }
        bkt = new std::detail::bucket<value_type>(value, buckets[index]);
        buckets[index] = bkt;
        ++count;
        set_load_factor();
        check_for_rehash();
        return make_pair(iterator(this, bkt), true);
    }
    iterator erase(iterator position)
    {
        iterator it = end();
        bucket_ptr bkt = position.get_bucket();
        if (bkt)
        {
            size_type index = do_hash(key_of(bkt->value()));
            if (index == -1) return end();
            bucket_ptr b = buckets[index];
            bucket_ptr prev = nullptr;
            while (b != bkt && b != nullptr)
            {
                prev = b;
                b = b->get_next();
            }
            if (b == bkt)
            {
                if (prev)
                {
                    prev->set_next(b->get_next());
                }
                else
                {
                    buckets[index] = b->get_next();
                }
                if (bkt->get_next())
                {
                    it = iterator(this, bkt->get_next());
                }
                else 
                {
                    ++index;
                    while (index < size_type(buckets.size()))
                    {
                        if (buckets[index])
                        {
                            it = iterator(this, buckets[index]);
                            break;
                        }
                        ++index;
                    }
                }
                delete bkt;
                --count;
                set_load_factor();
            }
        }
        return it;
    }
    iterator erase(const_iterator position)
    {
        iterator it = end();
        bucket_ptr bkt = position.get_non_const_bucket();
        if (bkt)
        {
            size_type index = do_hash(key_of(bkt->value()));
            if (index == -1) return end();
            bucket_ptr b = buckets[index];
            bucket_ptr prev = nullptr;
            while (b != bkt && b != nullptr)
            {
                prev = b;
                b = b->get_next();
            }
            if (b == bkt)
            {
                if (prev)
                {
                    prev->set_next(b->get_next());
                }
                else
                {
                    buckets[index] = b->get_next();
                }
                if (bkt->get_next())
                {
                    it = iterator(this, bkt->get_next());
                }
                else
                {
                    ++index;
                    while (index < size_type(buckets.size()))
                    {
                        if (buckets[index])
                        {
                            it = iterator(this, buckets[index]);
                            break;
                        }
                        ++index;
                    }
                }
                delete bkt;
                --count;
                set_load_factor();
            }
        }
        return it;
    }
    iterator erase(const_iterator first, const_iterator last)
    {
        iterator it = end();
        while (first != last)
        {
            const_iterator nxt = first;
            ++nxt;
            it = erase(first);
            first = nxt;
        }
        return it;
    }
    size_type erase(const key_type& key)
    {
        iterator it = find(key);
        if (it != end())
        {
            erase(it);
            return 1;
        }
        return 0;
    }
    iterator find(const key_type& key) noexcept
    {
        size_type index = do_hash(key);
        if (index >= 0)
        {
            bucket_ptr bkt = buckets[index];
            while (bkt)
            {
                if (keys_equal(key_of(bkt->value()), key))
                {
                    return iterator(this, bkt);
                }
                bkt = bkt->get_next();
            }
        }
        return end();
    }
    const_iterator find(const key_type& key) const noexcept
    {
        size_type index = do_hash(key);
        if (index >= 0)
        {
            bucket_ptr bkt = buckets[index];
            while (bkt)
            {
                if (keys_equal(key_of(bkt->value()), key))
                {
                    return const_iterator(this, bkt);
                }
                bkt = bkt->get_next();
            }
        }
        return cend();
    }
    inline bool contains(const key_type& key) const noexcept { return find(key) != cend(); }
private:
    inline const key_type& key_of(const value_type& value) const noexcept
    {
        return key_of_value(value);
    }
    inline bool keys_equal(const key_type& key1, const key_type& key2) const noexcept
    {
        return key_eq(key1, key2);
    }
    inline size_type do_hash(const key_type& key) const noexcept
    {
        if (buckets.empty()) return size_type(-1);
        return hash_function(key) % size_type(buckets.size());
    }
    bucket_ptr first_bucket() noexcept
    {
        bucket_ptr bkt = nullptr;
        std::vector<bucket_ptr>::size_type n = buckets.size();
        for (std::vector<bucket_ptr>::size_type i = 0; i < n; ++i)
        {
            bkt = buckets[i];
            if (bkt) break;
        }
        return bkt;
    }
    void copy_from(const self& that)
    {
        std::vector<bucket_ptr>::size_type n = that.buckets.size();
        buckets.resize(n);
        for (std::vector<bucket_ptr>::size_type i = 0; i < n; ++i)
        {
            bucket_ptr bkt = that.buckets[i];
            while (bkt)
            {
                buckets[i] = new std::detail::bucket<value_type>(bkt->value(), buckets[i]);
                bkt = bkt->get_next();
            }
        }
        count = that.count;
        loadf = that.loadf;
        max_loadf = that.max_loadf;
    }
    void set_load_factor() noexcept
    {
        size_type n = buckets.size();
        if (n == 0)
        {
            loadf = 1.0f;
        }
        else
        {
            float c = count;
            float nf = static_cast<uint32_t>(n);
            loadf = c / nf;
        }
    }
    void check_for_rehash()
    {
        if (loadf > max_loadf)
        {
            rehash();
        }
    }
    void rehash()
    {
        std::vector<bucket_ptr> b;
        std::swap(buckets, b);
        size_type n = b.size();
        buckets.resize(std::detail::next_hash_table_prime(n + 1));
        for (size_type i = 0; i < n; ++i)
        {
            bucket_ptr bkt = b[i];
            while (bkt)
            {
                const key_type& key = key_of(bkt->value());
                size_type index = do_hash(key);
                bucket_ptr nxt = bkt->get_next();
                bkt->set_next(buckets[index]);
                buckets[index] = bkt;
                bkt = nxt;
            }
        }
    }
    std::vector<bucket_ptr> buckets;
    int64_t count;
    float loadf;
    float max_loadf;
    key_of_function key_of_value;
    hash_fn hash_function;
    key_equal key_eq;
};

} // namespace std

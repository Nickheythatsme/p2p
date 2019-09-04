#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "hash.hpp"
#include <memory>
#include <vector>
#include <iterator>
#define HASH_TABLE_DEFAULT_SIZE 2048

namespace p2p {
namespace util {

class hash_table_exception : public std::exception
{
    public:
        explicit hash_table_exception (const char *what_arg) :
            std::exception (),
            what_arg (what_arg)
        {
        }
        [[nodiscard]] const char *what () const noexcept override
        {
            return what_arg;
        }
        hash_table_exception &operator= (const exception &other) noexcept
        {
            what_arg = other.what ();
            return *this;
        }
    private:
        const char *what_arg;
};


template<class K, class V>
class Entries
{
    public:
        V& get(const K &key);
        const V& get(const K &key) const;
        bool contains(const K &key) const;
        Entries<K,V>& put(K&& key, V&& value);
        Entries<K,V>& remove(const K& key);
        auto begin() const;
        auto end() const;
    private:
        std::vector<std::pair<K,V>> entries;
};

template<class K, class V>
class HashTable
{
    public:
        HashTable ();
        explicit HashTable (size_t len);
        HashTable (HashTable<K, V> &&rhs) noexcept;
        HashTable (const HashTable<K, V> &rhs);
        ~HashTable () = default;
        HashTable<K, V> &put (K key, V value);
        const V &operator[](const K &key) const;
        V &operator[](const K &key);
        V &get (const K &key);
        const V &get (const K &key) const;
        bool contains (const K &key) const;
        HashTable<K,V>& remove (const K &key);
    protected:
        size_t len;
        std::unique_ptr<Entries<K,V>[]> table;
    private:
};

} // namespace util
} // namespace p2p

#include "hash_table.cpp"
#endif // HASH_TABLE_H_

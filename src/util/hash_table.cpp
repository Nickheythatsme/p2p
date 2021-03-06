#include "hash_table.h"

namespace p2p {
namespace util {

template<class K, class V>
auto Entries<K, V>::begin() const
{
    return entries.begin();
}

template<class K, class V>
auto Entries<K, V>::end() const
{
    return entries.begin();
}

template<class K, class V>
V &Entries<K, V>::get(const K &key)
{
    for(auto &pair : entries) {
        if(pair.first == key) {
            return pair.second;
        }
    }
    throw hash_table_exception("Entry not found");
}

template<class K, class V>
const V &Entries<K, V>::get(const K &key) const
{
    for(const auto &pair : entries) {
        if(pair.first == key) {
            return pair.second;
        }
    }
    throw hash_table_exception("Entry not found");
}
template<class K, class V>
bool Entries<K, V>::contains(const K &key) const
{
    for(auto &pair : entries) {
        if(pair.first == key) {
            return true;
        }
    }
    return false;
}

template<class K, class V>
Entries <K, V> &Entries<K, V>::put(K &&key, V &&value)
{
    entries.emplace_back(
    std::move(key), std::move(value)
    );
    return *this;
}

template<class K, class V>
Entries <K, V> &Entries<K, V>::remove(const K &key)
{
    for(auto current = entries.begin(); current < entries.end(); ++current) {
        if((*current).first == key) {
            entries.erase(current);
            return *this;
        }
    }
    return *this;
}

template<class K, class V>
HashTable<K, V>::HashTable():
len(HASH_TABLE_DEFAULT_SIZE),
table(new Entries<K, V>[len])
{
}

template<class K, class V>
HashTable<K, V>::HashTable(size_t len):
len(len),
table(new Entries<K, V>[len])
{
}

template<class K, class V>
HashTable<K,V>::HashTable(const HashTable<K, V> &rhs):
    len(rhs.len),
    table(new Entries<K, V>[len])
{
    for (int i=0; i<len; ++i) {
        this->table[i] = rhs.table[i];
    }
}


template<class K, class V>
HashTable<K, V>::HashTable(HashTable &&rhs) noexcept :
len(rhs.len),
table(std::move(rhs.table))
{
    rhs.len = 0;
    rhs.table = nullptr;
}

template<class K, class V>
HashTable <K, V> &HashTable<K, V>::put(K key, V value)
{
    auto index = key % this->len;
    this->table[index].put(std::move(key), std::move(value));
    return *this;
}

template<class K, class V>
V &HashTable<K, V>::get(const K &key)
{
    auto index = key % this->len;
    return table[index].get(key);
}

template<class K, class V>
const V &HashTable<K, V>::get(const K &key) const
{
    auto index = key % this->len;
    return table[index].get(key);
}

template<class K, class V>
const V &HashTable<K, V>::operator[](const K &key) const
{
    return get(key);
}

template<class K, class V>
V &HashTable<K, V>::operator[](const K &key)
{
    return get(key);
}

template<class K, class V>
bool HashTable<K, V>::contains(const K &key) const
{
    auto index = key % this->len;
    return table[index].contains(key);
}

template<class K, class V>
HashTable <K, V> &HashTable<K, V>::remove(const K &key)
{
    auto index = key % this->len;
    table[index].remove(key);
    return *this;
}

} // namespace util
} // namespace p2p

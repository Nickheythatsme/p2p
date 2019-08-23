#include "hash_table.h"
// TODO remove iostream when not debugging
#include <iostream>

namespace p2p
{

template<class K, class V>
auto Entries<K,V>::begin() const
{
    return entries.begin();
}

template<class K, class V>
auto Entries<K,V>::end() const
{
    return entries.begin();
}


template<class K, class V>
V& Entries<K,V>::get(const K &key)
{
    for (auto& pair : entries) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    throw hash_table_exception("Entry not found");
}

template<class K, class V>
const V& Entries<K,V>::get(const K &key) const
{
    for (const auto& pair : entries) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    throw hash_table_exception("Entry not found");
}
template<class K, class V>
bool Entries<K,V>::contains(const K &key) const
{
    for (auto& pair : entries) {
        if (pair.first == key) {
            return true;
        }
    }
    return false;
}

template<class K, class V>
Entries<K,V>& Entries<K,V>::put(K key, V value)
{
    entries.emplace_back(
        std::pair<K,V>(std::move(key), std::move(value))
        );
    return *this;
}

template<class K, class V>
Entries<K,V>& Entries<K,V>::remove(const K& key)
{
    for (auto& pair : entries) {
        if (pair.first == key) {
            entries.erase(pair);
        }
    }
    return *this;
}

template<class K, class V>
HashTable<K, V>::HashTable ():
    len(HASH_TABLE_DEFAULT_SIZE),
    table (new Entries<K, V>[len])
{
}

template<class K, class V>
HashTable<K, V>::HashTable (size_t len):
    len(len),
    table (new Entries<K, V>[len])
{
}

template<class K, class V>
HashTable<K, V>::HashTable (HashTable &&rhs) noexcept :
    len (rhs.len),
    table (std::move (rhs.table))
{
    rhs.len = 0;
    rhs.table = nullptr;
}

template<class K, class V>
HashTable <K, V> &HashTable<K, V>::put (K key, V value)
{
    auto index = key % this->len;
    this->table[index].put(std::move(key), std::move(value));
    return *this;
}

template<class K, class V>
V &HashTable<K, V>::get (const K &key)
{
    auto index = key % this->len;
    return table[index].get(key);
}

template<class K, class V>
const V &HashTable<K, V>::get (const K &key) const
{
    auto index = key % this->len;
    return table[index].get(key);
}

template<class K, class V>
bool HashTable<K, V>::contains (const K &key) const
{
    auto index = key % this->len;
    return table[index].contains(key);
}

template<class K, class V>
HashTable<K,V>& HashTable<K, V>::remove (const K &key)
{
    auto index = key % this->len;
    return table[index].remove(key);
}

} // namespace p2p

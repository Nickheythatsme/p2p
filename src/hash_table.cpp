#include "hash_table.h"
// TODO remove iostream when not debugging
#include <iostream>

namespace p2p
{

template<class K, class V>
Entry<K, V>::Entry (K key, V value):
    key (std::move (key)),
    value (std::move (value))
{
}

template<class K, class V>
Entry<K, V>::Entry (Entry <K, V> &&rhs) noexcept
    :
    key (std::move (rhs.key)),
    value (std::move (rhs.value)),
    next (std::move (rhs.next))
{
}

template<class K, class V>
Entry<K, V>::Entry (const Entry <K, V> &rhs):
    key (rhs.key),
    value (rhs.value),
    next (nullptr)
{
}

template<class K, class V>
HashTable<K, V>::HashTable ():
    len (HASH_TABLE_DEFAULT_SIZE),
    table (new EntryPtr<K, V>[HASH_TABLE_DEFAULT_SIZE])
{
}

template<class K, class V>
HashTable<K, V>::HashTable (size_t len):
    len (len),
    table (new EntryPtr<K, V>[len])
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
    auto new_entry = std::make_unique<Entry<K, V>> (Entry<K, V> (std::move (key), std::move (value)));
    new_entry->next = std::move (this->table[index]);
    this->table[index] = std::move (new_entry);
    return *this;
}

template<class K, class V>
V &HashTable<K, V>::get (const K &key)
{
    auto res = _get(key);
    if (res == nullptr) {
        throw hash_table_exception ("Key was not found in table");
    } else {
        return (*res)->value;
    }
}

template<class K, class V>
const V &HashTable<K, V>::get (const K &key) const
{
    auto res = _get(key);
    if (res == nullptr) {
        throw hash_table_exception ("Key was not found in table");
    } else {
        return (*res)->value;
    }
}

template<class K, class V>
bool HashTable<K, V>::contains (const K &key) const
{
    return _get(key) != nullptr;
}

template<class K, class V>
EntryPtr<K,V>* HashTable<K,V>::_get(const K &key)
{
    auto index = key % this->len;
    if (table[index] == nullptr) {
        return nullptr;
    }
    return _get(table[index], key);
}

template<class K, class V>
EntryPtr<K,V>* HashTable<K,V>::_get(EntryPtr<K, V>& current, const K &key)
{
    if (current->key == key) {
        return &current;
    } else if (!current->next.get()) {
        return nullptr;
    } else {
        return _get(current->next, key);
    }
}

template<class K, class V>
const EntryPtr<K,V>* HashTable<K,V>::_get(const K &key) const
{
    auto index = key % this->len;
    if (table[index] == nullptr) {
        return nullptr;
    }
    return _get(table[index], key);
}

template<class K, class V>
const EntryPtr<K,V>* HashTable<K,V>::_get(const EntryPtr<K, V>& current, const K &key) const
{
    if (current->key == key) {
        return &current;
    } else if (!current->next.get()) {
        return nullptr;
    } else {
        return _get(current->next, key);
    }
}

} // namespace p2p

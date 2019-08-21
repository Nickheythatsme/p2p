#include "hash_table.h"
// TODO remove iostream when not debugging
#include <iostream>

namespace p2p {

template<class K, class V>
Entry<K, V>::Entry(K key, V value):
    key(std::move(key)),
    value(std::move(value)) {}

template<class K, class V>
HashTable<K, V>::HashTable():
    len(HASH_TABLE_DEFAULT_SIZE),
    table(new EntryPtr<K, V>[HASH_TABLE_DEFAULT_SIZE]) {
}

template<class K, class V>
HashTable<K, V>::HashTable(size_t len):
    len(len),
    table(new EntryPtr<K, V>[len]) {
}

template<class K, class V>
HashTable<K, V>::~HashTable() {
}

template<class K, class V>
HashTable<K, V> &HashTable<K, V>::put(K key, V value) {
  auto index = key % this->len;
  auto new_entry = std::make_unique<Entry<K, V>>(Entry<K, V>(std::move(key), std::move(value)));
  new_entry->next = std::move(this->table[index]);
  this->table[index] = std::move(new_entry);
  return *this;
}

template<class K, class V>
HashTable<K, V> &HashTable<K, V>::put_or_assign(K key, V value) {
  auto index = key % this->len;
  auto new_entry = std::make_unique<Entry<K, V>>(Entry<K, V>(std::move(key), std::move(value)));
  new_entry->next = std::move(this->table[index]);
  this->table[index] = std::move(new_entry);
  return *this;
}

template<class K, class V>
V &HashTable<K, V>::get(const K &key) {
  auto index = key % this->len;
  auto current = &table[index];
  while (*current) {
    if (key == (*current)->key) {
      return (*current)->value;
    }
    current = &(*current)->next;
  }
  throw hash_table_exception("Key was not found in table");
}

template<class K, class V>
const V &HashTable<K, V>::get(const K &key) const {
  auto index = key % this->len;
  auto current = &table[index];
  while (*current) {
    if (key == (*current)->key) {
      return (*current)->value;
    }
    current = &(*current)->next;
  }
  throw hash_table_exception("Key was not found in table");
}

template<class K, class V>
V &HashTable<K, V>::operator[](const K &key) {
  return this->get(key);
}

template<class K, class V>
const V &HashTable<K, V>::operator[](const K &key) const {
  return this->get(key);
}

template<class K, class V>
bool HashTable<K, V>::contains(const K &key) const {
  auto index = key % this->len;
  auto current = &table[index];
  while (*current) {
    if (key == (*current)->key) {
      return true;
    }
    current = &(*current)->next;
  }
  return false;
}

} // namespace p2p

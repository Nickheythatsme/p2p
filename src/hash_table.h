
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "hash.hpp"
#include <memory>
#define HASH_TABLE_DEFAULT_SIZE 2048

namespace p2p {

class hash_table_exception : public std::exception {
  public:
    explicit hash_table_exception(const char *what_arg) :
        std::exception(),
        what_arg(what_arg) {}
    [[nodiscard]] const char *what() const noexcept override {
      return what_arg;
    }
    hash_table_exception &operator=(const exception &other) noexcept {
      what_arg = other.what();
      return *this;
    }
  private:
    const char *what_arg;
};

template<class K, class V>
struct Entry {
  Entry(K key, V value);
  K key;
  V value;
  std::unique_ptr<Entry<K, V>> next{nullptr};
};

template<class K, class V>
using EntryPtr = std::unique_ptr<Entry<K, V>>;

template<class K, class V>
class HashTable {
  public:
    HashTable();
    HashTable(size_t len);
    ~HashTable();
    HashTable<K, V> &put(K key, V value);
    HashTable<K, V> &put_or_assign(K key, V value);
    V &operator[](const K &key);
    const V &operator[](const K &key) const;
    V &get(const K &key);
    const V &get(const K &key) const;
    bool contains(const K &key) const;
  protected:
    size_t len;
    std::unique_ptr<EntryPtr<K, V>[]> table{nullptr};
  private:
};

} // namespace p2p

#include "hash_table.cpp"
#endif // HASH_TABLE_H

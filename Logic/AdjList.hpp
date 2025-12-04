#ifndef ADJ_LIST_HPP
#define ADJ_LIST_HPP

#include <algorithm>
#include <functional>
#include <unordered_map>
#include <utility>

typedef unsigned int uint;

struct KeyEquals {
  bool operator()(const std::pair<uint, uint> &lhs,
                  const std::pair<uint, uint> &rhs) const {
    return (lhs.first == rhs.first && lhs.second == rhs.second) ||
           (lhs.first == rhs.second && lhs.second == rhs.first);
  }
};

struct KeyHasher {
  std::size_t operator()(const std::pair<uint, uint> &k) const {
    uint a = std::min(k.first, k.second);
    uint b = std::max(k.first, k.second);
    std::size_t seed = std::hash<uint>()(a);
    seed ^= std::hash<uint>()(b) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};

template <typename T> class AdjList {
public:
  AdjList() = default;

  T *find(const std::pair<uint, uint> &e) {
    auto it = table.find(e);
    return (it != table.end()) ? &it->second : nullptr;
  }

  void set(const std::pair<uint, uint> &e, const T &value) { table[e] = value; }

  bool contains(const std::pair<uint, uint> &e) const {
    return table.find(e) != table.end();
  }

  void clear() { table.clear(); }

  auto begin() { return table.begin(); }
  auto end() { return table.end(); }
  auto begin() const { return table.begin(); }
  auto end() const { return table.end(); }

private:
  std::unordered_map<std::pair<uint, uint>, T, KeyHasher, KeyEquals> table;
};

#endif

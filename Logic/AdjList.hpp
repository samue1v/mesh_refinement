#ifndef ADJ_LIST_HPP
#define ADJ_LIST_HPP

#include <functional>
#include <unordered_map>
#include <utility>

typedef unsigned int uint;

struct KeyEquals {
  bool operator()(std::pair<uint, uint> lhs, std::pair<uint, uint> rhs) const {
    return ((lhs.first == rhs.first) && (lhs.second == rhs.second)) ||
           ((lhs.first == rhs.second) && (lhs.second == rhs.first));
  }
};

struct KeyHasher {
  std::size_t operator()(const std::pair<uint, uint> &k) const {
    uint a = std::min(k.first, k.second);
    uint b = std::max(k.first, k.second);
    size_t seed = std::hash<uint>()(a);
    seed ^= std::hash<uint>()(b) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};

class AdjList {
public:
  AdjList() = default;
  char check(std::pair<uint, uint>);
  void add(std::pair<uint, uint>);
  void clear();

private:
  std::unordered_map<std::pair<uint, uint>, char, KeyHasher, KeyEquals> table;
};

#endif

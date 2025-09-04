#ifndef ADJ_LIST_HPP
#define ADJ_LIST_HPP

#include <unordered_map>
#include <functional>
#include <utility>

typedef unsigned int uint;

struct KeyEquals {
  bool operator()( std::pair<uint,uint> lhs, std::pair<uint,uint> rhs ) const
  {
    return ((lhs.first == rhs.first) && (lhs.second == rhs.second)) || ((lhs.first == rhs.second) && (lhs.second == rhs.first));
  }
};


struct KeyHasher
{
  std::size_t operator()(const std::pair<uint,uint>& k) const
  {
    return (std::hash<uint>()(k.first) ^ std::hash<uint>()(k.second));
  }
};

class AdjList{
  public:
  AdjList() = default;
  char check(std::pair<uint,uint>);
  void add(std::pair<uint,uint>);
  void clear();


  private:

  std::unordered_map<std::pair<uint,uint>,char,KeyHasher,KeyEquals> table;
};


#endif
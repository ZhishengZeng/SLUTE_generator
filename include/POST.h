// Copyright 2020 Author ChibiMarukoZ
#ifndef INCLUDE_POST_H_
#define INCLUDE_POST_H_
#include "Edge.h"
namespace slute {
class POST
{
 private:
  std::vector<Edge>             _edge_list;
  std::vector<std::vector<int>> _edge_idx_list;

 public:
  POST(std::vector<Edge>& edge_list) : _edge_list(move(edge_list))
  {
#if OBJECT_CREATE_LOG
    cout << "call class<POST> constructor.\n";
#endif
  }

  POST(const POST& other) : _edge_list(other._edge_list), _edge_idx_list(other._edge_idx_list)
  {
#if OBJECT_COPY_LOG
    cout << "call class<POST> copy constructor.\n";
#endif
  }

  POST(POST&& other)
      : _edge_list(move(other._edge_list)), _edge_idx_list(move(other._edge_idx_list))
  {
#if OBJECT_MOVE_LOG
    cout << "call class<POST> move constructor.\n";
#endif
  }

  ~POST()
  {
    _edge_list.clear();
    _edge_idx_list.clear();
#if OBJECT_DESTROY_LOG
    cout << "call class<POST> destructor.\n";
#endif
  }

  POST& operator=(const POST& other)
  {
    _edge_list     = other._edge_list;
    _edge_idx_list = other._edge_idx_list;
#if OBJECT_COPY_LOG
    cout << "call class<POST> operator=(copy).\n";
#endif
    return (*this);
  }

  POST& operator=(POST&& other)
  {
    _edge_list     = move(other._edge_list);
    _edge_idx_list = move(other._edge_idx_list);
#if OBJECT_MOVE_LOG
    cout << "call class<POST> operator=(move).\n";
#endif

    return (*this);
  }
  // getter
  std::vector<std::vector<int>>& get_edge_idx_list() { return _edge_idx_list; }
  // setter

  // function
  void initEdgeIdxList(int powv_h, int powv_v);
  void print();
};

}  // namespace slute
#endif  // INCLUDE_POST_H_

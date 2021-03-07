// Copyright 2020 Author ChibiMarukoZ
#ifndef INCLUDE_PRETREE_H_
#define INCLUDE_PRETREE_H_
#include "Edge.h"
namespace slut {
class PreTree
{
 private:
  std::vector<int>  _edge_num_list;
  std::vector<Edge> _edge_list;

 public:
  PreTree(std::vector<Edge>& edge_list) : _edge_list(move(edge_list))
  {
#if OBJECT_CREATE_LOG
    cout << "call class<PreTree> constructor.\n";
#endif
  }

  PreTree(const PreTree& other) : _edge_num_list(other._edge_num_list), _edge_list(other._edge_list)
  {
#if OBJECT_COPY_LOG
    cout << "call class<PreTree> copy constructor.\n";
#endif
  }

  PreTree(PreTree&& other)
      : _edge_num_list(move(other._edge_num_list)), _edge_list(move(other._edge_list))
  {
#if OBJECT_MOVE_LOG
    cout << "call class<PreTree> move constructor.\n";
#endif
  }

  ~PreTree()
  {
#if OBJECT_DESTROY_LOG
    cout << "call class<PreTree> destructor.\n";
#endif
  }

  PreTree& operator=(const PreTree& other)
  {
    _edge_num_list = other._edge_num_list;
    _edge_list     = other._edge_list;
#if OBJECT_COPY_LOG
    cout << "call class<PreTree> operator=(copy).\n";
#endif
    return (*this);
  }

  PreTree& operator=(PreTree&& other)
  {
    _edge_num_list = move(other._edge_num_list);
    _edge_list     = move(other._edge_list);
#if OBJECT_MOVE_LOG
    cout << "call class<PreTree> operator=(move).\n";
#endif
    return (*this);
  }
  // getter
  std::vector<int>&  get_edge_num_list() { return _edge_num_list; }
  std::vector<Edge>& get_edge_list() { return _edge_list; }
  // setter

  // function
  void createEdgeNumList(int powv_h, int powv_v);
  void print(int powv_h, int powv_v)
  {
    // print _edge_num_list
    std::cout << "          [";
    std::cout << " ";
    for (int i = 0; i < powv_h; i++) {
      std::cout << _edge_num_list[i];
      std::cout << " ";
    }
    std::cout << "/ ";
    for (int i = powv_h; i < (powv_h + powv_v); i++) {
      std::cout << _edge_num_list[i];
      std::cout << " ";
    }
    std::cout << "]";
    // print _edge_list
    std::cout << "[";
    for (size_t i = 0; i < _edge_list.size(); i++) {
      _edge_list[i].print();
    }
    std::cout << " ]\n";
  }
};

}  // namespace slut
#endif  // INCLUDE_PRETREE_H_

// Copyright 2020 Author ChibiMarukoZ
#ifndef INCLUDE_POWV_H_
#define INCLUDE_POWV_H_
#include "POST.h"
namespace slut {

class POWV
{
 private:
  std::vector<int>  _edge_num_list;
  std::vector<POST> _post_list;

 public:
  POWV(std::vector<int>& edge_num_list) : _edge_num_list(move(edge_num_list))
  {
#if OBJECT_CREATE_LOG
    cout << "call class<POWV> constructor.\n";
#endif
  }

  POWV(const POWV& other) : _edge_num_list(other._edge_num_list), _post_list(other._post_list)
  {
#if OBJECT_COPY_LOG
    cout << "call class<POWV> copy constructor.\n";
#endif
  }

  POWV(POWV&& other)
      : _edge_num_list(move(other._edge_num_list)), _post_list(move(other._post_list))
  {
#if OBJECT_MOVE_LOG
    cout << "call class<POWV> move constructor.\n";
#endif
  }

  ~POWV()
  {
#if OBJECT_DESTROY_LOG
    cout << "call class<POWV> destructor.\n";
#endif
  }

  POWV& operator=(const POWV& other)
  {
    _edge_num_list = other._edge_num_list;
    _post_list     = other._post_list;
#if OBJECT_COPY_LOG
    cout << "call class<POWV> operator=(copy).\n";
#endif
    return (*this);
  }

  POWV& operator=(POWV&& other)
  {
    _edge_num_list = move(other._edge_num_list);
    _post_list     = move(other._post_list);
#if OBJECT_MOVE_LOG
    cout << "call class<POWV> operator=(move).\n";
#endif

    return (*this);
  }
  // getter
  std::vector<int>&  get_edge_num_list() { return _edge_num_list; }
  std::vector<POST>& get_post_list() { return _post_list; }

  // setter

  // function
  void createPost(int powv_h, int powv_v, std::vector<Point>& point_list);
  void initEdgeCombList(int                                          powv_h,
                        int                                          powv_v,
                        std::vector<std::vector<std::vector<Edge>>>& edge_comb_list);
  void print(int powv_h, int powv_v);
  void printEdgeNumList(int powv_h, int powv_v);
  void printPostList(int powv_h, int powv_v);
};

}  // namespace slut
#endif  // INCLUDE_POWV_H_

// Copyright 2020 Author ChibiMarukoZ
#ifndef INCLUDE_COMB_H_
#define INCLUDE_COMB_H_
#include "POWV.h"
namespace slute {
class Comb
{
 private:
  std::ofstream* _slute_file;
  // POWV length = powv_h+powv_v;
  int    _powv_h;
  int    _powv_v;
  Point  _balance_point;
  double _h_tree_length;
  double _v_tree_length;
  // if _h_tree_length > 0 && _v_tree_length > 0 _upper_bound=min(_h_tree_length,_v_tree_length)
  double               _upper_bound;
  std::vector<Point>   _point_list;
  std::vector<PreTree> _pre_tree_list;
  std::vector<POWV>    _powv_list;

 public:
  Comb(std::ofstream* slute_file, std::vector<Point>& point_list)
      : _slute_file(slute_file),
        _powv_h(0),
        _powv_v(0),
        _h_tree_length(0),
        _v_tree_length(0),
        _point_list(move(point_list))
  {
#if OBJECT_CREATE_LOG
    cout << "call class<Comb> constructor.\n";
#endif
  }

  Comb(const Comb& other)
      : _powv_h(other._powv_h),
        _powv_v(other._powv_v),
        _balance_point(other._balance_point),
        _h_tree_length(other._h_tree_length),
        _v_tree_length(other._v_tree_length),
        _upper_bound(other._upper_bound),
        _point_list(other._point_list),
        _pre_tree_list(other._pre_tree_list),
        _powv_list(other._powv_list)
  {
#if OBJECT_COPY_LOG
    cout << "call class<Comb> copy constructor.\n";
#endif
  }

  Comb(Comb&& other)
      : _powv_h(other._powv_h),
        _powv_v(other._powv_v),
        _balance_point(move(other._balance_point)),
        _h_tree_length(other._h_tree_length),
        _v_tree_length(other._v_tree_length),
        _upper_bound(other._upper_bound),
        _point_list(move(other._point_list)),
        _pre_tree_list(move(other._pre_tree_list)),
        _powv_list(move(other._powv_list))
  {
#if OBJECT_MOVE_LOG
    cout << "call class<Comb> move constructor.\n";
#endif
  }

  ~Comb()
  {
#if OBJECT_DESTROY_LOG
    cout << "call class<Comb> destructor.\n";
#endif
  }

  Comb& operator=(const Comb& other)
  {
    _powv_h        = other._powv_h;
    _powv_v        = other._powv_v;
    _balance_point = other._balance_point;
    _h_tree_length = other._h_tree_length;
    _v_tree_length = other._v_tree_length;
    _upper_bound   = other._upper_bound;
    _point_list    = other._point_list;
    _pre_tree_list = other._pre_tree_list;
    _powv_list     = other._powv_list;
#if OBJECT_COPY_LOG
    cout << "call class<Comb> operator=(copy).\n";
#endif
    return (*this);
  }

  Comb& operator=(Comb&& other)
  {
    _powv_h        = other._powv_h;
    _powv_v        = other._powv_v;
    _balance_point = move(other._balance_point);
    _h_tree_length = other._h_tree_length;
    _v_tree_length = other._v_tree_length;
    _upper_bound   = other._upper_bound;
    _point_list    = move(other._point_list);
    _pre_tree_list = move(other._pre_tree_list);
    _powv_list     = move(other._powv_list);
#if OBJECT_MOVE_LOG

    cout << "call class<Comb> operator=(move).\n";
#endif

    return (*this);
  }

  // getter

  // setter

  // function
  void process();
  void initCombInfo();
  void initPowvInfo();
  void initUpBound();
  void initBalancePoint();
  void initHTreeLength();
  void initVTreeLength();
  void initPreTree();
  void initDegreeEdgeList(std::vector<std::vector<Edge>>& degree_edge_list);
  void createPowv();
  void reportEdgeNumList(std::vector<int>& edge_num_list, bool yes);
  void write();
  void print();
  void printCombInfo();
  void printPreTrees();
  void printPowvs();
  void destroy();
};

}  // namespace slute
#endif  // INCLUDE_COMB_H_

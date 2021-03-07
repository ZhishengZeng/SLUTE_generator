#include "POST.h"
namespace slut {
void POST::initEdgeIdxList(int powv_h, int powv_v)
{
  _edge_idx_list.resize(powv_h + powv_v);
  for (size_t i = 0; i < _edge_list.size(); i++) {
    Edge&  edge = _edge_list[i];
    Point& p1   = edge.get_p1();
    Point& p2   = edge.get_p2();
    if (p1.get_x() == p2.get_x()) {
      // x坐标相等 竖边
      _edge_idx_list[powv_h + p1.get_y()].push_back(p1.get_x());
    } else if (p1.get_y() == p2.get_y()) {
      // y坐标相等 横边
      _edge_idx_list[p1.get_x()].push_back(p1.get_y());
    } else {
      std::cout << "[ERROR] it is not a edge!\n";
    }
  }
  // 每个格子上可能有多条边
  for (size_t i = 0; i < _edge_idx_list.size(); i++) {
    std::sort(_edge_idx_list[i].begin(), _edge_idx_list[i].end());
  }
}

void POST::print()
{
  for (size_t i = 0; i < _edge_list.size(); i++) {
    std::cout << " ";
    _edge_list[i].print();
  }
}
}  // namespace slut

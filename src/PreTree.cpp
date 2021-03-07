#include "PreTree.h"
namespace slut {

void PreTree::createEdgeNumList(int powv_h, int powv_v)
{
  _edge_num_list.resize(powv_h + powv_v);
  for (size_t j = 0; j < _edge_list.size(); j++) {
    Edge&  edge = _edge_list[j];
    Point& p1   = edge.get_p1();
    Point& p2   = edge.get_p2();
    if (p1.get_y() == p2.get_y()) {
      // h上添加
      _edge_num_list[p1.get_x()]++;
    } else if (p1.get_x() == p2.get_x()) {
      // v上添加
      _edge_num_list[powv_h + p1.get_y()]++;
    } else {
      std::cout << "[ERROR] this edge is fault!" << std::endl;
    }
  }
}
}  // namespace slut

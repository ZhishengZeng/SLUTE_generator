#include "POWV.h"

#include "Util.h"
namespace slut {

void POWV::createPost(int powv_h, int powv_v, std::vector<Point>& point_list)
{
  // 初始化边组合集合
  std::vector<std::vector<std::vector<Edge>>> edge_comb_list;
  initEdgeCombList(powv_h, powv_v, edge_comb_list);

  // 初始化全零开始
  std::vector<int> idx_list;
  idx_list.resize(edge_comb_list.size());
  // 生成idx序列
  while (true) {
    // 将最后一位加至上界
    while (true) {
      std::vector<Edge> temp_list;
      for (size_t i = 0; i < idx_list.size(); i++) {
        for (size_t j = 0; j < edge_comb_list[i][idx_list[i]].size(); j++) {
          temp_list.push_back(edge_comb_list[i][idx_list[i]][j]);
        }
      }
      if (Util::filterPOST(point_list, temp_list)) {
        POST post(temp_list);
        post.initEdgeIdxList(powv_h, powv_v);
        _post_list.push_back(std::move(post));
      }

      // 若已达到上界,终止累加
      if ((idx_list[edge_comb_list.size() - 1] + 1)
          == (int) edge_comb_list[edge_comb_list.size() - 1].size()) {
        break;
      }
      idx_list[edge_comb_list.size() - 1]++;
    }
    // 找到一个未达到上界的idx
    int nub_idx;
    for (nub_idx = (idx_list.size() - 1); nub_idx >= 0; nub_idx--) {
      if ((idx_list[nub_idx] + 1) < (int) edge_comb_list[nub_idx].size()) {
        // 找到一个未达到上界 +1
        idx_list[nub_idx]++;
        // 将此上界右边的所有值设为0
        for (size_t i = (nub_idx + 1); i < idx_list.size(); i++) {
          idx_list[i] = 0;
        }
        break;
      }
    }
    // 若都已达到上界,则退出
    if (nub_idx < 0) {
      break;
    }
  }
}

void POWV::initEdgeCombList(int                                          powv_h,
                            int                                          powv_v,
                            std::vector<std::vector<std::vector<Edge>>>& edge_comb_list)
{
  for (int i = 0; i < (powv_h + powv_v); i++) {
    std::vector<Edge> source_list;
    if (i < powv_h) {
      for (int j = 0; j < (powv_v + 1); j++) {
        Point m_point(i, j);
        Point l_point(i + 1, j);
        source_list.emplace_back(m_point, l_point);
      }
    } else {
      for (int j = 0; j < (powv_h + 1); j++) {
        Point m_point(j, (i - powv_h));
        Point t_point(j, (i - powv_h + 1));
        source_list.emplace_back(m_point, t_point);
      }
    }
    std::vector<std::vector<Edge>> result_list;
    // 从n个中取k个
    int n = source_list.size();
    int k = _edge_num_list[i];
    if (n < k) {
      std::cout << "[ERROR] comb (n < k) !!" << std::endl;
    }

    // 初始化从0,1,2,3,...,(k-1)
    std::vector<int> idx_list;
    idx_list.resize(k);
    for (int j = 0; j < k; j++) {
      idx_list[j] = j;
    }

    while (true) {
      while (true) {
        std::vector<Edge> temp_list;
        for (int j = 0; j < k; j++) {
          temp_list.push_back(source_list[idx_list[j]]);
        }
        result_list.push_back(temp_list);
        if ((idx_list[k - 1] + 1) == n) {
          break;
        }
        idx_list[k - 1]++;
      }
      // 找到一个未达到上界的idx
      int nub_idx;
      for (nub_idx = (k - 1); nub_idx >= 0; nub_idx--) {
        if (idx_list[nub_idx] < (n - k + nub_idx)) {
          // 找到一个未达到上界 +1
          idx_list[nub_idx]++;
          // 将此上界右边的所有值设为逐项相加
          for (int j = (nub_idx + 1); j < k; j++) {
            idx_list[j] = idx_list[j - 1] + 1;
          }
          break;
        }
      }
      // 若都已达到上界,则退出
      if (nub_idx < 0) {
        break;
      }
    }
    edge_comb_list.push_back(result_list);
  }
}

void POWV::print(int powv_h, int powv_v)
{
  printEdgeNumList(powv_h, powv_v);
  printPostList(powv_h, powv_v);
}
void POWV::printEdgeNumList(int powv_h, int powv_v)
{
  if ((int) _edge_num_list.size() != (powv_h + powv_v)) {
    std::cout << "[WRAN] _edge_num_list.size() != (powv_h + powv_v) !!";
  }

  // print _edge_num_list
  std::cout << "          [ ";
  for (int i = 0; i < powv_h; i++) {
    std::cout << _edge_num_list[i] << " ";
  }
  std::cout << "/ ";
  for (int i = powv_h; i < (powv_h + powv_v); i++) {
    std::cout << _edge_num_list[i] << " ";
  }
  std::cout << "]" << std::endl;
}
void POWV::printPostList(int powv_h, int powv_v)
{
  // print _post_list
  if (_post_list.size() > 0) {
    std::cout << "          post_list:\n";
    for (size_t i = 0; i < _post_list.size(); i++) {
      std::cout << "              ";
      for (int i = 0; i < (powv_h + powv_v); i++) {
        std::cout << "  ";
      }
      std::cout << " [";
      _post_list[i].print();
      std::cout << " ]" << std::endl;
    }
  } else {
    std::cout << "post_list:                   null" << std::endl;
  }
}

}  // namespace slut

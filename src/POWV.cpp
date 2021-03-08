#include "POWV.h"

#include "Util.h"
namespace slut {

void POWV::createPost(int                   powv_h,
                      int                   powv_v,
                      std::vector<Point>&   point_list,
                      std::vector<PreTree>& pre_tree_list)
{
  std::vector<std::vector<Edge>> result_list;
  // 边数差异记录
  int              diff_count;
  std::vector<int> diff_num_list;
  for (size_t i = 0; i < pre_tree_list.size(); i++) {
    std::vector<Edge>& pt_edge_list     = pre_tree_list[i].get_edge_list();
    std::vector<int>&  pt_edge_num_list = pre_tree_list[i].get_edge_num_list();
    if (Util::isMappingEdgeNum(diff_count, diff_num_list, _edge_num_list, pt_edge_num_list)) {
      if (diff_count == 0) {
        // 边数完全匹配 pre_tree筛选后直接拷贝为备选post
        if (Util::filterPOST(point_list, pt_edge_list)) {
          result_list.push_back(pt_edge_list);
        }
      } else if (diff_count > 0) {
        // 边数部分匹配 先生成互补边源集合 再生成互补边组合集合 再生成备选post
        std::vector<std::vector<std::vector<Edge>>> edge_comb_list;
        initEdgeCombList(powv_h, powv_v, diff_num_list, pt_edge_list, edge_comb_list);
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
            // 加上pre_tree
            temp_list.insert(temp_list.end(), pt_edge_list.begin(), pt_edge_list.end());
            if (Util::filterPOST(point_list, temp_list)) {
              result_list.push_back(move(temp_list));
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
      } else {
        std::cout << "[ERROR] diff_s < 0 when all diff > 0" << std::endl;
      }
    }
  }
  Util::UniquePOST(result_list);
  for (size_t i = 0; i < result_list.size(); i++) {
    POST post(result_list[i]);
    post.initEdgeIdxList(powv_h, powv_v);
    _post_list.push_back(move(post));
  }
}

void POWV::initEdgeCombList(int                                          powv_h,
                            int                                          powv_v,
                            std::vector<int>&                            diff_num_list,
                            std::vector<Edge>&                           pt_edge_list,
                            std::vector<std::vector<std::vector<Edge>>>& edge_comb_list)
{
  for (int i = 0; i < (int) diff_num_list.size(); i++) {
    std::vector<Edge> source_list;
    if (diff_num_list[i] > 0) {
      if (i < powv_h) {
        for (int j = 0; j < (powv_v + 1); j++) {
          Point m_point(i, j);
          Point l_point(i + 1, j);
          Edge  edge(m_point, l_point);
          if (!Util::isExistIn(pt_edge_list, edge)) {
            source_list.push_back(move(edge));
          }
        }
      } else {
        for (int j = 0; j < (powv_h + 1); j++) {
          Point m_point(j, (i - powv_h));
          Point t_point(j, (i - powv_h + 1));
          Edge  edge(m_point, t_point);
          if (!Util::isExistIn(pt_edge_list, edge)) {
            source_list.push_back(move(edge));
          }
        }
      }

      std::vector<std::vector<Edge>> result_list;
      // 从n个中取k个
      int n = source_list.size();
      int k = diff_num_list[i];
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
          result_list.push_back(move(temp_list));
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
      edge_comb_list.push_back(move(result_list));
    }
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
    std::cout << "[INFO]           post_list:\n";
    for (size_t i = 0; i < _post_list.size(); i++) {
      std::cout << "[INFO]               ";
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

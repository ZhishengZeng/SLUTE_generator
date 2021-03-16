// Copyright 2020 Author ChibiMarukoZ
#ifndef INCLUDE_UTIL_H_
#define INCLUDE_UTIL_H_
#include "POWV.h"
namespace slute {
class Util
{
 private:
 public:
  Util()  = default;
  ~Util() = default;

  static double microtime();

  static bool comparePointASCByX(Point& a, Point& b);
  static bool comparePointASCByY(Point& a, Point& b);
  static bool comparePointListASCByX(std::vector<Point>& a_list, std::vector<Point>& b_list);
  static bool compareEqualPointList(std::vector<Point>& point_list_a,
                                    std::vector<Point>& point_list_b);
  static bool compareEdgeASCByX(Edge& a, Edge& b);
  static bool compareEdgeListASCByX(std::vector<Edge>& a_list, std::vector<Edge>& b_list);
  static bool compareEqualEdgeList(std::vector<Edge>& edge_list_a, std::vector<Edge>& edge_list_b);
  static void compressGeometryByX(std::vector<Point>& temp_list);
  static void compressGeometryByY(std::vector<Point>& temp_list);
  static bool filterPointList(std::vector<Point>& temp_list);
  static int  UniquePointList(std::vector<std::vector<Point>>& result_list);
  static bool filterEdgeList(std::vector<Edge>& temp_list);
  static int  UniqueEdgeList(std::vector<std::vector<Edge>>& result_list);
  static bool isMappingEdgeNum(int&              diff_count,
                               std::vector<int>& diff_num_list,
                               std::vector<int>& powv_edge_num_list,
                               std::vector<int>& pt_edge_num_list);
  static void countPointDegree(std::vector<std::pair<Point, int>>& point_num_list, Point& point);
  static bool isConnected(std::vector<Point>&                 given_point_list,
                          std::vector<Edge>&                  edge_list,
                          std::vector<std::pair<Point, int>>& point_num_list);
  static bool filterPOST(std::vector<Point>& point_list, std::vector<Edge>& temp_list);
  static int  UniquePOST(std::vector<std::vector<Edge>>& source_list);
  // 判断点是否在集合内
  template <typename T>
  static bool isExistIn(std::vector<T>& list, T& t)
  {
    for (size_t i = 0; i < list.size(); i++) {
      if (t == list[i]) {
        return true;
      }
    }
    return false;
  }

  /**
   * input  {}
   * output {}
   * input  {1,2,3} k=3
   * output {1,2,3}
   * input  {1,2,3,4} k=3
   * output {[1,2,3],[1,2,4],[1,3,4],[2,3,4]}
   *
   * return set<vector<T>>
   */
  template <typename T>
  static void combine(std::vector<std::vector<T>>& result_list,
                      std::vector<T>&              source_list,
                      int                          k,
                      bool (*filter)(std::vector<T>&)             = nullptr,
                      int (*unique)(std::vector<std::vector<T>>&) = nullptr)
  {
    if (source_list.size() == 0) {
      std::cout << "[WARN] source_list size is zero!!" << std::endl;
    }
    if ((int) source_list.size() < k) {
      std::cout << "[WARN] combine stop: k is greater than source_list size!!" << std::endl;
    }
    // 从n个中取k个
    int n = source_list.size();
    // 初始化从0,1,2,3,...,(k-1)
    std::vector<int> idx_list;
    idx_list.resize(k);
    for (int i = 0; i < k; i++) {
      idx_list[i] = i;
    }

    int create_num = 0;

    while (true) {
      while (true) {
        std::vector<T> temp_list;
        for (int i = 0; i < k; i++) {
          temp_list.push_back(source_list[idx_list[i]]);
        }
        // 过滤器
        if (filter != nullptr) {
          // 使用过滤器 过滤器可对每个结果进行修改以及判断是否需要放置
          if (filter(temp_list)) {
            result_list.push_back(temp_list);
            create_num++;
          }
        } else {
          result_list.push_back(temp_list);
          create_num++;
        }
        // 去重器
        int new_num = 200000;  //<! 新增数量
        if (unique != nullptr) {
          if (create_num % new_num == 0) {
            // 每新增二十万去一次重
            int delete_num = unique(result_list);
            std::cout << "[INFO] Created " << result_list.size() << " combs (add "
                      << new_num - delete_num << " combs)" << std::endl;
          }
        }
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
          for (int i = (nub_idx + 1); i < k; i++) {
            idx_list[i] = idx_list[i - 1] + 1;
          }
          break;
        }
      }
      // 若都已达到上界,则退出
      if (nub_idx < 0) {
        break;
      }
    }

    // 全部去重
    if (unique != nullptr) {
      unique(result_list);
      std::cout << "[INFO] Created " << result_list.size() << " Combs" << std::endl;
    }
  };

  /**
   * input  {}
   * output {}
   * input  {[1,2,3]}
   * output {[1],[2],[3]}
   * input  {[1,2,3],[1,2]}
   * output {[1,1],[1,2],[2,1],[2,2],[3,1],[3,2]}
   *
   * return set<vector<T>>
   */
  template <typename T, typename... ArgsFilter, typename... Args>
  static std::vector<std::vector<T>> product(std::vector<std::vector<T>> source_list,
                                             bool (*filter)(std::vector<T>&,
                                                            ArgsFilter... args_filter),
                                             Args... args)
  {
    std::vector<std::vector<T>> result_list;
    if (source_list.size() == 0) {
      std::cout << "[WARN] source_list size is zero!!" << std::endl;
      return result_list;
    }
    // 初始化全零开始
    std::vector<int> idx_list;
    idx_list.resize(source_list.size());
    // 生成idx序列
    while (true) {
      // 将最后一位加至上界
      while (true) {
        std::vector<T> temp_list;
        for (size_t i = 0; i < idx_list.size(); i++) {
          temp_list.push_back(source_list[i][idx_list[i]]);
        }
        // 过滤器
        if (filter != nullptr) {
          // 使用过滤器 过滤器可对结果进行修改
          if (filter(temp_list, args...)) {
            result_list.push_back(temp_list);
          }
        } else {
          result_list.push_back(temp_list);
        }
        // 若已达到上界,终止累加
        if ((idx_list[source_list.size() - 1] + 1)
            == (int) source_list[source_list.size() - 1].size()) {
          break;
        }
        idx_list[source_list.size() - 1]++;
      }
      // 找到一个未达到上界的idx
      int nub_idx;
      for (nub_idx = (idx_list.size() - 1); nub_idx >= 0; nub_idx--) {
        if ((idx_list[nub_idx] + 1) < (int) source_list[nub_idx].size()) {
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
    return result_list;
  };
};
}  // namespace slute
#endif  // INCLUDE_UTIL_H_

#include "Util.h"

namespace slut {

double Util::microtime()
{
  struct timeval  tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  return tv.tv_sec + tv.tv_usec / 1000000.00;
}

bool Util::comparePointASCByX(Point& a, Point& b)
{
  return a.get_x() != b.get_x() ? a.get_x() < b.get_x() : a.get_y() < b.get_y();
}
bool Util::comparePointASCByY(Point& a, Point& b)
{
  return a.get_y() != b.get_y() ? a.get_y() < b.get_y() : a.get_x() < b.get_x();
}
bool Util::comparePointListASCByX(std::vector<Point>& a_list, std::vector<Point>& b_list)
{
  if (a_list.size() == b_list.size()) {
    for (size_t i = 0; i < a_list.size(); i++) {
      if (a_list[i] != b_list[i]) {
        return comparePointASCByX(a_list[i], b_list[i]);
      }
    }
    return false;
  } else {
    return a_list.size() < b_list.size();
  }
}
bool Util::comparePointListASCByY(std::vector<Point>& a_list, std::vector<Point>& b_list)
{
  if (a_list.size() == b_list.size()) {
    for (size_t i = 0; i < a_list.size(); i++) {
      if (a_list[i] != b_list[i]) {
        return comparePointASCByY(a_list[i], b_list[i]);
      }
    }
    return false;
  } else {
    return a_list.size() < b_list.size();
  }
}
bool Util::compareEqualPointList(std::vector<Point>& point_list_a, std::vector<Point>& point_list_b)
{
  if (point_list_a.size() != point_list_b.size()) {
    return false;
  }
  for (size_t i = 0; i < point_list_a.size(); i++) {
    if (point_list_a[i] != point_list_b[i]) {
      return false;
    }
  }
  return true;
}

bool Util::compareEdgeASCByX(Edge& a, Edge& b)
{
  // 先比较p1
  if (a.get_p1() != b.get_p1()) {
    return comparePointASCByX(a.get_p1(), b.get_p1());
  } else {
    // p1相等比较p2
    return comparePointASCByX(a.get_p2(), b.get_p2());
  }
}

bool Util::compareEdgeListASCByX(std::vector<Edge>& a_list, std::vector<Edge>& b_list)
{
  if (a_list.size() == b_list.size()) {
    for (size_t i = 0; i < a_list.size(); i++) {
      if (a_list[i] != b_list[i]) {
        return compareEdgeASCByX(a_list[i], b_list[i]);
      }
    }
    return false;

  } else {
    return a_list.size() < b_list.size();
  }
}
bool Util::compareEqualEdgeList(std::vector<Edge>& edge_list_a, std::vector<Edge>& edge_list_b)
{
  if (edge_list_a.size() != edge_list_b.size()) {
    return false;
  }
  for (size_t i = 0; i < edge_list_a.size(); i++) {
    if (edge_list_a[i] != edge_list_b[i]) {
      return false;
    }
  }
  return true;
}

bool Util::comparePOWVBySumASC(POWV& powv_a, POWV& powv_b)
{
  int               sum_a = 0, sum_b = 0;
  std::vector<int>& edge_num_list_a = powv_a.get_edge_num_list();
  std::vector<int>& edge_num_list_b = powv_b.get_edge_num_list();

  for (size_t i = 0; i < edge_num_list_a.size(); i++) {
    sum_a += edge_num_list_a[i];
  }
  for (size_t i = 0; i < edge_num_list_b.size(); i++) {
    sum_b += edge_num_list_b[i];
  }
  return sum_a < sum_b;
}

void Util::compressGeometryByX(std::vector<Point>& temp_list)
{
  // reduction减少量,正数
  int base = 0, reduction = 0;
  if (temp_list[0].get_x() != 0) {
    reduction += temp_list[0].get_x();
    base = temp_list[0].get_x();
    temp_list[0].reduce_x(reduction);
  }

  for (size_t j = 1; j < temp_list.size(); j++) {
    if (base == temp_list[j].get_x()) {
      temp_list[j].reduce_x(reduction);
    } else {
      reduction += (temp_list[j].get_x() - base - 1);
      base = temp_list[j].get_x();
      temp_list[j].reduce_x(reduction);
    }
  }
}
void Util::compressGeometryByY(std::vector<Point>& temp_list)
{
  int reduction = 0, base = 0;
  if (temp_list[0].get_y() != 0) {
    reduction += temp_list[0].get_y();
    base = temp_list[0].get_y();
    temp_list[0].reduce_y(reduction);
  }

  for (size_t j = 1; j < temp_list.size(); j++) {
    if (base == temp_list[j].get_y()) {
      temp_list[j].reduce_y(reduction);
    } else {
      reduction += (temp_list[j].get_y() - base - 1);
      base = temp_list[j].get_y();
      temp_list[j].reduce_y(reduction);
    }
  }
}

void Util::compressPointList(std::vector<Point>& temp_list)
{
  std::sort(temp_list.begin(), temp_list.end(), Util::comparePointASCByX);
  Util::compressGeometryByX(temp_list);
  std::sort(temp_list.begin(), temp_list.end(), Util::comparePointASCByY);
  Util::compressGeometryByY(temp_list);
}

// filterCombs:按照x排序 x压缩 按照y排序 y压缩
bool Util::filterPointList(std::vector<Point>& temp_list)
{
  compressPointList(temp_list);
  return true;
}
// UniqueCombs:按照x排序 每新增二十万个去重一次
int Util::UniquePointList(std::vector<std::vector<Point>>& result_list)
{
  // 先排序
  for (size_t i = 0; i < result_list.size(); i++) {
    std::sort(result_list[i].begin(), result_list[i].end(), Util::comparePointASCByX);
  }
  std::sort(result_list.begin(), result_list.end(), Util::comparePointListASCByX);

  // 后去重
  int    delete_num = 0;
  size_t save_id    = 0;
  size_t sentry_id  = 0;
  size_t soldier_id = sentry_id + 1;
  while (sentry_id < result_list.size()) {
    std::vector<Point>& sentry_point_list = result_list[sentry_id];
    while (soldier_id < result_list.size()) {
      std::vector<Point>& soldier_point_list = result_list[soldier_id];
      if (!Util::compareEqualPointList(sentry_point_list, soldier_point_list)) {
        break;
      }
      ++delete_num;
      ++soldier_id;
    }
    result_list[save_id] = sentry_point_list;
    ++save_id;
    if (!(soldier_id < result_list.size())) {
      break;
    }
    sentry_id  = soldier_id;
    soldier_id = sentry_id + 1;
  }
  result_list.erase(result_list.begin() + save_id, result_list.end());
  return delete_num;
};
void Util::combinePointList(std::vector<std::vector<Point>>& result_list,
                            std::vector<Point>&              source_list,
                            int                              k,
                            bool (*filter)(std::vector<Point>&),
                            int (*unique)(std::vector<std::vector<Point>>&))
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
      std::vector<Point> temp_list;
      temp_list.reserve(k);
      for (int i = 0; i < k; i++) {
        // 拷贝至temp_list内
        temp_list.push_back(source_list[idx_list[i]]);
      }
      // 使用过滤器 过滤器可对每个结果进行修改以及判断是否需要放置
      if (filter(temp_list)) {
        result_list.push_back(move(temp_list));
        create_num++;
      }

      // 去重器
      int new_num = 500000;  //<! 新增数量
      if (create_num % new_num == 0) {
        // 每新增五十万去一次重
        int delete_num = unique(result_list);
        std::cout << "[INFO] Created " << result_list.size() << " combs (add "
                  << new_num - delete_num << " combs)" << std::endl;
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
  unique(result_list);

  std::cout << "[INFO] Created " << result_list.size() << " Combs" << std::endl;
};

void Util::productEdgeNumList(std::vector<std::vector<int>>& result_list,
                              std::vector<std::vector<int>>& source_list,
                              int                            upper_bound)
{
  if (source_list.size() == 0) {
    std::cout << "[WARN] source_list size is zero!!" << std::endl;
  }
  int powv_length = (int) source_list.size();
  int stop        = 0;
  // 初始化全零开始
  std::vector<int> idx_list;
  idx_list.resize(source_list.size());
  // 生成idx序列
  while (true) {
    // 将最后一位加至上界
    while (true) {
      std::vector<int> temp_list;
      for (size_t i = 0; i < idx_list.size(); i++) {
        temp_list.push_back(source_list[i][idx_list[i]]);
      }
      // 过滤
      int s = 0, first_num = temp_list[0];
      for (size_t i = 0; i < temp_list.size(); i++) {
        s += temp_list[i];
      }
      if (s <= upper_bound) {
        result_list.push_back(move(temp_list));
      } else {
        break;
      }
      // x11111型
      if ((first_num - 1) == (upper_bound - powv_length)) {
        stop = 1;
        break;
      }

      // 若已达到上界,终止累加
      if ((idx_list[source_list.size() - 1] + 1)
          == (int) source_list[source_list.size() - 1].size()) {
        break;
      }
      idx_list[source_list.size() - 1]++;
    }
    if (stop == 1) {
      break;
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
}

bool Util::filterInitEdgeList(std::vector<Edge>& temp_list)
{
  std::sort(temp_list.begin(), temp_list.end(), Util::compareEdgeASCByX);
  // 判断边是否有重叠 有重叠去重

  size_t save_id    = 0;
  size_t sentry_id  = 0;
  size_t soldier_id = sentry_id + 1;
  while (sentry_id < temp_list.size()) {
    Edge& sentry_edge = temp_list[sentry_id];
    while (soldier_id < temp_list.size()) {
      Edge& soldier_edge = temp_list[soldier_id];
      if (sentry_edge != soldier_edge) {
        break;
      }
      ++soldier_id;
    }
    temp_list[save_id] = sentry_edge;
    ++save_id;
    if (!(soldier_id < temp_list.size())) {
      break;
    }
    sentry_id  = soldier_id;
    soldier_id = sentry_id + 1;
  }

  temp_list.erase(temp_list.begin() + save_id, temp_list.end());

  return true;
}
int Util::UniqueInitEdgeList(std::vector<std::vector<Edge>>& result_list)
{
  // 先排序
  for (size_t i = 0; i < result_list.size(); i++) {
    std::sort(result_list[i].begin(), result_list[i].end(), Util::compareEdgeASCByX);
  }
  std::sort(result_list.begin(), result_list.end(), Util::compareEdgeListASCByX);

  // 后去重
  int delete_num = 0;

  size_t save_id    = 0;
  size_t sentry_id  = 0;
  size_t soldier_id = sentry_id + 1;
  while (sentry_id < result_list.size()) {
    std::vector<Edge>& sentry_edge_list = result_list[sentry_id];
    while (soldier_id < result_list.size()) {
      std::vector<Edge>& soldier_edge_list = result_list[soldier_id];
      if (!Util::compareEqualEdgeList(sentry_edge_list, soldier_edge_list)) {
        break;
      }
      ++delete_num;
      ++soldier_id;
    }
    result_list[save_id] = sentry_edge_list;
    ++save_id;
    if (!(soldier_id < result_list.size())) {
      break;
    }
    sentry_id  = soldier_id;
    soldier_id = sentry_id + 1;
  }

  result_list.erase(result_list.begin() + save_id, result_list.end());

  return delete_num;
}
void Util::productInitEdgeList(std::vector<std::vector<Edge>>& result_list,
                               std::vector<std::vector<Edge>>& source_list,
                               bool (*filter)(std::vector<Edge>&),
                               int (*unique)(std::vector<std::vector<Edge>>&))
{
  if (source_list.size() == 0) {
    std::cout << "[WARN] source_list size is zero!!" << std::endl;
  }
  // 初始化全零开始
  std::vector<int> idx_list;
  idx_list.resize(source_list.size());
  // 生成idx序列
  int create_num = 0;
  while (true) {
    // 将最后一位加至上界
    while (true) {
      std::vector<Edge> temp_list;
      temp_list.reserve(idx_list.size());
      for (size_t i = 0; i < idx_list.size(); i++) {
        // 拷贝边
        temp_list.push_back(source_list[i][idx_list[i]]);
      }
      // 使用过滤器 过滤器可对结果进行修改
      if (filter(temp_list)) {
        result_list.push_back(move(temp_list));
        create_num++;
      }

      // 去重器
      int new_num = 200000;  //<! 新增数量
      if (create_num % new_num == 0) {
        // 每新增二十万去一次重
        unique(result_list);
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
  // 全部去重
  unique(result_list);
}

bool Util::isMappingEdgeNum(int&              diff_s,
                            std::vector<int>& diff_num_list,
                            std::vector<int>& powv_edge_num_list,
                            std::vector<int>& pt_edge_num_list)
{
  if (powv_edge_num_list.size() != pt_edge_num_list.size()) {
    std::cout << "[ERROR] powv_edge_num_list.size() != pt_edge_num_list.size()" << std::endl;
  }

  diff_s = 0;
  diff_num_list.resize(powv_edge_num_list.size());
  for (size_t i = 0; i < powv_edge_num_list.size(); i++) {
    int diff = powv_edge_num_list[i] - pt_edge_num_list[i];
    if (diff < 0) {
      return false;
    }
    diff_s += diff;
    diff_num_list[i] = diff;
  }
  return true;
}

void Util::countPointDegree(std::vector<std::pair<Point, int>>& point_num_list, Point& point)
{
  int is_push = 1;
  for (size_t i = 0; i < point_num_list.size(); i++) {
    if (point_num_list[i].first == point) {
      is_push = 0;
      point_num_list[i].second++;
      break;
    }
  }
  if (is_push == 1) {
    point_num_list.push_back(std::make_pair(point, 1));
  }
}

bool Util::isExistIn(std::vector<Point>& point_list, Point& point)
{
  for (size_t i = 0; i < point_list.size(); i++) {
    if (point == point_list[i]) {
      return true;
    }
  }
  return false;
}

bool Util::isConnected(std::vector<Point>&                 given_point_list,
                       std::vector<Edge>&                  edge_list,
                       std::vector<std::pair<Point, int>>& point_num_list)
{
  // 初始化点访问记录
  std::vector<std::pair<Point, int>> point_visit_list;
  for (size_t i = 0; i < point_num_list.size(); i++) {
    point_visit_list.push_back(std::make_pair(point_num_list[i].first, 0));
  }

  // 初始化边访问记录
  std::vector<std::pair<Edge, int>> edge_visit_list;
  for (size_t i = 0; i < edge_list.size(); i++) {
    edge_visit_list.push_back(std::make_pair(edge_list[i], 0));
  }

  // 找一个度为1的点开始
  Point curr_point;
  for (size_t i = 0; i < point_num_list.size(); i++) {
    if (point_num_list[i].second == 1) {
      curr_point                 = point_num_list[i].first;
      point_visit_list[i].second = 1;
      break;
    }
  }

  // dfs
  std::stack<Point> point_stack;
  point_stack.push(curr_point);
  while (true) {
    // 从当前点出发，找到存在当前点且未访问的边 将此边置为visit
    // 若已经访问则跳过，若遍历完无可访问的边，则此点出栈 并将他的父节点设为当前节点
    int find_flag = 0;
    for (size_t i = 0; i < edge_visit_list.size(); i++) {
      if (edge_visit_list[i].second == 0) {
        Edge  edge = edge_visit_list[i].first;
        Point p1   = edge.get_p1();
        Point p2   = edge.get_p2();
        if (p1 == curr_point) {
          curr_point = p2;
          point_stack.push(curr_point);
          edge_visit_list[i].second = 1;
          find_flag                 = 1;
          break;
        } else if (p2 == curr_point) {
          curr_point = p1;
          point_stack.push(curr_point);
          edge_visit_list[i].second = 1;
          find_flag                 = 1;
          break;
        }
      }
    }

    if (find_flag == 0) {
      // 此点弹出
      point_stack.pop();
      // 若堆栈不为空
      if (!point_stack.empty()) {
        // 将父节点设为当前点
        curr_point = point_stack.top();
      } else {
        // 判断给定点是否都已遍历
        for (size_t i = 0; i < given_point_list.size(); i++) {
          for (size_t j = 0; j < point_visit_list.size(); j++) {
            if (given_point_list[i] == point_visit_list[j].first
                && point_visit_list[j].second == 0) {
              return false;
            }
          }
        }
        return true;
      }
    } else if (find_flag == 1) {
      // 查看备选点是否已经被访问 没被访问则将此点设为当前节点 否则存在环，返回flase
      for (size_t i = 0; i < point_visit_list.size(); i++) {
        if (curr_point == point_visit_list[i].first) {
          if (point_visit_list[i].second == 1) {
            return false;
          } else {
            point_visit_list[i].second = 1;
            break;
          }
        }
      }
    }
  }
}

bool Util::filterPOST(std::vector<Point>& given_point_list, std::vector<Edge>& temp_list)
{
  // 判断是否有重复的边
  sort(temp_list.begin(), temp_list.end(), compareEdgeASCByX);

  for (size_t i = 1; i < temp_list.size(); i++) {
    if (temp_list[i - 1] == temp_list[i]) {
      return false;
    }
  }

  // 判断在线路上"非给定点"的度 拒绝"非给定点"度为1
  std::vector<std::pair<Point, int>> point_num_list;
  point_num_list.reserve(temp_list.size() * 2);
  for (size_t i = 0; i < temp_list.size(); i++) {
    countPointDegree(point_num_list, temp_list[i].get_p1());
    countPointDegree(point_num_list, temp_list[i].get_p2());
  }

  for (size_t i = 0; i < point_num_list.size(); i++) {
    if (point_num_list[i].second == 1) {
      if (!isExistIn(given_point_list, point_num_list[i].first)) {
        return false;
      }
    }
  }

  // 无论连通与否 判断"边数==点数-1"是否成立
  if ((point_num_list.size() - 1) != temp_list.size()) {
    return false;
  }

  // 判断是否连通
  return isConnected(given_point_list, temp_list, point_num_list);
}
int Util::UniquePOST(std::vector<std::vector<Edge>>& result_list)
{
  // 先排序
  for (size_t i = 0; i < result_list.size(); i++) {
    std::sort(result_list[i].begin(), result_list[i].end(), Util::compareEdgeASCByX);
  }
  std::sort(result_list.begin(), result_list.end(), Util::compareEdgeListASCByX);
  // 后去重
  int delete_num = 0;

  size_t save_id    = 0;
  size_t sentry_id  = 0;
  size_t soldier_id = sentry_id + 1;
  while (sentry_id < result_list.size()) {
    std::vector<Edge>& sentry_edge_list = result_list[sentry_id];
    while (soldier_id < result_list.size()) {
      std::vector<Edge>& soldier_edge_list = result_list[soldier_id];
      if (!Util::compareEqualEdgeList(sentry_edge_list, soldier_edge_list)) {
        break;
      }
      ++delete_num;
      ++soldier_id;
    }
    result_list[save_id] = sentry_edge_list;
    ++save_id;
    if (!(soldier_id < result_list.size())) {
      break;
    }
    sentry_id  = soldier_id;
    soldier_id = sentry_id + 1;
  }
  result_list.erase(result_list.begin() + save_id, result_list.end());
  return delete_num;
}
void Util::productPOST(std::vector<std::vector<Edge>>& result_list,
                       std::vector<std::vector<Edge>>& source_list,
                       std::vector<Edge>&              pt_edge_list,
                       std::vector<Point>&             given_point_list,
                       bool (*filter)(std::vector<Point>& given_point_list,
                                      std::vector<Edge>&  temp_list),
                       int (*unique)(std::vector<std::vector<Edge>>&))
{
  if (source_list.size() == 0) {
    std::cout << "[WARN] source_list size is zero!!" << std::endl;
  }
  // 初始化全零开始
  std::vector<int> idx_list;
  idx_list.resize(source_list.size());
  // 生成idx序列
  int create_num = 0;
  while (true) {
    // 将最后一位加至上界
    while (true) {
      std::vector<Edge> temp_list;
      for (size_t i = 0; i < idx_list.size(); i++) {
        temp_list.push_back(source_list[i][idx_list[i]]);
      }
      // 加上 pt_edge_list
      temp_list.insert(temp_list.end(), pt_edge_list.begin(), pt_edge_list.end());
      // 使用过滤器
      if (filter(given_point_list, temp_list)) {
        result_list.push_back(move(temp_list));
        create_num++;
      }

      // 去重器
      int new_num = 200000;  //<! 新增数量
      if (create_num % new_num == 0) {
        // 每新增二十万去一次重
        unique(result_list);
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
  // 全部去重
  unique(result_list);
}

}  // namespace slut
